//ȫ�ֱ�����ʼ��

#include "stm32f10x_lib.h"
#include "extern_variables.h"

u8 Screen_update_Flag = 0;
u8 Poweron_TIM2_Enable_Flag = 0;
u8 Poweron_TIM2_Enable_Cut = 0;
u16  Alarm_num = 0;				   //������EEPROM�еľ����¼�����,��ʼ��Ϊ0����ַ0
u16  Alarm_End_num = 0;			   //������EEPROM�еľ��������¼�����,��ʼ��Ϊ0����ַ34
u16  Fault_num = 0;				   //������EEPROM�еĹ����¼�����,��ʼ��Ϊ0����ַ48
u16  Disable_num = 0;			   //������EEPROM�еĹر��¼�����,��ʼ��Ϊ0����ַ64

int Single_num = 0;				   //�з�����
u8 Single_temp1 = 0;
u8 Single_temp2 = 0;
u8 Single_Flag = 1;
u16 Spark_DANGER_A = 5;            //���μ�⵽�������ٽ�ֵ
u16 Spark_DANGER_B = 15;           //һ��ʱ���ڼ�⵽�����ۼӵ��ٽ�ֵ
u16 Spark_DANGER_B_M = 10;         //һ��ʱ���ڼ�⵽�����ۼӵļ���ʱ��   //��Spark_DANGER_M���ĵ��μ������
u16 Spark_DANGER_C_N = 20;         //һ��ʱ���ڳ�����⵽�𻨵ļ���ʱ��     //��Spark_DANGER_N���ĵ��μ������
u16 Alarm_TIME = 5;				   //���ⱨ��ʱ��
u16 AlarmTime_Cnt = 0;
u8 Alarm_Flag  = 0;
u16 SprayTime_Cnt = 0;
u8 Spray_Cnt = 0;
u16 Spray_Event[100] = {0};
u8 TIM3_Open_Cut = 0;
u8 Buzzer_Flag = 0;
u8 Alarm_reset_Flag = 0;
u8 Test_mode_Flag = 0;
u8 Test_result_Flag = 0;
u8 TimeDisplay = 0;
u8 RTCInitFlagStatus = 1;
u8 CPLD_0x840_Status = 0;

_CALENDAR calendar;
_DATA_SAVE Save_Info;
_DATA_SAVE Get_Info;
_DATA_SAVE_END Save_End_Info;
_DATA_SAVE_END Get_End_Info;
_DATA_SAVE_END Save_Fault_Info;
_DATA_SAVE_END Get_Fault_Info;
_DATA_SAVE_END Save_Disable_Info;
_DATA_SAVE_END Get_Disable_Info;				 

SCREEN_DATA  ScreenData;   //���յ���Ļ����
SCREEN_DATA  ScreenDataBuf;//���յ���Ļ���ݻ���buffer


