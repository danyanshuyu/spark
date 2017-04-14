#include <stdlib.h>
#include "stm32f10x_lib.h"
#include "extern_variables.h"


#define  SprayAddrOffset  -4//(STARTADDR-4)  //喷水次数保存在该地址

u32 Spark_B[30] = {0};      //存储B火花总数的FIFO
u32 Spark_C[30] = {0};      //存储C火花总数的FIFO
u8  Cnt = 0;			   //滑动计数求和计数器
u8  Cnt_B = 0;				//B滑动计数器
u8  Cnt_C = 0;			    //C滑动计数器
u8  Cnt_B_Flag = 0;			//B一段时间第一个single_num不为0标志位,不为0时置1
u32 Cir_Spark_Sum_B = 0;     //滑动计数B火花总数
u32 Cir_Spark_Sum_C = 0;     //滑动计数C火花总数
u32 total_spray_num[1] = {0};		 //总的喷水次数
//u32 Write_addr = 0;         //下一次写入的起始地址

void Water_Spray();
void Data_Save(u32 num);
void Data_Get_Once(u8 Num);
void Data_Get_All();

/*******************************************************************************
* 函数名	: void Spark_test()
* 函数描述  : 火花检测程序
* 输入参数  : 无
* 输出结果  : 无
* 返回值    : 无
* 程序说明  : 如果单次检测到火花个数大于Spark_DANGER_A，执行喷水操作
			  如果滑动计数检测到火花个数大于Spark_DANGER_B，执行喷水操作
			  如果连续检测到火花，且时间大于等于Spark_DANGER_C_N，执行喷水操作	
*******************************************************************************/
void Spark_test()
{	
		if(Single_num>=Spark_DANGER_A)	 //A
		{
			//喷水并报警
			Water_Spray();

			//保留此刻的时间和火花个数										  //情形1
			Data_Save(Single_num);


		  	//B和C重新滑动计数
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
				 Cnt_B_Flag = 1;			  //一段时间第一个single_num不为0 标志位置1	，开始计数
				 Spark_B[Cnt_B++] = Single_num;

				 Cir_Spark_Sum_B = 0;
				 for(Cnt=0;Cnt<Spark_DANGER_B_M;Cnt++)
				 {
					 Cir_Spark_Sum_B += Spark_B[Cnt];
				 }

				 if(Cir_Spark_Sum_B >= Spark_DANGER_B)		//超过危险值，记录数据，重新计数
				 {
				     //喷水
			         Water_Spray();

					 //保留此刻的时间和火花个数
					 Data_Save(Cir_Spark_Sum_B);									   //情形2

					 //B重新滑动计数
					 for(Cnt=0;Cnt<(sizeof(Spark_B)/sizeof(Spark_B[0]));Cnt++)
					 {
						 Spark_B[Cnt] = 0;
					 }
					 Cnt_B = 0;
					 Cnt_B_Flag = 0;
				 }
				 if(Cnt_B >= Spark_DANGER_B_M)				 //超过指定时间，重新计数
				 {
					 //B重新滑动计数
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
						//喷水
			            Water_Spray();

						Cir_Spark_Sum_C = 0;
						for(Cnt=0;Cnt<Spark_DANGER_C_N;Cnt++)
						{
							Cir_Spark_Sum_C += Spark_C[Cnt];
						}

						//保留此刻的时间和火花个数
						Data_Save(Cir_Spark_Sum_C);										 //情形3

						//C重新滑动计数
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
				  if(Cnt_B_Flag == 0)			   //如果标志位还是为0，即第一个single_num不是非0数，不开始滑动计数
				  {
				     //B重新滑动计数
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

					  if(Cnt_B >= Spark_DANGER_B_M)				 //超过指定时间，重新计数
					  {
						  //B重新滑动计数
						  for(Cnt=0;Cnt<(sizeof(Spark_B)/sizeof(Spark_B[0]));Cnt++)
						  {
							  Spark_B[Cnt] = 0;
						  }
						  Cnt_B = 0;
						  Cnt_B_Flag = 0;
					  }
				  }

			      //C
				  //C重新滑动计数
				  for(Cnt=0;Cnt<(sizeof(Spark_C)/sizeof(Spark_C[0]));Cnt++)
				  {
					  Spark_C[Cnt] = 0;
				  }
				  Cnt_C = 0;
			 }
		}		 		 
}



/*******************************************************************************
* 函数名	: void Data_Save(u32 num)
* 函数描述  : 保留喷水时刻的火花个数和喷水时间
* 输入参数  : 火花个数
* 输出结果  : 无
* 返回值    : 无	
*******************************************************************************/
void Data_Save(u32 num)
{
    u16 temp=0;	  
	_DATA_SAVE *p;
	temp = _24C08_I2C_HalfwordRead(0);		  //读取EEPROM 0地址当前记录的火花事件个数
	temp++;

	if(temp >= 1000)
	{
		;
	}
	else
	{
		//获取时间
		Get_Cur_Time();
	
		//填充信息结构体
		Save_Info.spark_num = num;
		Save_Info.calendar = calendar;
		Save_Info.Event_Nr = temp;
		
		p = &Save_Info;
		
		//将Save_Info信息保存到SRAM
		FSMC_SRAM_WriteStruct(p,12*(temp-1));	 //结构体目前大小为12字节
		
		_24C08_I2C_HalfwordWrite(temp,0);		  //火花事件数加一后存入EEPROM的0地址
	}
}

/*******************************************************************************
* 函数名	: void Data_Get()
* 函数描述  : 从flash获取某一次的喷水时间和火花个数
* 输入参数  : 保存数据Buffer
* 输出结果  : 无
* 返回值    : 无	
*******************************************************************************/
void Data_Get_Once(u8 Num)
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
		offset = (Num-1)*sizeof(Save_Info);	//第Num次喷水数据存储地址的偏移值
	
		//读出数据保存在Get_Info结构体中	 
		//ReadFlashStruct(offset,(u8 *)p,sizeof(Save_Info));
	     FSMC_SRAM_ReadStruct(p,offset,12);			 //sizeof(Save_Info)大小为12
	}
}

/*******************************************************************************
* 函数名	: void Data_Get_All()
* 函数描述  : 从flash获取喷水时刻的火花个数和喷水时间
* 输入参数  : 保存数据Buffer
* 输出结果  : 无
* 返回值    : 无	
*******************************************************************************/
void Data_Get_All()
{
	
	//取出喷水总次数Spray_Num
	ReadFlashNByte(SprayAddrOffset,total_spray_num,4);
		
}



/*******************************************************************************
* 函数名	: void Water_Spray()
* 函数描述  : 喷水与报警操作
* 输入参数  : 无
* 输出结果  : 无
* 返回值    : 无	
*******************************************************************************/
void Water_Spray()
{
	 if(Alarm_reset_Flag == 1)			      //声光复位按钮按下时，不报警
	 {
     	;	
	 }
	 else
	 {
	 	 FSMC_CPLD_Write(0x01,0x840);         //开启报警灯
		 Spray_Flag  = 1;					  //开始喷水倒计时
		 Spray_Cnt = 0;						  //一旦有最新火花事件，从新计数
	 }
}








