
#ifndef _EXTERN_VARIABLES
#define _EXTERN_VARIABLES
#include "fsmc_cpld_sram.h"

//Flash��������
#define FLASH_PAGE_SIZE    ((u16)0x400)   //USE_STM3210B

//Spark_test��������
extern int Single_num;                  //һ��ʱ�䵥Ԫ�ڻ𻨸���
extern u8  Single_temp1;                //һ��ʱ�䵥Ԫ��ʼ�����ۼ����еĸ���
extern u8  Single_temp2;                //һ��ʱ�䵥Ԫ���˴����ۼ����еĸ���
extern u8  Single_Flag;                 //�Ƿ��һ�δӻ��ۼ����ж�ȡ���ݱ�־λ
extern u16 Spark_DANGER_A;              //���μ�⵽�������ٽ�ֵ
extern u16 Spark_DANGER_B;              //һ��ʱ���ڼ�⵽�����ۼӵ��ٽ�ֵ
extern u16 Spark_DANGER_B_M;            //һ��ʱ���ڼ�⵽�����ۼӵļ���ʱ��   //��Spark_DANGER_M���ĵ��μ������
extern u16 Spark_DANGER_C_N;            //һ��ʱ���ڳ�����⵽�𻨵ļ���ʱ��     //��Spark_DANGER_N���ĵ��μ������
extern u8  Alarm_Flag;				    //���ⱨ������
extern u16 Alarm_TIME;                  //���ⱨ��ʱ��
extern u16 AlarmTime_Cnt;               //���ⱨ��ʱ�����
extern u16 SprayTime_Cnt;               //��ˮʱ�����
extern u8  Spray_Cnt;                   //��ˮϨ���¼��жϼ���
extern u16 Spray_Event[100];            //��ˮϨ���¼�����
extern u8  TIM3_Open_Cut;               //��ˮ�¼��ж��ӳٶ�ʱ�������رռ���



extern void Spark_test(void);          //�𻨼�� 


//Function��������
extern u8 Screen_update_Flag;
extern u8 Poweron_TIM2_Enable_Flag;
extern u8 Poweron_TIM2_Enable_Cut;
extern u16  Alarm_num;				   //�����¼���
extern u16  Alarm_End_num;			   //���������¼���
extern u8 Buzzer_Flag;				   //����������������
extern u16  Disable_num;			   //�ر��¼���
extern u8 Alarm_reset_Flag;		       //���ⱨ���豸����
extern u8 Test_mode_Flag;			   //Ԫ������ģʽ
extern u8 Test_result_Flag;			   //���Խ������
extern u16  Fault_num;				   //�����¼���


//PCF8563_RTC ��������
typedef struct{
	vu8 year;
	vu8 month;
	vu8 date;

	vu8 hour;
	vu8 min;
	vu8 second;
}_CALENDAR;
extern _CALENDAR calendar;
extern void Set_Day_Time(u8 syear,u8 smon,u8 sday);
extern void Set_Second_Time(u8 hour,u8 min,u8 sec);
extern void Get_Cur_Time();
extern void RTC_Configuration();

extern u8 TimeDisplay;				   //�ж���ʾʱ����λ
extern u8 RTCInitFlagStatus;		   //RTC�Ƿ��ʼ����ʾ���λ
extern u8 PCF8563_Time[7];		       //��ʼ��ʱ����ʱ�䴢��:�룬�֣�ʱ���죬�����ڣ����·ݣ���ݡ�
extern u8 PCF8563_Set_Time[7];		   //��ʱ����ʱ�䴢��:�룬�֣�ʱ���죬�����ڣ����·ݣ���ݡ�

//24C08_EEPROM��������
extern u8 I2C_Data[17];
extern u8 I2C_Data1[17];
extern u16 Buffer2;

//CPLD��������
extern u8 CPLD_0x840_Status;

//Flash ��������
extern void ReadFlashNByte(u32 ReadAddress,u32 *ReadBuf,u32 ReadNum);
extern void ReadFlashStruct(u32 ReadAddress,u8 *p,u32 ReadNum);
extern void WriteFlashOneWord(u32 WriteAddress,u32 WriteData);
extern void WriteFlashStruct(u32 WriteAddress,u16 *p);

//Data_Save	��������
typedef struct{
	u32 spark_num;
	_CALENDAR calendar;
	u16 Event_Nr;	
}_DATA_SAVE;
extern _DATA_SAVE Save_Info;
extern _DATA_SAVE Get_Info;


typedef struct{
	u32 remain;
	_CALENDAR calendar;
	u16 Event_Nr;	
}_DATA_SAVE_END;
extern _DATA_SAVE_END Save_End_Info;	//Data_Save_End	��������
extern _DATA_SAVE_END Get_End_Info;
extern _DATA_SAVE_END Save_Fault_Info;	//Data_Save_Fault	��������
extern _DATA_SAVE_END Get_Fault_Info;
extern _DATA_SAVE_END Save_Disable_Info; //Data_Save_Disable	��������
extern _DATA_SAVE_END Get_Disable_Info;				 

//���յ�����Ļ����
typedef struct _SCREEN_DATA
{
	u16 Count;
	u8  RcevData[100];
	u16 TranFinsh;
	u16 Busy;
}SCREEN_DATA;

extern SCREEN_DATA  ScreenData;   //���յ���Ļ����
extern SCREEN_DATA  ScreenDataBuf;//���յ���Ļ���ݻ���buffer


extern void MemCopy(u8 * DestAddr, u8 *SourceAddr, u32 ulSize);
extern void ScreenDataRecv_Process();



#endif





