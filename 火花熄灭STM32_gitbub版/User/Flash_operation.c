#include "stm32f10x_it.h"
#include <stdio.h>
#include "extern_variables.h"

//芯片内部flash地址范围：0x08000000-0x0807FFFF（512K）
#define STARTADDR 0x0807F800    //主存储器第255页首地址,flash存储数据开始地址

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

/*******************************************************************************
* 函数名	: ReadFlashNByte(uint32_t ReadAddress,uint8_t *ReadBuf,int32_t ReadNum)
* 函数描述  : 读取flash内部的N字节数据
* 输入参数  : ReadAddress:指定读取地址
			  ReadBuf：数据存储指针
			  ReadNum：读取字节数
* 输出结果  : 无
* 返回值    : 无
*******************************************************************************/
void ReadFlashNByte(u32 ReadAddress,u32 *ReadBuf,u32 ReadNum)
{
	int DataNum = 0;

	ReadAddress = (u32)STARTADDR + ReadAddress;
	
	while(DataNum<ReadNum)
	{
		*(ReadBuf+DataNum) = *(u32 *)ReadAddress++;
		DataNum++;
	}		
}  


/*******************************************************************************
* 函数名	: ReadFlashNByte(uint32_t ReadAddress,uint8_t *ReadBuf,int32_t ReadNum)
* 函数描述  : 读取flash内部的N字节数据
* 输入参数  : ReadAddress:指定读取地址的偏移地址
			  p：数据存储指针
			  ReadNum：读取字节数
* 输出结果  : 无
* 返回值    : 无
*******************************************************************************/
void ReadFlashStruct(u32 ReadAddress,u8 *p,u32 ReadNum)
{
	int i = 0;

	ReadAddress = (u32)STARTADDR + ReadAddress;
	
	while(i<ReadNum)
	{
		*p = *(u32 *)ReadAddress++;
		i++;
		p++;
	}		
}

/*******************************************************************************
* 函数名	: WriteFlashOneWord(uint32_t WriteAddress,uint32_t WriteData)
* 函数描述  : 向内部flash写入数据
* 输入参数  : WriteAddress:待写入的地址
			  WriteData：写入的数据
* 输出结果  : 无
* 返回值    : 无
*******************************************************************************/
void WriteFlashOneWord(u32 WriteAddress,u32 WriteData)
{
	FLASH_Unlock();
	
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	
	FLASHStatus = FLASH_ErasePage(STARTADDR);//擦除指定地址页    FLASH_EraseAllPages();

	if(FLASHStatus == FLASH_COMPLETE)
	{
		FLASHStatus = FLASH_ProgramWord(STARTADDR + WriteAddress, WriteData); 	
	}

    FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位

	FLASH_Lock();
}

/*******************************************************************************
* 函数名	: WriteFlashStruct(u32 WriteAddress, u16 *p)
* 函数描述  : 向内部flash写入数据
* 输入参数  : WriteAddress:待写入的地址
			  p：写入的数据结构体指针
* 输出结果  : 无
* 返回值    : 无
*******************************************************************************/
void WriteFlashStruct(u32 WriteAddress, u16 *p)
{
	u8 i = 0;
	u8 words = sizeof(Save_Info)/2;//写入flash多少个16位

	FLASH_Unlock();
	
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	
	FLASHStatus = FLASH_ErasePage(STARTADDR);//擦除指定地址页    FLASH_EraseAllPages();	//擦除全部FLASH页面

	if(FLASHStatus == FLASH_COMPLETE)
	{
		while(i<words)
		{
			FLASHStatus = FLASH_ProgramHalfWord(STARTADDR + WriteAddress, *p); 		//一次写一个半字，即16位，占两个地址
			WriteAddress+=2;											  //ARM是32位机，一个字是32位，半字是16位，字节永远是8位
			p++;														  //stm32的flash是每8位一个寻址地址
			i++;
		}	
	}

    FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位

	FLASH_Lock();
}







