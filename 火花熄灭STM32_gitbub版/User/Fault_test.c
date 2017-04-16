#include <stdlib.h>
#include "stm32f10x_lib.h"
#include "extern_variables.h"


void Data_Save_Fault();
void Data_Get_Once_Fault(u16 Num);


/*******************************************************************************
* 函数名	: void Data_Save_Fault()
* 函数描述  : 保留故障事件的时间
* 输入参数  : 无
* 输出结果  : 无
* 返回值    : 无	
*******************************************************************************/
void Data_Save_Fault()
{
    u16 temp=0;	  
	_DATA_SAVE_END *p;
	temp = _24C08_I2C_HalfwordRead(48);		  //读取EEPROM 48地址当前记录的故障事件个数
	temp++;
	//获取时间
	Get_Cur_Time();

	//填充信息结构体
	Save_Fault_Info.remain = 0;
	Save_Fault_Info.calendar = calendar;
	Save_Fault_Info.Event_Nr = temp;
	
	p = &Save_Fault_Info;
	
	//将Save_Fault_Info信息保存到SRAM
	FSMC_SRAM_WriteStruct(p,12*(temp-1)+0x10000);	 //结构体目前大小为12字节	,Fault信息起始地址为0x68010000
	
	_24C08_I2C_HalfwordWrite(temp,48);		  //火花事件数加一后存入EEPROM的48地址

}

/*******************************************************************************
* 函数名	: void Data_Get_Once_Fault(u8 Num)
* 函数描述  : 从SRAM获取某一次的故障事件的时间
* 输入参数  : 事件序号
* 输出结果  : 无
* 返回值    : 无	
*******************************************************************************/
void Data_Get_Once_Fault(u16 Num)
{
	int offset;
	_DATA_SAVE_END *p;
	p = &Get_Fault_Info;
	if(Num == 0)
	{
		;
	}
	else
	{
		offset = (Num-1)*sizeof(Save_Fault_Info)+0x10000;	//第Num次喷水数据存储地址的偏移值
	
	     FSMC_SRAM_ReadStruct(p,offset,12);			 //sizeof(Save_Fault_Info)大小为12
	}
}



