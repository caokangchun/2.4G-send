
#include "Common.h"
#include "BSPEEPROM.h"


//#define FLASH_PAGE_SIZE             ((uint16_t)0x400)	//1024

#define MM32_FLASH_BASE					0x8000000U
#define FLASH_PAGE_SIZE             	1024U	
#define MM32_PAGE_NUM		        	128U


static uint32_t StartAddress;             	//Stort使用的FLASH起始地址
static uint16_t Size;                     	//EEPROM大小
static bool Switch = FALSE;             	//使用开关，当初始化正确后有效


//不检查的写入  
static bool EEPROM_Write_NoCheck(uint32_t a,uint16_t *d,uint16_t l);

//*******************************************************************
//作用：EEPROM初始化
//参数：FlashSize-芯片FLASH大小，单位字节
//      EEPROMSize-定义EEPROM大小，单位字节，必须为2的倍数
//返回：TRUE-成功建立EEPROM
//      FALSE-失败建立EEPROM
//*******************************************************************
bool BSPEEPROMInit(uint32_t FlashSize, uint16_t EEPROMSize)
{
	
    //确保容量大小为偶数
    FlashSize &= 0xFFFFFFFE;
    EEPROMSize &= 0xFFFE;

    if(FlashSize < EEPROMSize)
    {
        Switch = FALSE;
        return(FALSE);
    }

    Size = EEPROMSize;

    //计算EEPROM使用FLASH的起始页面地址
    if(EEPROMSize % FLASH_PAGE_SIZE)
    {
        StartAddress = 0x8000000 + (FlashSize - FLASH_PAGE_SIZE * (EEPROMSize / FLASH_PAGE_SIZE + 1));
    }
    else
    {
        StartAddress = 0x8000000 + (FlashSize - EEPROMSize);
    }
	
    Switch = TRUE;
    return(TRUE);
}

//清空Address指向的值
//Address:指定的地址
static bool FLASHErase(uint32_t Address)
{
    uint32_t pageAddress;
    uint32_t tmp;
    uint16_t Data[FLASH_PAGE_SIZE / 2];
    uint8_t Page;
    uint16_t i = 0;
    FLASH_Status status;

    //获取指定地址所在页的FLASH首地址，并复制整个页
    Page = (Address - 0x8000000) / FLASH_PAGE_SIZE;
    pageAddress = 0x8000000 + Page * FLASH_PAGE_SIZE;
    tmp = pageAddress;
    while(i < (FLASH_PAGE_SIZE / 2))
    {
        Data[i] = *((uint32_t*)tmp);
        tmp += 2;
        ++i;
    }

    //在内存清除要修改的Address指向的值
    Data[(Address - pageAddress) / 2] = 0xFFFF;

    //刷除所在页
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	status = FLASH_ErasePage(pageAddress); 
    if(status != FLASH_COMPLETE)
    {
		FLASH_Lock();
        return(FALSE);
    }

    //数据回写，除了Address地址的空间
    i = 0;
    tmp = pageAddress;
    while(i < (FLASH_PAGE_SIZE / 2))
    {
        if(Data[i] != 0xFFFF)
        {
            FLASH_ProgramHalfWord(tmp, Data[i]);
        }
        tmp += 2;
        ++i;
    }
    FLASH_Lock();
    return(TRUE);
    
}

#if 0
//*******************************************************************
//作用：EEPROM写数据.2个字节（半字）
//参数：Address-写入的EEPROM地址.例:0x300
//      Data-写入的数据
//返回：TRUE-成功写入
//      FALSE-失败写入
//*******************************************************************
bool BSPEEPROMWrite_old(uint16_t Address, uint16_t Data)
{
    FLASH_Status status;
    uint32_t flashAddress;
    uint16_t tmp;

    if(Switch == FALSE)
    {
        return(FALSE);
    }

    if(Address >= Size / 2)
    {
        return(FALSE);
    }

    flashAddress = StartAddress + (Address * 2);

    //判断数据能否写入
    tmp = *((uint32_t*)flashAddress);
    if(tmp != 0xFFFF)
    {
        if(FLASHErase(flashAddress) == TRUE)
        {
            FLASH_Unlock();
            FLASH_ProgramHalfWord(flashAddress, Data);
            FLASH_Lock();
            return(TRUE);
        }
		return(FALSE);
    }
    else
    {
        FLASH_Unlock();
        fmc_flag_clear(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
        status = FLASH_ProgramHalfWord(flashAddress, Data);
        FLASH_Lock();
        if(status == FLASH_COMPLETE)
        {
            return(TRUE);
        }
        return(FALSE);
    }
}


//*******************************************************************
//从EEPROM地址开始写入指定长度的数据
//ReadAddr:起始地址.输入相对地址.
//DataBuf:数据指针
//DataLen:半字(16位)数.长度
//*******************************************************************
bool BSPEEPROMWriteBytes_old(uint16_t WriteAddr, uint16_t *DataBuf, uint16_t DataLen)
{
	uint16_t i;
	uint8_t re=1;
	
	for(i=0; i<DataLen; i++)
	{
		re &= BSPEEPROMWrite(WriteAddr, DataBuf[i]);
		WriteAddr ++;//地址增加.
		
	}
	if(re)
	{
		return(TRUE);
	}		
	else
	{
		return(FALSE);
	}		
	
}

#endif


//*******************************************************************
//不检查的写入
//WriteAddr:起始地址
//DataBuf:数据指针
//DataLen:半字(16位)数   
//*******************************************************************
static bool EEPROM_Write_NoCheck(uint32_t WriteAddr, uint16_t *DataBuf, uint16_t DataLen)   
{ 			 		
	FLASH_Status status;
	uint16_t i;
	
	
	for(i=0; i<DataLen; i++)
	{
		status = FLASH_ProgramHalfWord(WriteAddr, DataBuf[i]);
	    WriteAddr += 2;//地址增加2.
	}  
	
	if(status == FLASH_COMPLETE)
	{
		return(TRUE);
	}
	return(FALSE);
} 

//*******************************************************************
//作用：EEPROM写数据.2个字节（半字）
//参数：WriteAddr-写入的EEPROM地址.例:0x300
//      DataBuf-数据指针
//		DataLen:半字(16位)数(就是要写入的16位数据的个数.)
//返回：TRUE-成功写入
//      FALSE-失败写入
//*******************************************************************
bool BSPEEPROMWrite(uint32_t WriteAddr, uint16_t *DataBuf, uint16_t DataLen)
{
	uint32_t pagePos;	   					//页地址
	uint16_t pageOff;	   					//页内偏移地址(16位字计算)
	uint16_t pageRemain; 					//页内剩余地址(16位字计算)	   
 	uint16_t i;    
	bool  wStatus = TRUE;						//写入是否成功
	
	uint32_t offAddr;   					//去掉0X08000000后的地址
	uint32_t operateAddress;				//操作地址
	uint32_t flashAddress;					//Flash地址,绝对地址.
	uint16_t FlashBuf[FLASH_PAGE_SIZE/2];	//Flash数据存放缓冲区
	
	flashAddress = StartAddress + (WriteAddr * 2);
	
	if((flashAddress >= (MM32_FLASH_BASE + FLASH_PAGE_SIZE * MM32_PAGE_NUM)))
	{
		return(FALSE);//非法地址
	}
	
	FLASH_Unlock();									//解锁
	offAddr= flashAddress - MM32_FLASH_BASE;		//实际偏移地址.
	pagePos = offAddr / FLASH_PAGE_SIZE;			//页地址,第几页  0~64 for GD32F150G8
	pageOff = (offAddr % FLASH_PAGE_SIZE) / 2;		//在页内的偏移(2个字节为基本单位.)
	pageRemain = FLASH_PAGE_SIZE / 2 - pageOff;		//页剩余空间大小   
	
	if(DataLen <= pageRemain)
	{
		pageRemain = DataLen;						//不大于该页范围
	}
	
	while(1) 
	{	
		operateAddress = pagePos * FLASH_PAGE_SIZE + MM32_FLASH_BASE;
		BSPEEPROMReadBytes(operateAddress - StartAddress, FlashBuf,FLASH_PAGE_SIZE/2);//读出整个页的内容
		for(i=0; i<pageRemain; i++)//校验数据
		{
			if(FlashBuf[pageOff+i] != 0XFFFF)break;//需要擦除  	  
		}
		
		if(i < pageRemain)//需要擦除
		{
			FLASH_ErasePage(operateAddress);//擦除这个页
			for(i=0; i<pageRemain; i++)//复制
			{
				FlashBuf[i+pageOff] = DataBuf[i];	  
			}
			wStatus &= EEPROM_Write_NoCheck(operateAddress, FlashBuf, FLASH_PAGE_SIZE/2);//写入整个页  
		}
		else
		{
			wStatus &= EEPROM_Write_NoCheck(flashAddress, DataBuf, pageRemain);//写已经擦除了的,直接写入页剩余区间. 
		}		
		
		if(DataLen == pageRemain)
		{
			break;//写入结束了
		}
		else//写入未结束
		{
			pagePos++;						//页地址增1
			pageOff=0;						//偏移位置为0 	 
		   	DataBuf 	 += pageRemain;  	//指针偏移
			flashAddress += pageRemain;		//写地址偏移	   
		   	DataLen 	 -= pageRemain;		//字节(16位)数递减
			if(DataLen > (FLASH_PAGE_SIZE/2))
			{
				pageRemain = FLASH_PAGE_SIZE/2;//下一个页还是写不完
			}
			else 
			{
				pageRemain = DataLen;//下一个页可以写完了
			}
		}	 
	};	
	FLASH_Lock();//上锁
	
	return (wStatus);//返回写状态.1成功 0失败
}






//*******************************************************************
//作用：EEPROM读数据.2个字节（半字）
//参数：Address-读出的EEPROM地址
//返回：读出的数据.
//*******************************************************************
uint16_t BSPEEPROMRead(uint16_t Address)
{
    uint16_t tmp;
    uint32_t flashAddress;

	if(Switch == FALSE)
    {
        return(0xFFFF);
    }
	
	if(Address >= Size/2)
    {
        return(0xFFFF);
    }
	
	
    flashAddress = StartAddress + (Address * 2);//隔2个字节读取.	
	
    tmp = *((uint32_t*)flashAddress);
    return(tmp);
}

//*******************************************************************
//从EEPROM地址开始读出指定长度的数据
//ReadAddr:起始地址.输入相对地址.
//DataBuf:数据指针
//DataLen:半字(16位)数.长度
//*******************************************************************
void BSPEEPROMReadBytes(uint32_t ReadAddr, uint16_t *DataBuf, uint16_t DataLen)
{
	uint16_t i;
	
//	if(ReadAddr > StartAddress)//这个原来是为了BSPEEPROMWrite()读取数据时，输入的是绝对地址.
//	{
//		ReadAddr = ReadAddr - StartAddress;
//	}
	
	for(i=0;i<DataLen;i++)
	{
		DataBuf[i]=BSPEEPROMRead(ReadAddr);//读取2个字节.
//		ReadAddr+=2;//偏移2个字节.	
		ReadAddr++;//偏移.	
	}
}

//*******************************************************************
//作用：EEPROM格式化
//参数：无
//返回：TRUE-成功
//      FALSE-失败
//*******************************************************************
bool BSPEEPROMFormat(void)
{
    uint32_t addr = StartAddress;
    uint32_t i = 0;
    FLASH_Status status;

    if(Switch == FALSE)
    {
        return(FALSE);
    }

    while(i < Size)
    {
        FLASH_Unlock();
        status = FLASH_ErasePage(addr);
        if(status != FLASH_COMPLETE)
        {
            FLASH_Lock();
            return(FALSE);
        }
        addr += FLASH_PAGE_SIZE;
        i += FLASH_PAGE_SIZE;
    }
    FLASH_Lock();
    return(TRUE);
}

//*******************************************************************
//作用：EEPROM删除输出
//参数：AddrStart-开始地址
//      AddrEnd-结束地址    
//返回：TRUE-成功
//      FALSE-失败
//*******************************************************************
bool BSPEEPROMDelete(uint16_t AddrStart, uint16_t AddrEnd)
{
    uint32_t flashFromAddress;
    uint32_t flashToAddress;
    FLASH_Status status;

    if(Switch == FALSE)
    {
        return(FALSE);
    }

    if(AddrStart >= Size / 2 || AddrEnd >= Size / 2)
    {
        return(FALSE);
    }

    if(AddrEnd <= AddrStart)
    {
        return(FALSE);
    }

    flashFromAddress = StartAddress + (AddrStart * 2);
    flashToAddress = StartAddress + (AddrEnd * 2);

    while(flashFromAddress <=  flashToAddress)
    {
        if((flashFromAddress - 0x8000000) % FLASH_PAGE_SIZE)
        {
            if(FLASHErase(flashFromAddress) == FALSE)
            {
                return(FALSE);
            }
            flashFromAddress += 2;
        }
        else
        {
            if(flashToAddress - flashFromAddress >= FLASH_PAGE_SIZE - 2)
            {
                FLASH_Unlock();
                status = FLASH_ErasePage(flashFromAddress);
                FLASH_Lock();
                if(status != FLASH_COMPLETE)
                {
                    return(FALSE);
                }
                flashFromAddress += FLASH_PAGE_SIZE;
            }
            else
            {
                if(FLASHErase(flashFromAddress) == FALSE)
                {
                    return(FALSE);
                }
                flashFromAddress += 2;
            }
        }
    }
    return(TRUE);
}


