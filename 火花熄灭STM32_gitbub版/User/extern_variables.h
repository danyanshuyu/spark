
#ifndef _EXTERN_VARIABLES
#define _EXTERN_VARIABLES
#include "fsmc_cpld_sram.h"

//Flash操作参数
#define FLASH_PAGE_SIZE    ((u16)0x400)   //USE_STM3210B

//Spark_test操作参数
extern int Single_num;                 //一个时间单元内火花个数
extern u8 Single_temp1;                 //一个时间单元开始处火花累加器中的个数
extern u8 Single_temp2;                 //一个时间单元终了处火花累加器中的个数
extern u8 Single_Flag;                 //是否第一次从火花累加器中读取数据标志位
extern u16 Spark_DANGER_A;              //单次检测到火花数的临界值
extern u16 Spark_DANGER_B;              //一段时间内检测到火花数累加的临界值
extern u16 Spark_DANGER_B_M;            //一段时间内检测到火花数累加的极限时间   //即Spark_DANGER_M倍的单次检测周期
extern u16 Spark_DANGER_C_N;            //一段时间内持续检测到火花的极限时间     //即Spark_DANGER_N倍的单次检测周期
extern u8  Spray_Flag;				   //喷水控制
extern u16 Spray_TIME;                 //喷水时间
extern u16 Spray_Cnt;                  //喷水时间计数
extern u8 Screen_update_Flag;
extern u8 Poweron_TIM2_Enable_Flag;
extern u8 Poweron_TIM2_Enable_Cut;
extern u16  Alarm_num;				   //警报事件数
extern u8 Buzzer_Flag;				   //触摸屏蜂鸣器控制
extern u16  Disable_num;			   //关闭事件数
extern u8 Alarm_reset_Flag;		       //声光报警设备控制
extern u8 Test_mode_Flag;			   //元件测试模式
extern u8 Test_result_Flag;			   //测试结果代码
extern u16  Fault_num;				   //故障事件数


extern void Spark_test(void);          //火花检测 


//Function操作参数
extern u8 TimeDisplay;				   //中断显示时间标记位
extern u8 RTCInitFlagStatus;		   //RTC是否初始化显示标记位
extern u8 PCF8563_Time[7];		       //初始化时设置时间储存:秒，分，时，天，（星期），月份，年份。
extern u8 PCF8563_Set_Time[7];		  //临时设置时间储存:秒，分，时，天，（星期），月份，年份。


//PCF8563_RTC 操作参数
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

//Flash 操作参数
extern void ReadFlashNByte(u32 ReadAddress,u32 *ReadBuf,u32 ReadNum);
extern void ReadFlashStruct(u32 ReadAddress,u8 *p,u32 ReadNum);
extern void WriteFlashOneWord(u32 WriteAddress,u32 WriteData);
extern void WriteFlashStruct(u32 WriteAddress,u16 *p);

//Data_Save	操作参数
typedef struct{
	u32 spark_num;
	_CALENDAR calendar;
	u16 Event_Nr;	
}_DATA_SAVE;
extern _DATA_SAVE Save_Info;
extern _DATA_SAVE Get_Info;

//Data_Save_Fault	操作参数
typedef struct{
	u32 remain;
	_CALENDAR calendar;
	u16 Event_Nr;	
}_DATA_SAVE_FAULT;
extern _DATA_SAVE_FAULT Save_Fault_Info;
extern _DATA_SAVE_FAULT Get_Fault_Info;

//Data_Save_Disable	操作参数
typedef struct{
	u32 remain;
	_CALENDAR calendar;
	u16 Event_Nr;	
}_DATA_SAVE_DISABLE;
extern _DATA_SAVE_DISABLE Save_Disable_Info;
extern _DATA_SAVE_DISABLE Get_Disable_Info;				 

//接收到的屏幕数据
typedef struct _SCREEN_DATA
{
	u16 Count;
	u8  RcevData[100];
	u16 TranFinsh;
	u16 Busy;
}SCREEN_DATA;

extern SCREEN_DATA  ScreenData;   //接收的屏幕数据
extern SCREEN_DATA  ScreenDataBuf;//接收的屏幕数据缓存buffer

//24C08_EEPROM操作参数
extern u8 I2C_Data[17];
extern u8 I2C_Data1[17];
extern u16 Buffer2;

extern void MemCopy(u8 * DestAddr, u8 *SourceAddr, u32 ulSize);
extern void ScreenDataRecv_Process();



#endif





