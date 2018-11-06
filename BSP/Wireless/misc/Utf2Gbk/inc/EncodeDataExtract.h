#ifndef __ENCODEDATAEXTRACT_H
#define __ENCODEDATAEXTRACT_H


#include "Encoding.h"
//#include "FlashDriver.h"
#include "W25Qxx.h"

#define Flash_Read_Data(a, b ,c)    W25Qxx_ReadBuffer(c, a, b)

short int ExtractBit16(uint32_t Address);
Summary16 * ExtractSummary16(uint32_t Address);
char ExtractBit8(uint32_t Address);


#endif

