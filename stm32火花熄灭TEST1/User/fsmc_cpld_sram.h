


#ifndef __FSMC_CPLD_SRAM_H
#define __FSMC_CPLD_SRAM_H


#include "stm32f10x_lib.h"

void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u16 *pBuffer, u32 WriteBuffer, u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u16 *pBuffer, u32 ReadBuffer, u32 NumHalfwordToRead);
void FSMC_CPLD_Init(void);
void FSMC_CPLD_Write(u8 pBuffer, u32 WriteAddr);
u8   FSMC_CPLD_Read(u32 ReadAddr);

#endif
