#include "stm32f10x_lib.h"
#include "extern_variables.h"
#include "24C08_PCF8563_I2c.h"
//                        �룬�֣�ʱ���죬���ڣ��·ݣ���ݡ�
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
  //����PWR��BKP��ʱ�ӣ�from APB1��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  //�������
  PWR_BackupAccessCmd(ENABLE);

  //���ݼĴ���ģ�鸴λ
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
 
  //�ȴ��ȶ�
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  //RTCʱ��Դ���ó�LSE���ⲿ32.768K��
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  //RTC����
  RCC_RTCCLKCmd(ENABLE);

  //��������Ҫ�ȴ�APB1ʱ����RTCʱ��ͬ�������ܶ�д�Ĵ���
  RTC_WaitForSynchro();

  //��д�Ĵ���ǰ��Ҫȷ����һ�������Ѿ�����
  RTC_WaitForLastTask();
  
  //ʹ�����ж�
  RTC_ITConfig(RTC_IT_SEC, ENABLE);   

  //��д�Ĵ���ǰ��Ҫȷ����һ�������Ѿ�����
  RTC_WaitForLastTask();

  //����RTC��Ƶ����ʹRTCʱ��Ϊ1Hz
  //RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  //�ȴ��Ĵ���д�����
  RTC_WaitForLastTask();
}


/******************************************************************************
* Function Name  : void Time_Regulate()
* Description    : �����û������ʱ��
* Input          : None
* Output         : None
* Return         : RTC��ǰ�ļ���ֵ
******************************************************************************/
void Set_Day_Time(u8 syear,u8 smon,u8 sday)
{

       	PCF8563_Set_Time[6] = syear;
		PCF8563_Set_Time[5] = smon;
		PCF8563_Set_Time[3] = sday;
	    PCF8563_WriteAdress(0x00, 0x20);	//��ֹRTC source clock
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
       	PCF8563_WriteAdress(0x00, 0x20);	//��ֹRTC source clock
	   	PCF8563_WriteAdress(0x02, HEX_TO_BCD(PCF8563_Set_Time[0]));			   
		PCF8563_WriteAdress(0x03, HEX_TO_BCD(PCF8563_Set_Time[1]));
		PCF8563_WriteAdress(0x04, HEX_TO_BCD(PCF8563_Set_Time[2]));
		PCF8563_WriteAdress(0x00, 0x00);	//Enable RTC sorce clock
}
/******************************************************************************
* Function Name  : void Time_Regulate()
* Description    : �����û������ʱ��
* Input          : None
* Output         : None
* Return         : RTC��ǰ�ļ���ֵ
******************************************************************************/
void Get_Cur_Time()
{
		PCF8563_ReadTimes();
		calendar.year = PCF8563_Time[6];	//��		 //����calender�ṹ��
		calendar.month = PCF8563_Time[5];	//��
		calendar.date = PCF8563_Time[3];	//��
	    calendar.hour = PCF8563_Time[2];//Сʱ
	    calendar.min = PCF8563_Time[1];//����
	    calendar.second = PCF8563_Time[0];//����
}





