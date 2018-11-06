#ifndef	__EEPROM_H__
#define	__EEPROM_H__


#include <stdint.h>
#include "HAL_device.h"

/*注意使用此BSPEEPROM包需要栈内存远远大于512B，这是由于在读写时开辟了一个
约512大小的局部数组，而如果函数嵌套过多则会导致堆栈溢出导致系统卡死！！！*/

#include "Common.h"
#include "BSP.h"

//*******************************************************************
//作用：EEPROM初始化
//参数：FlashSize - 输入正在使用的芯片FLASH大小，用于定位内存边界
//      EEPROMSize - 自定义EEPROM大小，单位字节，必须为2的倍数
//返回：TRUE-成功建立EEPROM
//      FALSE-失败建立EEPROM
//*******************************************************************
extern bool BSPEEPROMInit(uint32_t FlashSize, uint16_t EEPROMSize);
//*******************************************************************
//作用：EEPROM写数据.2个字节（半字）
//参数：WriteAddr-写入的EEPROM地址，0～EEPROMSize 例:0x300
//      DataBuf-数据指针
//		DataLen:半字(16位)数(就是要写入的16位数据的个数.)
//		原则上DataLen应小于EEPROMSize - WriteAddr
//返回：TRUE-成功写入
//      FALSE-失败写入
//*******************************************************************
bool BSPEEPROMWrite(uint32_t WriteAddr, uint16_t *DataBuf, uint16_t DataLen);
//*******************************************************************
//作用：EEPROM读数据.2个字节（半字）
//参数：Address - 读出的EEPROM地址 0～EEPROMSize
//返回：data - 读出的数据.
//*******************************************************************
extern uint16_t BSPEEPROMRead(uint16_t Address);
//*******************************************************************
//作用：EEPROM格式化
//参数：void - 空
//返回：TRUE - 成功
//      FALSE - 失败
//*******************************************************************
extern bool BSPEEPROMFormat(void);
//*******************************************************************
//作用：EEPROM删除输出，删除一段的数据
//参数：AddrStart-开始地址
//      AddrEnd-结束地址    
//返回：TRUE-成功
//      FALSE-失败
//*******************************************************************
extern bool BSPEEPROMDelete(uint16_t AddrStart, uint16_t AddrEnd);

//*******************************************************************
//作用：从EEPROM地址开始写入指定长度的数据
//参数：ReadAddr:起始地址.输入相对地址，0～EEPROMSize
//		DataBuf:数据指针
//		DataLen:半字(16位)数.长度
//返回：void - 空
//*******************************************************************
extern bool BSPEEPROMWriteBytes(uint16_t WriteAddr, uint16_t *DataBuf, uint16_t DataLen);
//*******************************************************************
//作用：从EEPROM地址开始读出指定长度的数据
//参数：ReadAddr - 起始地址.0～EEPROMSize
//		DataBuf - 读缓存数据指针
//		DataLen - 半字(16位)数.长度
//返回：void - 空
//*******************************************************************
extern void BSPEEPROMReadBytes(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t DataLen);






#endif



