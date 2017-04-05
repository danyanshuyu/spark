#include "stm32f10x_lib.h"
#include "extern_variables.h"
#include "24C08_PCF8563_I2c.h"
//                        秒，分，时，天，星期，月份，年份。
u8 PCF8563_Set_Time[7] = {0, 0, 0, 0,   0,    0,   0};
/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : Configures the RTC.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{ 
  //启用PWR和BKP的时钟（from APB1）
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  //后备域解锁
  PWR_BackupAccessCmd(ENABLE);

  //备份寄存器模块复位
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
 
  //等待稳定
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  //RTC时钟源配置成LSE（外部32.768K）
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  //RTC开启
  RCC_RTCCLKCmd(ENABLE);

  //开启后需要等待APB1时钟与RTC时钟同步，才能读写寄存器
  RTC_WaitForSynchro();

  //读写寄存器前，要确定上一个操作已经结束
  RTC_WaitForLastTask();
  
  //使能秒中断
  RTC_ITConfig(RTC_IT_SEC, ENABLE);   

  //读写寄存器前，要确定上一个操作已经结束
  RTC_WaitForLastTask();

  //设置RTC分频器，使RTC时钟为1Hz
  //RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  //等待寄存器写入完成
  RTC_WaitForLastTask();
}


/******************************************************************************
* Function Name  : void Time_Regulate()
* Description    : 返回用户输入的时间
* Input          : None
* Output         : None
* Return         : RTC当前的计数值
******************************************************************************/
void Set_Day_Time(u8 syear,u8 smon,u8 sday)
{

       	PCF8563_Set_Time[6] = syear;
		PCF8563_Set_Time[5] = smon;
		PCF8563_Set_Time[3] = sday;
	    PCF8563_WriteAdress(0x00, 0x20);	//禁止RTC source clock
		PCF8563_WriteAdress(0x05, HEX_TO_BCD(PCF8563_Set_Time[3]));
		PCF8563_WriteAdress(0x07, HEX_TO_BCD(PCF8563_Set_Time[5]));
		PCF8563_WriteAdress(0x08, HEX_TO_BCD(PCF8563_Set_Time[6]));
		PCF8563_WriteAdress(0x00, 0x00);	//Enable RTC sorce clock
}

void Set_Second_Time(u8 hour,u8 min,u8 sec)
{
		PCF8563_Set_Time[0] = sec;
		PCF8563_Set_Time[1] = min;
		PCF8563_Set_Time[2] = hour;
       	PCF8563_WriteAdress(0x00, 0x20);	//禁止RTC source clock
	   	PCF8563_WriteAdress(0x02, HEX_TO_BCD(PCF8563_Set_Time[0]));			   
		PCF8563_WriteAdress(0x03, HEX_TO_BCD(PCF8563_Set_Time[1]));
		PCF8563_WriteAdress(0x04, HEX_TO_BCD(PCF8563_Set_Time[2]));
		PCF8563_WriteAdress(0x00, 0x00);	//Enable RTC sorce clock
}
/******************************************************************************
* Function Name  : void Time_Regulate()
* Description    : 返回用户输入的时间
* Input          : None
* Output         : None
* Return         : RTC当前的计数值
******************************************************************************/
void Get_Cur_Time()
{
		PCF8563_ReadTimes();
		calendar.year = PCF8563_Time[6];	//年		 //存入calender结构体
		calendar.month = PCF8563_Time[5];	//月
		calendar.date = PCF8563_Time[3];	//日
	    calendar.hour = PCF8563_Time[2];//小时
	    calendar.min = PCF8563_Time[1];//分钟
	    calendar.second = PCF8563_Time[0];//秒钟
}





