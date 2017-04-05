#include "stm32f10x_it.h"
#include <stdio.h>
#include "extern_variables.h"

//оƬ�ڲ�flash��ַ��Χ��0x08000000-0x0807FFFF��512K��
#define STARTADDR 0x0807F800    //���洢����255ҳ�׵�ַ,flash�洢���ݿ�ʼ��ַ

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

/*******************************************************************************
* ������	: ReadFlashNByte(uint32_t ReadAddress,uint8_t *ReadBuf,int32_t ReadNum)
* ��������  : ��ȡflash�ڲ���N�ֽ�����
* �������  : ReadAddress:ָ����ȡ��ַ
			  ReadBuf�����ݴ洢ָ��
			  ReadNum����ȡ�ֽ���
* ������  : ��
* ����ֵ    : ��
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
* ������	: ReadFlashNByte(uint32_t ReadAddress,uint8_t *ReadBuf,int32_t ReadNum)
* ��������  : ��ȡflash�ڲ���N�ֽ�����
* �������  : ReadAddress:ָ����ȡ��ַ��ƫ�Ƶ�ַ
			  p�����ݴ洢ָ��
			  ReadNum����ȡ�ֽ���
* ������  : ��
* ����ֵ    : ��
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
* ������	: WriteFlashOneWord(uint32_t WriteAddress,uint32_t WriteData)
* ��������  : ���ڲ�flashд������
* �������  : WriteAddress:��д��ĵ�ַ
			  WriteData��д�������
* ������  : ��
* ����ֵ    : ��
*******************************************************************************/
void WriteFlashOneWord(u32 WriteAddress,u32 WriteData)
{
	FLASH_Unlock();
	
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	
	FLASHStatus = FLASH_ErasePage(STARTADDR);//����ָ����ַҳ    FLASH_EraseAllPages();

	if(FLASHStatus == FLASH_COMPLETE)
	{
		FLASHStatus = FLASH_ProgramWord(STARTADDR + WriteAddress, WriteData); 	
	}

    FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ

	FLASH_Lock();
}

/*******************************************************************************
* ������	: WriteFlashStruct(u32 WriteAddress, u16 *p)
* ��������  : ���ڲ�flashд������
* �������  : WriteAddress:��д��ĵ�ַ
			  p��д������ݽṹ��ָ��
* ������  : ��
* ����ֵ    : ��
*******************************************************************************/
void WriteFlashStruct(u32 WriteAddress, u16 *p)
{
	u8 i = 0;
	u8 words = sizeof(Save_Info)/2;//д��flash���ٸ�16λ

	FLASH_Unlock();
	
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	
	FLASHStatus = FLASH_ErasePage(STARTADDR);//����ָ����ַҳ    FLASH_EraseAllPages();	//����ȫ��FLASHҳ��

	if(FLASHStatus == FLASH_COMPLETE)
	{
		while(i<words)
		{
			FLASHStatus = FLASH_ProgramHalfWord(STARTADDR + WriteAddress, *p); 		//һ��дһ�����֣���16λ��ռ������ַ
			WriteAddress+=2;											  //ARM��32λ����һ������32λ��������16λ���ֽ���Զ��8λ
			p++;														  //stm32��flash��ÿ8λһ��Ѱַ��ַ
			i++;
		}	
	}

    FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ

	FLASH_Lock();
}







