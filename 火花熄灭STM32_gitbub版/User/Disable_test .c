#include <stdlib.h>
#include "stm32f10x_lib.h"
#include "extern_variables.h"


void Data_Save_Disable();
void Data_Get_Once_Disable(u16 Num);


/*******************************************************************************
* ������	: void Data_Save_Disable()
* ��������  : �����ر��¼���ʱ��
* �������  : ��
* ������  : ��
* ����ֵ    : ��	
*******************************************************************************/
void Data_Save_Disable()
{
    u16 temp=0;	  
	_DATA_SAVE_END *p;
	temp = _24C08_I2C_HalfwordRead(64);		  //��ȡEEPROM 64��ַ��ǰ��¼�Ĺر��¼�����
	temp++;
	//��ȡʱ��
	Get_Cur_Time();

	//�����Ϣ�ṹ��
	Save_Disable_Info.remain = 0;
	Save_Disable_Info.calendar = calendar;
	Save_Disable_Info.Event_Nr = temp;
	
	p = &Save_Disable_Info;
	
	//��Save_Fault_Info��Ϣ���浽SRAM
	FSMC_SRAM_WriteStruct(p,12*(temp-1)+0x20000);	 //�ṹ��Ŀǰ��СΪ12�ֽ�	,Disable��Ϣ��ʼ��ַΪ0x68020000
	
	_24C08_I2C_HalfwordWrite(temp,64);		  //���¼�����һ�����EEPROM��48��ַ

}

/*******************************************************************************
* ������	: void Data_Get_Once_Disable(u8 Num)
* ��������  : ��SRAM��ȡĳһ�εĹر��¼���ʱ��
* �������  : �¼����
* ������  : ��
* ����ֵ    : ��	
*******************************************************************************/
void Data_Get_Once_Disable(u16 Num)
{
	int offset;
	_DATA_SAVE_END *p;
	p = &Get_Disable_Info;
	if(Num == 0)
	{
		;
	}
	else
	{
		offset = (Num-1)*sizeof(Save_Disable_Info)+0x20000;	//��Num����ˮ���ݴ洢��ַ��ƫ��ֵ
	
	     FSMC_SRAM_ReadStruct(p,offset,12);			 //sizeof(Save_Disable_Info)��СΪ12
	}
}



