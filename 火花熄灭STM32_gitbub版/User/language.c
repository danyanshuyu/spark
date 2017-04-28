#include "stm32f10x_lib.h"
#include "language.h"

u8 Lang_Flag = 0;   //Flag为0 表示中文显示，
                    //Flag为1 表示英文显示，
                    //对应字符型指针二维数组的第一维数值

char *Content[2][120]={{  //  0      1      2      3      4      5      6      7      8      9            																	
					 /*0*/ "警报","开始","结束","序号：","警报详情","L001 过滤装置","DG001 火花数 103","L002 气旋分离器","开始执行熄灭程序","传感器确认开启",
					 /*1*/ "传感器关闭完成","熄灭完成","传感器确认关闭","故障","检测不到传感器","开启开关发生错误","故障详情","未能检测到传感器","请检查传感器是否已接上","或查看传感器是否已损坏",
					 /*2*/ "关闭","关闭探测组","关闭熄灭组","系统","  1   输入密码","  2   查看储存信息","  3   关闭操作","  4   测试元件功能","  5   维护模式","  6   安装参数",	   
					 /*3*/ "  7   系统参数操作","  8   关闭系统","请点击“小键盘”输入","格式为“XXXX”","611 火花极限，单次火花数 (单位：个数)","612 火花极限，持续火花数 (单位：个数)","613 火花极限，火花时间 (单位：秒)","614 时间极限 (单位：秒)","615 熄灭时间 (单位：秒)","格式为“X”，“XX”或“XXX”",
					 /*4*/ "当前值","修改值","61 探头组参数","62 增压参数","63 防冻参数","64 系统关闭参数","7 调整系统参数","71 更改语言","72 更改日期","73 更改时间",
					 /*5*/ "74 更改密码","75 删除数据操作","76 调整对比度","77 开关内部喇叭","78 更新控制器","79 设置控制器","请点击“小键盘”输入日期","格式为“XX.XX.XX”","当前为","修改为",
					 /*6*/ "请输入原密码","21 查看警报消息","22 查看故障消息","23 查看关闭消息","历史信息","751 删除警报历史信息","752 删除故障历史信息","753 删除关闭历史信息","确认删除全部警报信息？","按“叉号”键取消，按“钩号”键确认",
					 /*7*/ "已删除全部警报信息！","确认测试传感器元件？","正在进行测试。。。","请稍等！","此功能有待开发","数据超过规定容量！","请及时清理数据","611 火花极限，单次火花数","612 火花极限，持续火花数","613 火花极限，火花时间",
					 /*8*/ "614 时间极限","615 熄灭时间","火花数    ","       正在喷水","  对不起，您没有权限访问！","密码正确！","请再次点击“小键盘”","清空数据并确认以获得权限","清空数据并确认以取消权限","密码错误！请重新输入",
					 /*9*/ "恭喜！已获权限","已取消权限","格式错误！请重新输入","输入新密码，格式为“XXXX”","已输入密码","清空数据并确认以保存密码","已保存密码","771   打开按键蜂鸣器","772   关闭按键蜂鸣器","DG001  传感器故障",
					/*10*/ "DG001  传感器工作正常","DG002  传感器未知","初始化中","发生故障，请排查！","启动成功","完成","确认删除全部故障信息？","已删除全部故障信息！","31 探头组开关操作","312   打开探头组",
					/*11*/ "311   关闭探头组","探头已被关闭","       已熄灭","616 报警时间","616 报警时间 (单位：秒)","","","","","",
					   },
                       {
					 /*0*/ "Alarm","Begining","End","Nr：","Alarminfo","L001 Filter","DG001 Sparks 103","L002 Cyclone","Extinguishment start","Sensor open clear",
					 /*1*/ "Sensor closed made","Extinguishment end","Sensor closed clear","Fault","Flow switch signal too long","Get open switch fault","Faultinfo","The flow switch signal continues after","extinguishing.The flow switch signal","continues for other extinguishing systems",
					 /*2*/ "Disable","Cutoff detector group","Cutoff extinguishment group","System","1 Enter code","2 View stored data","3 Deal with disabling","4 Function test components","5 Deal with maintenance mode","6 Deal with installation parameters",	   
					 /*3*/ "7 Deal with system parameters","8 Disable system","Please click keyboard to enter","The format is“XXXX”","611 Counter threshold,counter sparks","612 Counter threshold,sustained counter sparks","613 Counter threshold,counter sparks[sec.]","614 Time threshold[sec.]","615 Extinguishment time[sec.]","The format is“X”,“XX”or“XXX”",
					 /*4*/ "Current","Change to","61 Parameters for detector group","62 Parameters for pump system","63 Parameters for frost protection","64 Parameters for system disable","7 Adjust system parameters","71 Change langusge","72 Change date","73 Change time",
					 /*5*/ "74 Change pass code","75 Delete data memory","76 Set display contrast","77 Set internal horn","78 Control console update","79 Control console configuration","Please click keyboard to enter","The format is“XX.XX.XX”","Current","Change to",
					 /*6*/ "Enter the old password","21 View data of alarm messages","22 View data of fault messages","23 View data of disable messages","History","751 Delete alarm history","752 Delete fault history","753 Delete disable history","Confirm to delete all alerts?","Press“x”to cancel“√”to confirm",
					 /*7*/ "All alerts deleted","Confirm to test components?","Testing...","Please wait on","Remain to be developed","Too much data","Please clean up immediately","611 Counter threshold,counter sparks","612 Counter threshold,sustained counter sparks","613 Counter threshold,counter sparks",
					 /*8*/ "614 Time threshold","615 Extinguishment time","Sparks    ","       Spraying","Sorry，you do not have access rights!","code right!","click keyboard again","clear and confirm to get access","clear and confirm to cancel access","code wrong ,retry",
					 /*9*/ "get permission","Cancel permission","wrong,retry","enter new code,format is“XXXX”","code entered","clear and confirm to save","password saved","771   Open the Buzzer","772   Close the Buzzer","DG001  Sensor fault",
					/*10*/ "DG001  Sensor works properly","DG002  Sensor unknown","","","","","","","","",
					/*11*/ "","","","","","","","","","",
					  }};
