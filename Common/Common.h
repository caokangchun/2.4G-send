#ifndef		__COMMON_H__
#define		__COMMON_H__

#include <stdint.h>
#include "MM32F103.h"


#define 	COMM_SRAM_BB_BASE          	((Uint32)0x22000000)
#define 	COMM_PERIPH_BB_BASE        	((Uint32)0x42000000) 

//***************************************************
//Note:
//Define the SRAM bit operation that use M3 Bit-Band area.
//***************************************************
#define		SRAMSetBit(Data, BitNum)	(*((volatile uint32_t *)(COMM_SRAM_BB_BASE + ((((uint32_t)(&##Data)) - COMM_SRAM_BB_BASE) * 32UL) + BitNum * 4UL)) = 1UL)
#define		SRAMClrBit(Data, BitNum)	(*((volatile uint32_t *)(COMM_SRAM_BB_BASE + ((((uint32_t)(&##Data)) - COMM_SRAM_BB_BASE) * 32UL) + BitNum * 4UL)) = 0UL)
#define		SRAMGetBit(Data, BitNum)	(*((volatile uint32_t *)(COMM_SRAM_BB_BASE + ((((uint32_t)(&##Data)) - COMM_SRAM_BB_BASE) * 32UL) + BitNum * 4UL)))
#define		SRAMCplBit(Data, BitNum)	((Data) ^= (1UL << (BitNum)))

//***************************************************
//Note:
//Define the Peripheral bit operation that use M3 Bit-Band area.
//***************************************************
#define		PPHSetBit(Data, BitNum)	(*((volatile uint32_t *)(COMM_PERIPH_BB_BASE + ((((uint32_t)(&##Data)) - COMM_PERIPH_BB_BASE) * 32UL) + BitNum * 4UL)) = 1UL)
#define		PPHClrBit(Data, BitNum)	(*((volatile uint32_t *)(COMM_PERIPH_BB_BASE + ((((uint32_t)(&##Data)) - COMM_PERIPH_BB_BASE) * 32UL) + BitNum * 4UL)) = 0UL)
#define		PPHGetBit(Data, BitNum)	(*((volatile uint32_t *)(COMM_PERIPH_BB_BASE + ((((uint32_t)(&##Data)) - COMM_PERIPH_BB_BASE) * 32UL) + BitNum * 4UL)))
#define		PPHCplBit(Data, BitNum)	((Data) ^= (1UL << (BitNum)))

//***************************************************
//Note:
//Define the bit operation that not use M3 Bit-Band area.
//***************************************************
#define		SetBit(Data, BitNum)	((Data) |= (1UL << (BitNum)))
#define		ClrBit(Data, BitNum)	((Data) &= (~(1UL << (BitNum)))) 
#define		CplBit(Data, BitNum)	((Data) ^= (1UL << (BitNum)))
#define		GetBit(Data, BitNum)	(((Data) >> (BitNum)) & 0x1UL)

//***************************************************
//Note:
//Get the arrary's data length(The number of units). 
//***************************************************
#define     GetArrayLen(Array)  	(sizeof(Array)/sizeof(Array[0]))


#endif




















