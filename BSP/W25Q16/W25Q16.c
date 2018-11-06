#include <stdio.h>
#include "Common.h"
#include "BSP.h"
#include "MM32F103.h"
#include "W25Q16.h"


#define W25Q_MAX_CAPACITY			2097152
#define W25Q_BLOCK_SIZE				65536
#define W25Q_SECTOR_SIZE			4096
#define W25Q_PAGE_SIZE				256

#define W25Q_CMD_MID_DID_READ		0x90
#define W25Q_CMD_READ				0x03
#define W25Q_CMD_PAGE_PROGRAM		0x02
#define W25Q_CMD_WRITE_ENABLE		0x06
#define W25Q_CMD_WRITE_DISABLE		0x04
#define W25Q_CMD_READ_SR1			0x05
#define W25Q_CMD_WRITE_SR1			0x01
#define W25Q_CMD_SECTOR_ERASE		0x20
#define W25Q_CMD_BLOCK_ERASE		0xD8
#define W25Q_CMD_CHIP_ERASE			0xC7
#define W25Q_CMD_RESET_ENABLE		0x66
#define W25Q_CMD_RESET				0x99


#define ClipEnable()				(GPIO_ResetBits(GPIOB, GPIO_Pin_0))
#define ClipDisable()				(GPIO_SetBits(GPIOB, GPIO_Pin_0))


static void SPIWrite(uint8_t Data)
{
	//while(SPI_GetFlagStatus(SPI1, SPI_FLAG_TXEPT) == RESET);
	SPI_SendData(SPI1, Data);
	while(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXAVL) == RESET);
	SPI_ReceiveData(SPI1);
}

static uint8_t SPIRead(void)
{
	uint8_t tmp;
	
	//while(SPI_GetFlagStatus(SPI1, SPI_FLAG_TXEPT) == RESET);
	SPI_SendData(SPI1, 0xFF);
	while(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXAVL) == RESET);
	tmp = SPI_ReceiveData(SPI1);
	return(tmp);
}

void W25Q16Init(void)
{
	SPI_InitTypeDef SPIStruct;
	GPIO_InitTypeDef GPIOStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIOStruct.GPIO_Pin  = GPIO_Pin_5;
	GPIOStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIOStruct);
	GPIOStruct.GPIO_Pin  = GPIO_Pin_7;
	GPIOStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIOStruct);
	GPIOStruct.GPIO_Pin  = GPIO_Pin_6;
	GPIOStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIOStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIOStruct.GPIO_Pin  = GPIO_Pin_0;
	GPIOStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIOStruct);

	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_DeInit(SPI1);
	SPIStruct.SPI_Mode = SPI_Mode_Master;
    SPIStruct.SPI_DataSize = SPI_DataSize_8b;
    SPIStruct.SPI_DataWidth = SPI_DataWidth_8b;
    SPIStruct.SPI_CPOL = SPI_CPOL_Low;
    SPIStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPIStruct.SPI_NSS = SPI_NSS_Soft;
    SPIStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPIStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1, &SPIStruct);
    
    SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Tx);
	SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Rx);
}

bool W25Q16Open(void)
{
	uint8_t tmp1;
	uint8_t tmp2;
	
	SPI_Cmd(SPI1, ENABLE);
	
	//获取出厂ID及产品ID，以检测芯片功能是否正常
	ClipEnable();
	SPIWrite(W25Q_CMD_MID_DID_READ);
	SPIWrite(0xFF);
	SPIWrite(0xFF);
	SPIWrite(0x00);
	tmp1 = SPIRead();
	tmp2 = SPIRead();
	ClipDisable();
	if(tmp1 == 0xFF || tmp2 == 0xFF)
	{
		return(FALSE);
	}
	
	return(TRUE);
}

void W25Q16Close(void)
{
	SPI_Cmd(SPI1, DISABLE);
}

void W25Q16Reset(void)
{
	ClipEnable();
	SPIWrite(W25Q_CMD_RESET_ENABLE);
	ClipDisable();
	ClipEnable();
	SPIWrite(W25Q_CMD_RESET);
	ClipDisable();
}

static bool ChipBusy(void)
{
	uint8_t tmp;
	
	ClipEnable();
	SPIWrite(W25Q_CMD_READ_SR1);
    tmp = SPIRead();
	ClipDisable();
	
	if(GetBit(tmp, 0) == 1)
	{
		return(TRUE);
	}
	return(FALSE);
}

void W25Q16SectorErase(uint32_t Address)
{
    if(Address >= W25Q_MAX_CAPACITY && (Address % W25Q_SECTOR_SIZE != 0))
	{
		return;
	}

    ClipEnable();
    SPIWrite(W25Q_CMD_WRITE_ENABLE); 
    ClipDisable();
    
    ClipEnable();
    SPIWrite(W25Q_CMD_SECTOR_ERASE); 
    SPIWrite((Address >> 16) & 0xFF); 
    SPIWrite((Address >> 8) & 0xFF); 
    SPIWrite(Address & 0xFF); 
    ClipDisable();

	while(ChipBusy() == TRUE)
	{
		//BSPWatchDogReset();
	}
}

void W25Q16BlockErase(uint32_t Address)
{
    if(Address >= W25Q_MAX_CAPACITY && (Address % W25Q_BLOCK_SIZE != 0))
	{
		return;
	}
    
    ClipEnable();
    SPIWrite(W25Q_CMD_WRITE_ENABLE); 
    ClipDisable();
    
    ClipEnable();
    SPIWrite(W25Q_CMD_BLOCK_ERASE); 
    SPIWrite((Address >> 16) & 0xFF); 
    SPIWrite((Address >> 8) & 0xFF); 
    SPIWrite(Address & 0xFF); 
    ClipDisable();

	while(ChipBusy() == TRUE)
	{
		//BSPWatchDogReset();
	}
}

void W25Q16ClipErase(void)
{    
    ClipEnable();
    SPIWrite(W25Q_CMD_WRITE_ENABLE); 
    ClipDisable();
    
    ClipEnable();
    SPIWrite(W25Q_CMD_CHIP_ERASE); 
    ClipDisable();

	while(ChipBusy() == TRUE)
	{
		//BSPWatchDogReset();
	}
}

void W25Q16PageProgram(uint32_t Address, const uint8_t* Data, uint16_t Length)
{	
	uint8_t offset;
	uint16_t i = 0;
	
	if(Address >= W25Q_MAX_CAPACITY)
	{
		return;
	}
	
	//限制写数据区域在起始地址至Page末尾，防止数据覆盖
	offset = Address % W25Q_PAGE_SIZE;
	if(Length > (W25Q_PAGE_SIZE - offset))
	{
		Length = W25Q_PAGE_SIZE - offset;
	}

    ClipEnable();
    SPIWrite(W25Q_CMD_WRITE_ENABLE); 
    ClipDisable();
    
    ClipEnable();
    SPIWrite(W25Q_CMD_PAGE_PROGRAM);
	SPIWrite((Address >> 16) & 0xFF); 
	SPIWrite((Address >> 8) & 0xFF); 
	SPIWrite(Address & 0xFF); 
	while(i < Length)
	{	
		SPIWrite(*(Data + i));
		++i;
	}
    ClipDisable();

	while(ChipBusy() == TRUE)
	{
		//BSPWatchDogReset();
	}
}

void W25Q16Read(uint32_t Address, uint8_t* Data, uint16_t Length)
{
	uint16_t i = 0;
	
    if(Address > W25Q_MAX_CAPACITY)
	{
		return;
	}
	
	ClipEnable();
    SPIWrite(W25Q_CMD_READ);
    SPIWrite((Address >> 16) & 0xFF); 
    SPIWrite((Address >> 8) & 0xFF); 
    SPIWrite(Address & 0xFF);
	while(i < Length)
	{	
		*(Data + i) = SPIRead();
		++i;
	}
    ClipDisable();
}


