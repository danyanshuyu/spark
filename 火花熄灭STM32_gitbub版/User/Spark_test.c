#include <stdlib.h>
#include "stm32f10x_lib.h"
#include "extern_variables.h"


#define  SprayAddrOffset  -4//(STARTADDR-4)  //��ˮ���������ڸõ�ַ

u32 Spark_B[30] = {0};      //�洢B��������FIFO
u32 Spark_C[30] = {0};      //�洢C��������FIFO
u8  Cnt = 0;			   //����������ͼ�����
u8  Cnt_B = 0;				//B����������
u8  Cnt_C = 0;			    //C����������
u8  Cnt_B_Flag = 0;			//Bһ��ʱ���һ��single_num��Ϊ0��־λ,��Ϊ0ʱ��1
u32 Cir_Spark_Sum_B = 0;     //��������B������
u32 Cir_Spark_Sum_C = 0;     //��������C������


void Water_Spray();
void Data_Save(u32 num);
void Data_Save_End();
void Data_Get_Once(u16 Num);
void Data_End_Get_Once(u16 Num);
void Data_Get_All();

/*******************************************************************************
* ������	: void Spark_test()
* ��������  : �𻨼�����
* �������  : ��
* ������  : ��
* ����ֵ    : ��
* ����˵��  : ������μ�⵽�𻨸�������Spark_DANGER_A��ִ����ˮ����
			  �������������⵽�𻨸�������Spark_DANGER_B��ִ����ˮ����
			  ���������⵽�𻨣���ʱ����ڵ���Spark_DANGER_C_N��ִ����ˮ����	
*******************************************************************************/
void Spark_test()
{	
		if(Single_num>=Spark_DANGER_A)	 //A
		{
			//��ˮ������
			Water_Spray();

			//�����˿̵�ʱ��ͻ𻨸���										  //����1
			Data_Save(Single_num);


		  	//B��C���»�������
			for(Cnt=0;Cnt<(sizeof(Spark_B)/sizeof(Spark_B[0]));Cnt++)
			{
				Spark_B[Cnt] = 0;
			}
			Cnt_B = 0;
			Cnt_B_Flag = 0;
			for(Cnt=0;Cnt<(sizeof(Spark_C)/sizeof(Spark_C[0]));Cnt++)
			{
				Spark_C[Cnt] = 0;
			}
			Cnt_C = 0;
		}
		else
		{
			 if(Single_num != 0)
			 {
				 //B
				 Cnt_B_Flag = 1;			  //һ��ʱ���һ��single_num��Ϊ0 ��־λ��1	����ʼ����
				 Spark_B[Cnt_B++] = Single_num;

				 Cir_Spark_Sum_B = 0;
				 for(Cnt=0;Cnt<Spark_DANGER_B_M;Cnt++)
				 {
					 Cir_Spark_Sum_B += Spark_B[Cnt];
				 }

				 if(Cir_Spark_Sum_B >= Spark_DANGER_B)		//����Σ��ֵ����¼���ݣ����¼���
				 {
				     //��ˮ
			         Water_Spray();

					 //�����˿̵�ʱ��ͻ𻨸���
					 Data_Save(Cir_Spark_Sum_B);									   //����2

					 //B���»�������
					 for(Cnt=0;Cnt<(sizeof(Spark_B)/sizeof(Spark_B[0]));Cnt++)
					 {
						 Spark_B[Cnt] = 0;
					 }
					 Cnt_B = 0;
					 Cnt_B_Flag = 0;
				 }
				 if(Cnt_B >= Spark_DANGER_B_M)				 //����ָ��ʱ�䣬���¼���
				 {
					 //B���»�������
					 for(Cnt=0;Cnt<(sizeof(Spark_B)/sizeof(Spark_B[0]));Cnt++)
					 {
						 Spark_B[Cnt] = 0;
					 }
					 Cnt_B = 0;
					 Cnt_B_Flag = 0;
				 }

			     //C
			     Spark_C[Cnt_C++] = Single_num;
				 	if(Cnt_C >= Spark_DANGER_C_N)
					{
						//��ˮ
			            Water_Spray();

						Cir_Spark_Sum_C = 0;
						for(Cnt=0;Cnt<Spark_DANGER_C_N;Cnt++)
						{
							Cir_Spark_Sum_C += Spark_C[Cnt];
						}

						//�����˿̵�ʱ��ͻ𻨸���
						Data_Save(Cir_Spark_Sum_C);										 //����3

						//C���»�������
						for(Cnt=0;Cnt<(sizeof(Spark_C)/sizeof(Spark_C[0]));Cnt++)
						{
							Spark_C[Cnt] = 0;
						}
						Cnt_C = 0;
					}
			 }
			 if(Single_num == 0)
			 {
				  //B
				  if(Cnt_B_Flag == 0)			   //�����־λ����Ϊ0������һ��single_num���Ƿ�0��������ʼ��������
				  {
				     //B���»�������
					 for(Cnt=0;Cnt<(sizeof(Spark_B)/sizeof(Spark_B[0]));Cnt++)
					 {
						 Spark_B[Cnt] = 0;
					 }
					 Cnt_B = 0;
					 Cnt_B_Flag = 0;
				  }
				  if(Cnt_B_Flag == 1)
				  {
					  Spark_B[Cnt_B++] = Single_num;

					  Cir_Spark_Sum_B = 0;
					  for(Cnt=0;Cnt<Spark_DANGER_B_M;Cnt++)
					  {
						  Cir_Spark_Sum_B += Spark_B[Cnt];
					  }

					  if(Cnt_B >= Spark_DANGER_B_M)				 //����ָ��ʱ�䣬���¼���
					  {
						  //B���»�������
						  for(Cnt=0;Cnt<(sizeof(Spark_B)/sizeof(Spark_B[0]));Cnt++)
						  {
							  Spark_B[Cnt] = 0;
						  }
						  Cnt_B = 0;
						  Cnt_B_Flag = 0;
					  }
				  }

			      //C
				  //C���»�������
				  for(Cnt=0;Cnt<(sizeof(Spark_C)/sizeof(Spark_C[0]));Cnt++)
				  {
					  Spark_C[Cnt] = 0;
				  }
				  Cnt_C = 0;
			 }
		}		 		 
}



/*******************************************************************************
* ������	: void Data_Save(u32 num)
* ��������  : ������ˮʱ�̵Ļ𻨸�������ˮʱ��
* �������  : �𻨸���
* ������  : ��
* ����ֵ    : ��	
*******************************************************************************/
void Data_Save(u32 num)
{
    u16 temp=0;	  
	_DATA_SAVE *p;
	temp = _24C08_I2C_HalfwordRead(0);		  //��ȡEEPROM 0��ַ��ǰ��¼�Ļ��¼�����
	temp++;

	if(temp >= 1000)
	{
		;
	}
	else
	{
		//��ȡʱ��
		Get_Cur_Time();
	
		//�����Ϣ�ṹ��
		Save_Info.spark_num = num;
		Save_Info.calendar = calendar;
		Save_Info.Event_Nr = temp;
		
		p = &Save_Info;
		
		//��Save_Info��Ϣ���浽SRAM
		FSMC_SRAM_WriteStruct(p,24*(temp-1));	 //��ʼ�ͽ����¼����ṹ�干ռ24�ֽڣ���ʼ�¼��ṹ��ռǰ12�ֽ�
		
		_24C08_I2C_HalfwordWrite(temp,0);		  //���¼�����1�����EEPROM��0��ַ
	}
}

/*******************************************************************************
* ������	: void Data_Save_End()
* ��������  : ������ˮ����ʱ�̵�ʱ��
* �������  : ��
* ������  : ��
* ����ֵ    : ��	
*******************************************************************************/
void Data_Save_End()
{
    u16 temp=0;	  
	_DATA_SAVE_END *p;
	temp = _24C08_I2C_HalfwordRead(34);		  //��ȡEEPROM 34��ַ��ǰ��¼�Ļ𻨽����¼�����
	temp++;

	if(temp >= 1000)
	{
		;
	}
	else
	{
		//��ȡʱ��
		Get_Cur_Time();
	
		//�����Ϣ�ṹ��
		Save_End_Info.remain = 0;
	    Save_End_Info.calendar = calendar;
	    Save_End_Info.Event_Nr = temp;
		
		p = &Save_End_Info;
		
		//��Save_Info��Ϣ���浽SRAM
		FSMC_SRAM_WriteStruct(p,24*(temp-1)+12);	 //��ʼ�ͽ����¼����ṹ�干ռ24�ֽڣ������¼��ṹ��ռ��12�ֽ�
		
		_24C08_I2C_HalfwordWrite(temp,34);		     //�𻨽����¼�����1�����EEPROM��34��ַ
	}
}

/*******************************************************************************
* ������	: void Data_Get()
* ��������  : ��flash��ȡĳһ�ε���ˮʱ��ͻ𻨸���
* �������  : ��������Buffer
* ������  : ��
* ����ֵ    : ��	
*******************************************************************************/
void Data_Get_Once(u16 Num)
{
	int offset;
	_DATA_SAVE *p;
	p = &Get_Info;
	if(Num == 0)
	{
		;
	}
	else
	{
		offset =2*(Num-1)*sizeof(Save_Info);	//��Num����ˮ���ݴ洢��ַ��ƫ��ֵ
	
		//�������ݱ�����Get_Info�ṹ����	 
		//ReadFlashStruct(offset,(u8 *)p,sizeof(Save_Info));
	     FSMC_SRAM_ReadStruct(p,offset,12);			 //sizeof(Save_Info)��СΪ12
	}
}

/*******************************************************************************
* ������	: void Data_End_Get_Once(u16 Num)
* ��������  : ��flash��ȡĳһ�ε���ˮʱ��ͻ𻨸���
* �������  : ��������Buffer
* ������  : ��
* ����ֵ    : ��	
*******************************************************************************/
void Data_End_Get_Once(u16 Num)
{
	int offset;
	_DATA_SAVE_END *p;
	p = &Get_End_Info;
	if(Num == 0)
	{
		;
	}
	else
	{
		offset = 2*(Num-1)*sizeof(Save_Info)+12;	//��Num����ˮ�����¼��洢��ַ��ƫ��ֵ
	
	     FSMC_SRAM_ReadStruct(p,offset,12);			 //sizeof(Save_Info)��СΪ12
	}
}


/*******************************************************************************
* ������	: void Water_Spray()
* ��������  : ��ˮ�뱨������
* �������  : ��
* ������  : ��
* ����ֵ    : ��	
*******************************************************************************/
void Water_Spray()
{
     //������ˮ
	 FSMC_CPLD_Write(CPLD_0x840_Status|=IO_SPRAY_EN,0x840);         //������ˮ�̵���
	 if(Spray_Cnt == 100) 
	 {
	 	Spray_Cnt = 0;
	 }
	 else
	 {
	 	Spray_Cnt++;
	 }
	 Spray_Event[Spray_Cnt] = 1;
	 SprayTime_Cnt = 1;										//��ˮ�ڼ��ڣ����ַ������¼�����ˮʱ��˳��
	 TIM3_Open_Cut = 0;
	 TIM_Cmd(TIM3,ENABLE);
	 

	 //�������ⱨ��
	 if(Alarm_reset_Flag == 1)			                       //���⸴λ��ť����ʱ��������
	 {
     	;	
	 }
	 else
	 {
	 	 FSMC_CPLD_Write(CPLD_0x840_Status|=IO_ALARM_EN,0x840);         //����������
		 Alarm_Flag  = 1;					                     //��ʼ���ⱨ������ʱ
		 AlarmTime_Cnt = 0;						                 //һ�������»��¼������¼���
	 }

}








