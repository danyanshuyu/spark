#include <stdlib.h>
#include "stm32f10x_lib.h"
#include "extern_variables.h"


void Data_Save_Fault();
void Data_Get_Once_Fault(u8 Num);


/*******************************************************************************
* ������	: void Data_Save(u32 num)
* ��������  : ������ˮʱ�̵Ļ𻨸�������ˮʱ��
* �������  : �𻨸���
* ������  : ��
* ����ֵ    : ��	
*******************************************************************************/
void Data_Save_Fault()
{
    u16 temp=0;	  
	_DATA_SAVE_FAULT *p;
    Delay_ms(0x58);							  //�����Ķ�д����֮������еȴ�ʱ�䣬ʱ�����EEPROM��������ȷ��д
	temp = _24C08_I2C_HalfwordRead(48);		  //��ȡEEPROM 48��ַ��ǰ��¼�Ļ��¼�����
	temp++;
	//��ȡʱ��
	Get_Cur_Time();

	//�����Ϣ�ṹ��
	Save_Fault_Info.remain = 0;
	Save_Fault_Info.calendar = calendar;
	Save_Fault_Info.Event_Nr = temp;
	
	p = &Save_Fault_Info;
	
	//��Save_Fault_Info��Ϣ���浽SRAM
	FSMC_SRAM_WriteStruct(p,12*(temp-1)+0x10000);	 //�ṹ��Ŀǰ��СΪ12�ֽ�	,Fault��Ϣ��ʼ��ַΪ0x68010000
	
	_24C08_I2C_HalfwordWrite(temp,48);		  //���¼�����һ�����EEPROM��48��ַ

}

/*******************************************************************************
* ������	: void Data_Get()
* ��������  : ��flash��ȡĳһ�ε���ˮʱ��ͻ𻨸���
* �������  : ��������Buffer
* ������  : ��
* ����ֵ    : ��	
*******************************************************************************/
void Data_Get_Once_Fault(u8 Num)
{
	int offset;
	_DATA_SAVE_FAULT *p;
	p = &Get_Fault_Info;
	if(Num == 0)
	{
		;
	}
	else
	{
		offset = (Num-1)*sizeof(Save_Fault_Info)+0x10000;	//��Num����ˮ���ݴ洢��ַ��ƫ��ֵ
	
	     FSMC_SRAM_ReadStruct(p,offset,12);			 //sizeof(Save_Fault_Info)��СΪ12
	}
}



