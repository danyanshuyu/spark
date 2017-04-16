//全局变量初始化

#include "stm32f10x_lib.h"
#include "extern_variables.h"

u8 Screen_update_Flag = 0;
u8 Poweron_TIM2_Enable_Flag = 0;
u8 Poweron_TIM2_Enable_Cut = 0;
u16  Alarm_num = 0;				   //储存在EEPROM中的警报事件总数,初始化为0，地址0
u16  Alarm_End_num = 0;			   //储存在EEPROM中的警报结束事件总数,初始化为0，地址34
u16  Fault_num = 0;				   //储存在EEPROM中的故障事件总数,初始化为0，地址48
u16  Disable_num = 0;			   //储存在EEPROM中的关闭事件总数,初始化为0，地址64

int Single_num = 0;				   //有符号数
u8 Single_temp1 = 0;
u8 Single_temp2 = 0;
u8 Single_Flag = 1;
u16 Spark_DANGER_A = 5;            //单次检测到火花数的临界值
u16 Spark_DANGER_B = 15;           //一段时间内检测到火花数累加的临界值
u16 Spark_DANGER_B_M = 10;         //一段时间内检测到火花数累加的极限时间   //即Spark_DANGER_M倍的单次检测周期
u16 Spark_DANGER_C_N = 20;         //一段时间内持续检测到火花的极限时间     //即Spark_DANGER_N倍的单次检测周期
u16 Alarm_TIME = 5;				   //声光报警时间
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

SCREEN_DATA  ScreenData;   //接收的屏幕数据
SCREEN_DATA  ScreenDataBuf;//接收的屏幕数据缓存buffer


