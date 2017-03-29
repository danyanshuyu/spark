

#include "stm32f10x_lib.h"
#include "extern_variables.h"
#include <string.h>
#include "language.h"

u8  CodeBuffer[20];
u8	Code_Flag1 = 0;
u8  Code_Flag2 = 4;				   //使用权限等级,取值0/1/2/3/4
u8	Code_Flag3 = 0;
u8	Code_Flag4 = 0;
u8	Code_length = 0;
u8  Layer = 2;					   //图层计数
u8  Confirm_FlagStatus = 0;		   //确保确认键只能确认一次
u8  Menu_FlagStatus = 0;           //菜单键取值状态，取值0/1/2/3，对应4个菜单
int  System_FlagStatus = -1;	   //系统一级菜单的取值，0~7  0~799
u8  LR_FlagStatus = 0;             //左右键计数状态，按一次右键加1，按一次左键减1
u8  UD_FlagStatus = 0;             //上下键计数状态，按一次下键加1，按一次上键减1
u8  Button_Dn = 2;				   //警报界面时，上下键按一次更新显示另外2条信息
u16 xitong_Dn = 112;			   //系统选择图标初始Y轴位置
char JINGBAO_CUT[5],GUZHANG_CUT[5],GUANBI_CUT[5];			   //屏幕底部，警报个数、故障个数、关闭个数
char SECOND1[5],YEAR1[15],MONTH1[5],DATE1[5],HOUR1[15],MIN1[5];
char SECOND2[5],YEAR2[15],MONTH2[5],DATE2[5],HOUR2[15],MIN2[5];
char SPARK_NUM[5],SPARKS[50];			 
char EVENT_NR[5];
char TEMP_NUM[5];
char Display61[10];

void TextProcess();
void ButtonProcess();
void itoa();
void itoaD();
void itoaT();
void itoaF();
void Alarm_Display_A();
void Alarm_Display_B();
void Fault_Display_A();
void Disable_Display_A();

void ScreenDataRecv_Process()
{
	u16 Config_Control = 0;
	u8 	Control_Type = 0;
	//将数据拷贝到ScreenDataBuf
	MemCopy((u8*)&ScreenDataBuf,(u8*)&ScreenData,sizeof(ScreenData));
	ScreenData.Count = 0;
	ScreenData.Busy = 0;
	ScreenData.TranFinsh = 0;

	//处理数据
	if(ScreenDataBuf.TranFinsh==1 && ScreenData.RcevData[0]==0xEE)
	{
		//B111表示为组件控件指令
		Config_Control = (ScreenDataBuf.RcevData[1]<<8) | (ScreenDataBuf.RcevData[2]); 
		//表示控件为按钮控件或者文本控件				
		Control_Type = ScreenDataBuf.RcevData[7]; 
		
		if(Config_Control == 0xB111)//为读取组件控件指令
		{
			if(Control_Type == 0x10)//控件类型为按钮控件
			{
				ButtonProcess();
			}
			else if(Control_Type == 0x11)//控件类型为文本控件
			{
				TextProcess();
			}
		}
	}
	ScreenDataBuf.TranFinsh = 0;//处理数据完成	
}


void TextProcess()
{
	char Txt[50],code[20];
	u16 ScreenID = 0;//画面ID
	u16 ControlID = 0;//控件ID
	u8 	Subtype = 0;//表示开关按钮还是切换画面按钮
	u8  Status = 0;//按钮状态
	u8  i = 8,j = 0,month = 0,day = 0,hour = 0,min = 0,second = 0, year = 0;
    u16	sum = 0;
	u16  Txt_temp[10];

	ScreenID = (ScreenDataBuf.RcevData[3]<<8) | (ScreenDataBuf.RcevData[4]);
	ControlID = (ScreenDataBuf.RcevData[5]<<8) | (ScreenDataBuf.RcevData[6]);
			
	do
    {
        Txt[j] = ScreenDataBuf.RcevData[i];       
        i++;
		j++;
    }while(i<(ScreenDataBuf.Count-5));
	Txt[j] = '\0';

	if(ScreenID == 1 && ControlID == 32 && System_FlagStatus == 0)		 //	  01 Enter code
	{
	     Code_length = _24C08_I2C_ByteRead(32);
		 Delay_ms(0x58);
		 _24C08_I2C_BufferRead(code,16,Code_length);
		 code[Code_length] = '\0';

		 WriteLayer(3-Layer);
	     switch(Code_Flag1)
		 {
		  case 0:
				 if(strcmp(code,Txt) == 0)
				 {
				      SetFcolor(0xFFFF);
					  GUI_RectangleFill(181,229,479,287);
					  GUI_RectangleFill(200,300,500,360);
				      SetFcolor(0);
					  DisText(200,300,0,6,Content[Lang_Flag][85]);
		              DisText(200,240,0,5,Content[Lang_Flag][86]);
					  if(Code_Flag4 == 0)
					  {
					       DisText(200,270,0,5,Content[Lang_Flag][87]);
					  }
					  if(Code_Flag4 == 1)
					  {
					       DisText(200,270,0,5,Content[Lang_Flag][88]);
					  }
					  Code_Flag1 = 1;
				 }
				 else
				 {
					  SetFcolor(0xFFFF);
					  GUI_RectangleFill(181,229,479,287);
					  GUI_RectangleFill(200,300,500,360);
				      SetFcolor(0);
					  DisText(200,230,0,6,Content[Lang_Flag][89]);
					  Code_Flag1 = 0;
				 }
				 break;
		  case 1:
				 if(Txt[1] == '\0' )
				 {
					  SetFcolor(0xFFFF);
					  GUI_RectangleFill(181,229,479,287);
					  GUI_RectangleFill(200,300,500,360);
					  GUI_RectangleFill(501,0,619,72);	   
				      SetFcolor(0);
		
					  switch(Code_Flag4)
					  {			
						  case 0:DisText(200,230,0,6,Content[Lang_Flag][90]);
								 DisArea_Image(534,12,0,0);
							     Code_Flag4 = 1;
							     Code_Flag2 = 4;
								 break;
						  case 1:DisText(200,230,0,6,Content[Lang_Flag][91]);
						         DisArea_Image(534,12,1,0);					  
							     Code_Flag4 = 0;
							     Code_Flag2 = 0;
							     break;		
					     default:break;
					  }
					  Code_Flag1 = 0;
				 }
				 break;
		 default:break;
		 }
		 DisplyLayer(3-Layer);					   
	     CopyLayer(3-Layer,Layer); 
	}

	if(ScreenID == 1 && ControlID == 32 && System_FlagStatus == 61)		 //	  72 Change date
	{
	     year = (Txt[0]-48)*10+Txt[1]-48;
	  	 month = (Txt[3]-48)*10+Txt[4]-48;
		 day = (Txt[6]-48)*10+Txt[7]-48;
	     if(Txt[0] != '.' && Txt[1] != '.' && Txt[2] == '.' && Txt[3] != '.' && Txt[4] != '.' && Txt[5] == '.' && Txt[6] != '.' && Txt[7] != '.' && Txt[8] == '\0' && year >= 0 && hour < 100 && month > 0 && month < 13 && day > 0 && day < 32)
		 {
			  Set_Day_Time(year,month,day);
			  RTCInitFlagStatus = 1;
			  RTC_Display();
			  SetFcolor(0xFFFF);
			  GUI_RectangleFill(181,229,479,287);
		      SetFcolor(0);
			  DisText(200,230,0,9,Txt);
		 }
	     else
		 {
			  SetFcolor(0xFFFF);
			  GUI_RectangleFill(181,229,479,287);
		      SetFcolor(0);
			  DisText(200,230,0,6,Content[Lang_Flag][92]);
		 }
	}
	if(ScreenID == 1 && ControlID == 32 && System_FlagStatus == 62)		 //	  73 Change date
	{	  
	     hour = (Txt[0]-48)*10+Txt[1]-48;
	     min = (Txt[3]-48)*10+Txt[4]-48;
	     second = (Txt[6]-48)*10+Txt[7]-48;
		 if(Txt[0] != '.' && Txt[1] != '.' && Txt[2] == '.' && Txt[3] != '.' && Txt[4] != '.' && Txt[5] == '.' && Txt[6] != '.' && Txt[7] != '.' && Txt[8] == '\0' && hour >= 0 && hour < 24 && min >= 0 && min < 60 && second >= 0 && second < 60)
		 {
		      Txt[2] = ':';
			  Txt[5] = ':';
		  	  Set_Second_Time(hour,min,second);
			  RTC_Display();
			  SetFcolor(0xFFFF);
			  GUI_RectangleFill(181,229,479,287);
		      SetFcolor(0);
			  DisText(200,230,0,9,Txt);
		 }
		 else
		 {
			  SetFcolor(0xFFFF);
			  GUI_RectangleFill(181,229,479,287);
		      SetFcolor(0);
			  DisText(200,230,0,6,Content[Lang_Flag][92]);
		 }
	}
	if(ScreenID == 1 && ControlID == 32 && System_FlagStatus == 63)		 //	  74 Change pass code
	{

	     switch(Code_Flag3)
		 {
		     case 0:Code_length = _24C08_I2C_ByteRead(32);
		            Delay_ms(0x58);
			        _24C08_I2C_BufferRead(code,16,Code_length);
		            code[Code_length] = '\0';
					if(strcmp(code,Txt) == 0)
				    {
				         SetFcolor(0xFFFF);
					     GUI_RectangleFill(181,229,479,287);
					     GUI_RectangleFill(200,300,500,360);
				         SetFcolor(0);
					     DisText(200,300,0,6,Content[Lang_Flag][85]);
		                 DisText(200,240,0,5,Content[Lang_Flag][86]);
					     DisText(200,270,0,5,Content[Lang_Flag][93]);
					     Code_Flag3 = 1;
					}
					else
					{
						 SetFcolor(0xFFFF);
						 GUI_RectangleFill(181,229,479,287);
						 GUI_RectangleFill(200,300,500,360);
					     SetFcolor(0);
						 DisText(200,230,0,6,Content[Lang_Flag][89]);
						 Code_Flag3 = 0;
					}
					break;
		     case 1:memset(CodeBuffer,'\0', sizeof(CodeBuffer));			  //清空CodeBuffer字符串
			        strcpy(CodeBuffer,Txt);
				    SetFcolor(0xFFFF);
				    GUI_RectangleFill(181,229,479,287);
				    GUI_RectangleFill(200,300,500,360);
			        SetFcolor(0);
				    DisText(200,300,0,6,Content[Lang_Flag][94]);
				    DisText(200,240,0,5,Content[Lang_Flag][86]);
				    DisText(200,270,0,5,Content[Lang_Flag][95]);
					Code_Flag3 = 2;
					break;		
			 case 2:
				    if(Txt[0] == '\0' )
				    {
					     Code_length = strlen(CodeBuffer);	                   // 不包括结束字符"\0"
					     _24C08_I2C_ByteWrite(Code_length,32);
						 Delay_ms(0x58);
				         _24C08_I2C_PageWrite(CodeBuffer,16,Code_length);
					     SetFcolor(0xFFFF);
					     GUI_RectangleFill(181,229,479,287);
					     GUI_RectangleFill(200,300,500,360);
				         SetFcolor(0);
					     DisText(200,230,0,6,Content[Lang_Flag][96]);
						 memset(CodeBuffer,'\0', sizeof(CodeBuffer));		  //清空CodeBuffer字符串
						 Code_Flag3 = 0;
				    }
					break;
		    default:break;
		}
		    
	}
	if(ScreenID == 1 && ControlID == 32 && System_FlagStatus >= 500 && System_FlagStatus <= 504)		 //	  611 火花极限，单次火花数
	{																									 //	  612 火花极限，持续火花数
	 u8	m = 0,k = 0,n = 0;																				 //	  613 火花极限，火花时间
		k = j;																							 //	  614 时间极限
		if(Txt[0] == '\0')																				 //	  615 熄灭时间
		sum = 0;																						 
		else
		{ 																							 
			for(m = 0;m < j;m++)		//小键盘输入字符串转换成数值									
			{																							
			    k--;																					
				n = k;
				Txt[m] = Txt[m]-48;
				Txt_temp[m] = Txt[m];
				for(n;n>0;n--)
				{
				   Txt_temp[m] = Txt_temp[m] * 10;    
				}
				sum += Txt_temp[m];
			}
		}
		SetFcolor(0xFFFF);
		GUI_RectangleFill(181,229,479,287);
		SetFcolor(0);
		switch (System_FlagStatus)
		{
			case 500:Spark_DANGER_A = sum ;
					 itoaT(Spark_DANGER_A,Display61);
					 DisText(280,230,0,9,Display61);
					 _24C08_I2C_HalfwordWrite(Spark_DANGER_A,2);
				     break;
			case 501:Spark_DANGER_B = sum ;
					 itoaT(Spark_DANGER_B,Display61);
					 DisText(280,230,0,9,Display61);
					 _24C08_I2C_HalfwordWrite(Spark_DANGER_B,4);
				     break;
			case 502:Spark_DANGER_B_M = sum ;
					 itoaT(Spark_DANGER_B_M,Display61);
					 DisText(280,230,0,9,Display61);
					 _24C08_I2C_HalfwordWrite(10*Spark_DANGER_B_M,6);
				     break;
			case 503:Spark_DANGER_C_N = sum ;
					 itoaT(Spark_DANGER_C_N,Display61);
					 DisText(280,230,0,9,Display61);
					 _24C08_I2C_HalfwordWrite(10*Spark_DANGER_C_N,8);
				     break;
            case 504:Spray_TIME = sum ;
					 itoaT(Spray_TIME,Display61);
					 DisText(280,230,0,9,Display61);
					 _24C08_I2C_HalfwordWrite(Spray_TIME,10);
				     break;
			 default:break;
		}																								 
	}

}

void ButtonProcess()
{
	u16 ScreenID = 0;           		//画面ID
	u16 ControlID = 0;          		//控件ID
	u8 	Subtype = 0;            		//表示开关按钮还是切换画面按钮
	u8  Status = 0;             		//按钮状态
	u16 Flag = 0;               		//按钮判断
	u16 temp_num = 0;           		//临时数据
	u8  cursor_Yn = 0;          		//选择光标上下移动范围

	ScreenID = (ScreenDataBuf.RcevData[3]<<8) | (ScreenDataBuf.RcevData[4]);
	ControlID = (ScreenDataBuf.RcevData[5]<<8) | (ScreenDataBuf.RcevData[6]);

	if(ScreenDataBuf.Count == 14)//接收到为14个字节，表示按下的是开关按钮
	{
		Subtype = ScreenDataBuf.RcevData[8];
		Status	= ScreenDataBuf.RcevData[9];
		if(Subtype == 0x01)//表示按下的是开关按钮，对画面切换按钮不处理
		{
			Code_Flag1 = 0;				 //	 如在输入密码过程中退出，相应标志位复位
			Code_Flag3 = 0;				 //

			if(ScreenID == 1 && ControlID == 37 && Status == 0)                //声光报警复位按钮，由按下变为弹起                
			Alarm_reset_Flag = 0;														   
			if(ScreenID == 1 && ControlID == 37 && Status == 1)                //声光报警复位按钮，由弹起变为按下                
			Alarm_reset_Flag = 1;														   


			if(ScreenID == 1 && ControlID == 5)                                
			Flag = 1;														   //如果画面1的第5个按钮按下,报警
			if(Alarm_num > 0 && ScreenID == 1 && ControlID == 17 && Menu_FlagStatus == 0 && LR_FlagStatus == 0)	
			Flag = 2;														   //警报界面时的右键功能
			if(ScreenID == 1 && ControlID == 18 && Menu_FlagStatus == 0 && LR_FlagStatus == 1)  
			Flag = 3;														   //警报->右键->左键
			if(ScreenID == 1 && ControlID == 24 && Menu_FlagStatus == 0 && LR_FlagStatus == 0)			
			Flag = 4;														   //警报界面时，下键的功能
 			if(ScreenID == 1 && ControlID == 19 && Menu_FlagStatus == 0 && UD_FlagStatus > 0 && LR_FlagStatus == 0 )			 
			Flag = 5;														   //警报界面时，上键的功能		     			
			if(ScreenID == 1 && ControlID == 10)                                                
			Flag = 6;														   //如果画面1的第10个按钮按下，故障
			if(ScreenID == 1 && ControlID == 17 && Menu_FlagStatus == 1 && LR_FlagStatus == 0 && Fault_num != 0)	
			Flag = 7;														   //故障界面时的右键功能
			if(ScreenID == 1 && ControlID == 18 && Menu_FlagStatus == 1 && LR_FlagStatus == 1)  
			Flag = 8;														   //故障->右键->左键
			if(ScreenID == 1 && ControlID == 16)                                                
			Flag = 9;														   //如果画面1的第16个按钮按下，关闭
			if(ScreenID == 1 && ControlID == 17 && Menu_FlagStatus == 2)		                
			Flag = 10;														   //关闭界面时的右键功能
			if((ScreenID == 1 && ControlID == 11) || (ScreenID == 1 && (ControlID == 18 || ControlID == 21) && Menu_FlagStatus == 3 && LR_FlagStatus == 1) )        
			Flag = 11;														   //如果画面1的第11个按钮按下，系统
												                               //系统-->右键-->左键			
			if(ScreenID == 1 && ControlID == 24 && Menu_FlagStatus == 3)		               
			Flag = 12;														   //系统界面时的下键功能
			if(ScreenID == 1 && ControlID == 19 && Menu_FlagStatus == 3)		                
			Flag = 13;														   //系统界面时的上键功能
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus == 0 && LR_FlagStatus == 0)		 
			Flag = 14;                                                         //系统界面时的右键功能---1输入密码
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 50 && LR_FlagStatus == 2 && UD_FlagStatus >= 0 && UD_FlagStatus <= 4 )
			Flag = 15;														   //61探头组参数的右键功能--611 火花极限，单次火花数
																			   //61探头组参数的右键功能--612 火花极限，持续火花数
																		       //61探头组参数的右键功能--613 火花极限，火花时间		
																			   //61探头组参数的右键功能--614 时间极限
																			   //61探头组参数的右键功能--615 熄灭时间
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 5 && LR_FlagStatus == 1 && UD_FlagStatus == 0  || (ScreenID == 1 && (ControlID == 18 || ControlID == 21) && System_FlagStatus >= 500 && System_FlagStatus <= 504 && LR_FlagStatus == 3))
			Flag = 19;														   //6安装参数设置界面的右键功能--61 探头组参数
																			   //61 探头组参数-->右键-->左键
			if(Code_Flag2 > 0 && ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus == 5 && LR_FlagStatus == 0 || (ScreenID == 1 && (ControlID == 18 || ControlID == 21) && System_FlagStatus >= 50 && System_FlagStatus <= 53 && LR_FlagStatus == 2))
			Flag = 20;														   //系统界面时的右键功能---6安装参数
																			   //6安装参数设置-->右键-->左键
			if(Code_Flag2 > 0 && ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus == 6 && LR_FlagStatus == 0 || (ScreenID == 1 && (ControlID == 18 || ControlID == 21) && System_FlagStatus >= 60 && System_FlagStatus <= 68 && LR_FlagStatus == 2))		 
			Flag = 21;                                                         //系统界面时的右键功能---7系统参数设置
			    										                       //7系统参数设置-->右键-->左键
		    if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 6 && LR_FlagStatus == 1 && UD_FlagStatus == 0 )
			Flag = 37;														   //7系统参数设置界面的右键功能--71 Change language 
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 6 && LR_FlagStatus == 1 && UD_FlagStatus == 1 )
			Flag = 22;														   //7系统参数设置界面的右键功能--72 Change date 
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 6 && LR_FlagStatus == 1 && UD_FlagStatus == 2 )
			Flag = 23;														   //7系统参数设置界面的右键功能--73 Change time
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 6 && LR_FlagStatus == 1 && UD_FlagStatus == 3 )
			Flag = 24;														   //7系统参数设置界面的右键功能--74 Change pass code			
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus == 1 && LR_FlagStatus == 0 || (ScreenID == 1 && (ControlID == 18 || ControlID == 21) && System_FlagStatus >= 10 && System_FlagStatus <= 12 && LR_FlagStatus == 2))
			Flag = 25;														   //系统界面时的右键功能---2查看储存信息
																			   //2查看储存信息-->右键-->左键
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 1 && LR_FlagStatus == 1 && UD_FlagStatus == 0 )
			Flag = 26;														   //2查看储存信息界面的右键功能--21 查看警报信息 
			if(ScreenID == 1 && ControlID == 24 && Menu_FlagStatus == 3 && System_FlagStatus == 10 && LR_FlagStatus == 2)	   		
			Flag = 27;														   //21查看警报历史信息界面时，下键的功能
			if(ScreenID == 1 && ControlID == 19 && Menu_FlagStatus == 3 && System_FlagStatus == 10 && UD_FlagStatus > 0 && LR_FlagStatus == 2 )			 
			Flag = 28;														   //21查看警报历史信息界面时，上键的功能
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 6 && LR_FlagStatus == 1 && UD_FlagStatus == 4  || (ScreenID == 1 && (ControlID == 18 || ControlID == 21) && System_FlagStatus >= 640 && System_FlagStatus <= 642 && LR_FlagStatus == 3))
			Flag = 29;														   //7系统参数设置界面的右键功能--75 删除历史信息
																			   //75 删除历史信息-->右键-->左键
		    if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 64 && LR_FlagStatus == 2 && UD_FlagStatus == 0 )
			Flag = 30;														   //751 删除警报历史信息
			if(ScreenID == 1 && ControlID == 20 && Menu_FlagStatus == 3 && System_FlagStatus == 640 && Confirm_FlagStatus == 0)		 
			Flag = 31;                                                         //751 删除警报历史信息--确认删除历史信息																
			if(Code_Flag2 > 0 && ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus == 3 && LR_FlagStatus == 0)		 
			Flag = 32;                                                         //4测试元件
			if(ScreenID == 1 && ControlID == 20 && Menu_FlagStatus == 3 && System_FlagStatus == 3 && Confirm_FlagStatus == 0)		 
			Flag = 33;                                                         //4测试元件--确认
			if(Code_Flag2 > 0 && ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus == 2 && LR_FlagStatus == 0)		 
			Flag = 34;                                                         //系统界面时的右键功能---3关闭功能
			if(Code_Flag2 > 0 && ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus == 4 && LR_FlagStatus == 0)		 
			Flag = 35;                                                         //系统界面时的右键功能---5维护模式
			if(Code_Flag2 > 0 && ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus == 7 && LR_FlagStatus == 0)		 
			Flag = 36;                                                         //系统界面时的右键功能---8关闭系统
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 60 && UD_FlagStatus == 0 )
			Flag = 38;														   //71 Change language--711 Language English
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 60 && UD_FlagStatus == 1 )
			Flag = 39;														   //71 Change language--712 中文  
			if(Code_Flag2 < 4 && ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus > 1 && LR_FlagStatus == 0)
			Flag = 40;														   //没有权限，禁止访问高级功能
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 6 && LR_FlagStatus == 1 && UD_FlagStatus == 6 )
			Flag = 41;														   //7系统参数设置界面的右键功能--77 开关内部喇叭
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 66 && UD_FlagStatus == 0 )
			Flag = 42;														   //77 开关内部喇叭--771 打开按键蜂鸣器
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 66 && UD_FlagStatus == 1 )
			Flag = 43;														   //77 开关内部喇叭--772 关闭按键蜂鸣器
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 1 && LR_FlagStatus == 1 && UD_FlagStatus == 1 )
			Flag = 44;														   //2查看储存信息界面的右键功能--22 查看故障信息 
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 64 && LR_FlagStatus == 2 && UD_FlagStatus == 1 )
			Flag = 45;														   //752 删除故障历史信息
			if(ScreenID == 1 && ControlID == 20 && Menu_FlagStatus == 3 && System_FlagStatus == 641 && Confirm_FlagStatus == 0)		 
			Flag = 46;                                                         //752 删除故障历史信息--确认删除历史信息
			if(Code_Flag2 > 0 && ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && UD_FlagStatus == 2 && LR_FlagStatus == 0 || (ScreenID == 1 && (ControlID == 18 || ControlID == 21) && System_FlagStatus >= 20 && System_FlagStatus <= 25 && LR_FlagStatus == 2))
			Flag = 47;														   //系统界面时的右键功能---3关闭操作
																			   //3关闭操作-->右键-->左键
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 2 && LR_FlagStatus == 1 && UD_FlagStatus == 0 )
			Flag = 48;														   //3关闭操作右键功能--31 探头组开关操作	
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 20 && UD_FlagStatus == 1 )
			Flag = 49;														   //31 探头组开关操作--311 打开探头组
			if(ScreenID == 1 && (ControlID == 17 || ControlID == 20) && Menu_FlagStatus == 3 && System_FlagStatus == 20 && UD_FlagStatus == 0 )
			Flag = 50;														   //31 探头组开关操作--312 关闭探头组


			switch(Flag)
			{
			    case 0:	   break;		                                       //未定义的判断条件，不做任何操作
	        	case 1:                                                        //如果画面1的第5个按钮按下,报警
				    Menu_FlagStatus = 0;
					LR_FlagStatus = 0;
					Button_Dn = 2;
					UD_FlagStatus = 0;
					
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(135,16,22,0);		
					DisText(271,23,0,7,Content[Lang_Flag][0]);
					DisText(15,85,0,3,Content[Lang_Flag][1]);
					DisText(340,85,0,3,Content[Lang_Flag][2]);
					DisText(470,85,0,3,Content[Lang_Flag][3]);
	
					if(Alarm_num>=Button_Dn)
				    {
				       Alarm_Display_A(Alarm_num);
				       Alarm_Display_B(Alarm_num-1);
					   GUI_Rectangle(465,115,600,175);
				    }
				    else if(Alarm_num==1)
				    {
					   Alarm_Display_A(1);
					   GUI_Rectangle(465,115,600,175);
				    }
				    else if(Alarm_num==0)
				    {
				        ;
				    }
					DisplyLayer(Layer);
					Layer=3-Layer;
							
					break;
				case 2:	                                                       //警报界面时的右键功能
				{
					LR_FlagStatus++;
	
	                Data_Get_Once(Alarm_num-UD_FlagStatus);
					WriteLayer(Layer);
				    SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					SetFcolor(0);
					DisArea_Image(135,16,22,0);		
					DisText(250,23,0,7,Content[Lang_Flag][4]);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);		
					DisText(15,85,0,3,Content[Lang_Flag][5]);
					itoaT(Get_Info.spark_num,SPARK_NUM);
			        strcat(strcat(strcat(SPARKS,"DG001    "),Content[Lang_Flag][82]),SPARK_NUM);
					DisText(15,115,0,2,SPARKS);
			        memset(SPARKS,'\0', sizeof(SPARKS));			                   //清零SPARKS字符串
					DisText(15,145,0,3,Content[Lang_Flag][7]);
					DisText(15,175,0,2,"EG002");
					DisText(15,205,0,2,"17.01.16");
					DisText(120,205,0,2,"16:28:41:039");
					DisText(280,205,0,2,Content[Lang_Flag][8]);
					DisText(120,235,0,2,"16:28:41:339");
					DisText(280,235,0,2,Content[Lang_Flag][9]);
					DisText(120,265,0,2,"16:28:41:511");
					DisText(280,265,0,2,Content[Lang_Flag][10]);
					DisText(120,295,0,2,"16:28:46:039");
					DisText(280,295,0,2,Content[Lang_Flag][11]);
					DisText(120,325,0,2,"16:28:46:277");
					DisText(280,325,0,2,Content[Lang_Flag][12]);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}	break;
				case 3:                                                        //警报->右键->左键
				{
				        Menu_FlagStatus = 0;
	                    LR_FlagStatus--;
						 
					    WriteLayer(Layer);
						SetFcolor(0xFFFF);
						GUI_RectangleFill(131,1,499,72);
						GUI_RectangleFill(0,74,619,380);
						SetFcolor(0);
						DisArea_Image(135,16,22,0);		
						DisText(271,23,0,7,Content[Lang_Flag][0]);
						DisText(15,85,0,3,Content[Lang_Flag][1]);
						DisText(340,85,0,3,Content[Lang_Flag][2]);
						DisText(470,85,0,3,Content[Lang_Flag][3]);
	
					    if(Alarm_num>=Button_Dn)
			            {
			                Alarm_Display_A(Alarm_num-Button_Dn+2);			   //Alarm_num-(Button_Dn-2)
			     		    Alarm_Display_B(Alarm_num-Button_Dn+1);			   //Alarm_num-(Button_Dn-2)-1
				        }			        
						else if(Alarm_num<Button_Dn)
			     	    {
				       	    Alarm_Display_A(Alarm_num-Button_Dn+2);			   //Alarm_num-(Button_Dn-2)
			     	    }
						if(Alarm_num != 0)
						{
							if (UD_FlagStatus%2 == 0)
							{
								GUI_Rectangle(465,115,600,175);                //在A处显示选择框
							}
							else
							{
							    GUI_Rectangle(465,235,600,295);                //在B处显示选择框
							}
						}
						DisplyLayer(Layer);
					    Layer=3-Layer;
				}	break;
				case 4:			
				{															   //警报界面时，下键的功能
				    if(Alarm_num == 2)
					{
					    if (UD_FlagStatus == 0)
						{
						    UD_FlagStatus++;
							WriteLayer(Layer);
							SetFcolor(0xFFFF);
							GUI_RectangleFill(131,1,499,72);
							GUI_RectangleFill(0,74,619,380);
							SetFcolor(0);
							DisArea_Image(135,16,22,0);		
							DisText(271,23,0,7,Content[Lang_Flag][0]);
							DisText(15,85,0,3,Content[Lang_Flag][1]);
							DisText(340,85,0,3,Content[Lang_Flag][2]);
							DisText(470,85,0,3,Content[Lang_Flag][3]);
							Alarm_Display_A(2);			 
				     		Alarm_Display_B(1);
							GUI_Rectangle(465,235,600,295);
							DisplyLayer(Layer);
						    Layer=3-Layer;
						}										
					}
					else
					{
					    if(Alarm_num>(UD_FlagStatus+1))	 
					    {   
						    UD_FlagStatus++;   
						    				                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	                        if (UD_FlagStatus%2 == 0)
							{
							    Button_Dn+=2;
							}                                                                                                                                                                                                                                                                                         
							WriteLayer(Layer);
							SetFcolor(0xFFFF);
							GUI_RectangleFill(131,1,499,72);
							GUI_RectangleFill(0,74,619,380);
							SetFcolor(0);
							DisArea_Image(135,16,22,0);		
							DisText(271,23,0,7,Content[Lang_Flag][0]);
							DisText(15,85,0,3,Content[Lang_Flag][1]);
							DisText(340,85,0,3,Content[Lang_Flag][2]);
							DisText(470,85,0,3,Content[Lang_Flag][3]);
		
						    if(Alarm_num>=Button_Dn)
				            {
				                Alarm_Display_A(Alarm_num-Button_Dn+2);		   //Alarm_num-(Button_Dn-2)
				     		    Alarm_Display_B(Alarm_num-Button_Dn+1);		   //Alarm_num-(Button_Dn-2)-1
					        }			        
							else if(Alarm_num<Button_Dn)
				     	    {
					       	    Alarm_Display_A(Alarm_num-Button_Dn+2);		   //Alarm_num-(Button_Dn-2)
				     	    }
							if (UD_FlagStatus%2 == 0)
							{
								GUI_Rectangle(465,115,600,175);                //在A处显示选择框
							}
							else
							{
							    GUI_Rectangle(465,235,600,295);                //在B处显示选择框
							}
							DisplyLayer(Layer);
						    Layer=3-Layer;
						}
						else
						{
						   ;
						}
					}
					
				}	break;
	 			case 5:			 
				{															   //警报界面时，上键的功能
			     	UD_FlagStatus--;
				    if (UD_FlagStatus%2 != 0)
					{
						Button_Dn-=2;
					}
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(135,16,22,0);		
					DisText(271,23,0,7,Content[Lang_Flag][0]);
					DisText(15,85,0,3,Content[Lang_Flag][1]);
					DisText(340,85,0,3,Content[Lang_Flag][2]);
					DisText(470,85,0,3,Content[Lang_Flag][3]);
					Alarm_Display_A(Alarm_num-Button_Dn+2);				       //Alarm_num-(Button_Dn-2)
			        Alarm_Display_B(Alarm_num-Button_Dn+1);				       //Alarm_num-(Button_Dn-2)-1
					if (UD_FlagStatus%2 != 0)
					{
						GUI_Rectangle(465,235,600,295);                        //在B处显示选择框
					}
					else
					{
						GUI_Rectangle(465,115,600,175);                        //在A处显示选择框
					}
					DisplyLayer(Layer);
					Layer=3-Layer;
				}	break;
	 			case 6:	                                                       //如果画面1的第10个按钮按下，故障
				{
			    	Menu_FlagStatus = 1;
					LR_FlagStatus = 0;
	
					WriteLayer(Layer);
				    SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,19,0);		
					DisText(271,23,0,7,Content[Lang_Flag][13]);
					DisText(15,85,0,3,Content[Lang_Flag][1]);
					DisText(340,85,0,3,Content[Lang_Flag][2]);
					DisText(470,85,0,3,Content[Lang_Flag][3]);
					Fault_Display_A(Fault_num);
					DisplyLayer(Layer);
					Layer=3-Layer;					  
				}   break;
	 			case 7:	                                                       //故障界面时的右键功能
				{
				    LR_FlagStatus++;
	
					WriteLayer(Layer);
				    SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					SetFcolor(0);
					DisArea_Image(143,23,19,0);		
					DisText(250,23,0,7,Content[Lang_Flag][16]);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);		
					DisText(15,85,0,3,Content[Lang_Flag][14]);
					DisText(65,130,0,2,Content[Lang_Flag][17]);
					DisText(65,160,0,2,Content[Lang_Flag][18]);
					DisText(65,190,0,2,Content[Lang_Flag][19]);
					DisplyLayer(Layer);
					Layer=3-Layer;				
				}   break;
	 			case 8:		                                                   //故障->右键->左键
				{
					Menu_FlagStatus = 1;
					LR_FlagStatus--;
	
					WriteLayer(Layer);
				    SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,19,0);		
					DisText(271,23,0,7,Content[Lang_Flag][13]);
					DisText(15,85,0,3,Content[Lang_Flag][1]);
					DisText(340,85,0,3,Content[Lang_Flag][2]);
					DisText(470,85,0,3,Content[Lang_Flag][3]);
					Fault_Display_A(Fault_num);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
	 			case 9:                                                        //如果画面1的第16个按钮按下，关闭
				{
					Menu_FlagStatus = 2;
	
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,20,0);		
					DisText(271,23,0,7,Content[Lang_Flag][20]);
					DisText(15,85,0,3,Content[Lang_Flag][1]);
					DisText(340,85,0,3,Content[Lang_Flag][2]);
					DisText(470,85,0,3,Content[Lang_Flag][3]);
					Disable_Display_A(Disable_num);
					DisplyLayer(Layer);
					Layer=3-Layer;				 
				}   break;
	 			case 10: 		                                               //关闭界面时的右键功能
				{
	
				}   break;
	 			case 11:         
				{															   //如果画面1的第11个按钮按下，系统
					Menu_FlagStatus = 3;									   //系统-->右键-->左键
					UD_FlagStatus = 0;
					LR_FlagStatus = 0;
					System_FlagStatus = -1; 
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					GUI_RectangleFill(10,120,44,350);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(45,120,0,2,Content[Lang_Flag][24]);
					DisText(45,145,0,2,Content[Lang_Flag][25]);
					DisText(45,170,0,2,Content[Lang_Flag][26]);
					DisText(45,195,0,2,Content[Lang_Flag][27]);
					DisText(45,220,0,2,Content[Lang_Flag][28]);
					DisText(45,245,0,2,Content[Lang_Flag][29]);
					DisText(45,270,0,2,Content[Lang_Flag][30]);
					DisText(45,295,0,2,Content[Lang_Flag][31]);				
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);	
					DisplyLayer(Layer);
					Layer=3-Layer;		 
				}   break;
	 			case 12: 		                                               //系统界面时的下键功能
				{

					if(LR_FlagStatus == 0)									   //主菜单下键        8个
					cursor_Yn = 8;
					if(System_FlagStatus ==6)		                   		   //7的下键菜单  	   9个
					cursor_Yn = 9;
					if(System_FlagStatus == 1 || System_FlagStatus == 64)	   //2、75的下键菜单   3个
					cursor_Yn = 3;
					if(System_FlagStatus == 5 )			                       //6的下键菜单	   4个
					cursor_Yn = 4;
					if(System_FlagStatus == 50)								   //61的下键菜单	   5个
					cursor_Yn = 5;
					if(System_FlagStatus == 60 || System_FlagStatus == 66 || System_FlagStatus == 20)	   //71、77/31的下键菜单  2个
					cursor_Yn = 2;
					if(cursor_Yn != 0)
					{	
					    SetFcolor(0xFFFF);
						GUI_RectangleFill(10,120,44,350);
						if(xitong_Dn<(100+25*cursor_Yn))
						{
							xitong_Dn+=25;
							UD_FlagStatus += 1;
						}
						else
						{
							xitong_Dn=125;
							UD_FlagStatus = 0;
						}
						DisArea_Image(20,xitong_Dn,2,0);
					}
						
				}   break;
	 			case 13: 		                                               //系统界面时的上键功能
				{				
					if(LR_FlagStatus == 0)									   //主菜单上键     8个
					cursor_Yn = 8;
					if(System_FlagStatus ==6)					 			   //7的上键菜单	9个
					cursor_Yn = 9;
					if(System_FlagStatus ==1 || System_FlagStatus == 64)	   //2的上键菜单	3个
					cursor_Yn = 3;
					if(System_FlagStatus == 5) 				 				   //6的上键菜单	4个
					cursor_Yn = 4;
					if(System_FlagStatus == 50)								   //61的上键菜单	5个
					cursor_Yn = 5;
					if(System_FlagStatus == 60 || System_FlagStatus == 66 || System_FlagStatus == 20 )     //71、77、31的上键菜单	2个
					cursor_Yn = 2;
					if(cursor_Yn != 0)
					{
					    SetFcolor(0xFFFF);
						GUI_RectangleFill(10,120,44,350);
						if(xitong_Dn>125)
						{
							xitong_Dn-=25;
							UD_FlagStatus -= 1;
						}
						else
						{
							xitong_Dn=(100+25*cursor_Yn);
							UD_FlagStatus = (cursor_Yn-1);
						}
						DisArea_Image(20,xitong_Dn,2,0);
					}
				
				}   break;
	 			case 14: 		 
				{                                                              //1输入密码
				    LR_FlagStatus++;
					System_FlagStatus = 0;
				    WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][24]);
					GUI_Rectangle(180,168,480,288);
				    GUI_Line(180,228,480,228);
					DisText(200,170,0,9,"********");
					DisText(200,240,0,5,Content[Lang_Flag][32]);
					DisText(200,270,0,5,Content[Lang_Flag][33]);
					SetFcolor(0x4A69);		   //灰色
					GUI_Rectangle(179,167,481,289);
					GUI_Rectangle(178,166,482,290);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
	 			case 15: 
				{															   //611 火花极限，单次火花数
																			   //612 火花极限，持续火花数
					LR_FlagStatus++;										   //613 火花极限，火花时间
																			   //614 时间极限
					WriteLayer(Layer);										   //615 熄灭时间
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					switch(UD_FlagStatus)
					{
					    case 0:temp_num = _24C08_I2C_HalfwordRead(2);
						       System_FlagStatus = 500;					
							   DisText(5,80,0,2,Content[Lang_Flag][34]);
							   break;
					    case 1:temp_num = _24C08_I2C_HalfwordRead(4);
						       System_FlagStatus = 501;					
							   DisText(5,80,0,2,Content[Lang_Flag][35]);
							   break;
						case 2:temp_num = _24C08_I2C_HalfwordRead(6)/10;
						       System_FlagStatus = 502;					
							   DisText(5,80,0,2,Content[Lang_Flag][36]);
							   break;
						case 3:temp_num = _24C08_I2C_HalfwordRead(8)/10;
						       System_FlagStatus = 503;					
							   DisText(5,80,0,2,Content[Lang_Flag][37]);
							   break;
						case 4:temp_num = _24C08_I2C_HalfwordRead(10);
						       System_FlagStatus = 504;					
							   DisText(5,80,0,2,Content[Lang_Flag][38]);
							   break;
					   default:break;
					}
					UD_FlagStatus = 0;
					GUI_Rectangle(180,168,480,288);
				    GUI_Line(180,228,480,228);
					itoaT(temp_num,TEMP_NUM);
					DisText(280,170,0,9,TEMP_NUM);
					DisText(200,240,0,5,Content[Lang_Flag][32]);
					DisText(200,270,0,5,Content[Lang_Flag][39]);
					DisText(20,180,0,7,Content[Lang_Flag][40]);
					DisText(20,240,0,7,Content[Lang_Flag][41]);
					SetFcolor(0x4A69);		   //灰色
					GUI_Rectangle(179,167,481,289);
					GUI_Rectangle(178,166,482,290);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
	 			case 19: 
				{															   //61 探头组参数
																			   //61 探头组参数-->右键-->左键
					if(LR_FlagStatus == 1)
					LR_FlagStatus++;
					if(LR_FlagStatus == 3)
					LR_FlagStatus--;
					UD_FlagStatus = 0;
					System_FlagStatus = 50;
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][42]);
					DisText(45,120,0,2,Content[Lang_Flag][77]);
					DisText(45,145,0,2,Content[Lang_Flag][78]);
					DisText(45,170,0,2,Content[Lang_Flag][79]);
					DisText(45,195,0,2,Content[Lang_Flag][80]);
					DisText(45,220,0,2,Content[Lang_Flag][81]);
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;
	
				}   break;
	 			case 20: 
				{															   //6安装参数
																			   //6安装参数设置-->右键-->左键
					if(LR_FlagStatus == 0)
					LR_FlagStatus++;
					if(LR_FlagStatus == 2)
					LR_FlagStatus--;												
					System_FlagStatus = 5;
					UD_FlagStatus = 0;
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][29]);
				    DisText(45,120,0,2,Content[Lang_Flag][42]);
					DisText(45,145,0,2,Content[Lang_Flag][43]);
					DisText(45,170,0,2,Content[Lang_Flag][44]);
					DisText(45,195,0,2,Content[Lang_Flag][45]);
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
	 			case 21: 		 
				{                                                              //7系统参数设置
				    										                   //7系统参数设置-->右键-->左键
					if(LR_FlagStatus == 0)
					LR_FlagStatus++;
					if(LR_FlagStatus == 2)
					LR_FlagStatus--;
					System_FlagStatus = 6;
					UD_FlagStatus = 0; 
				    WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][46]);
				    DisText(45,120,0,2,Content[Lang_Flag][47]);
					DisText(45,145,0,2,Content[Lang_Flag][48]);
					DisText(45,170,0,2,Content[Lang_Flag][49]);
					DisText(45,195,0,2,Content[Lang_Flag][50]);
					DisText(45,220,0,2,Content[Lang_Flag][51]);
					DisText(45,245,0,2,Content[Lang_Flag][52]);
					DisText(45,270,0,2,Content[Lang_Flag][53]);
					DisText(45,295,0,2,Content[Lang_Flag][54]);
					DisText(45,320,0,2,Content[Lang_Flag][55]);
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
	 			case 22: 
				{															   //72 Change date
				    LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 61;
					Get_Cur_Time();
					itoa(calendar.year,YEAR1);
					itoaD(calendar.month,MONTH1);
					itoaD(calendar.date,DATE1);
					strcat(strcat(strcat(strcat(YEAR1,"."),MONTH1),"."),DATE1);
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisText(5,80,0,2,Content[Lang_Flag][48]);
					GUI_Rectangle(180,168,480,288);
				    GUI_Line(180,228,480,228);
					DisText(200,170,0,9,YEAR1);
					DisText(200,240,0,5,Content[Lang_Flag][56]);
					DisText(200,270,0,5,Content[Lang_Flag][57]);
					DisText(20,180,0,7,Content[Lang_Flag][58]);
					DisText(20,240,0,7,Content[Lang_Flag][59]);
					SetFcolor(0x4A69);		   //灰色
					GUI_Rectangle(179,167,481,289);
					GUI_Rectangle(178,166,482,290);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
	 			case 23: 
				{															   //73 Change time
					LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 62;
					Get_Cur_Time();
					itoaD(calendar.hour,HOUR1);
					itoaD(calendar.min,MIN1);
					itoaD(calendar.second,SECOND1);
					strcat(strcat(strcat(strcat(HOUR1,":"),MIN1),":"),SECOND1);
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisText(5,80,0,2,Content[Lang_Flag][49]);
					GUI_Rectangle(180,168,480,288);
				    GUI_Line(180,228,480,228);
					DisText(200,170,0,9,HOUR1);
					DisText(200,240,0,5,Content[Lang_Flag][56]);
					DisText(200,270,0,5,Content[Lang_Flag][57]);
					DisText(20,180,0,7,Content[Lang_Flag][58]);
					DisText(20,240,0,7,Content[Lang_Flag][59]);
					SetFcolor(0x4A69);		   //灰色
					GUI_Rectangle(179,167,481,289);
					GUI_Rectangle(178,166,482,290);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
	 			case 24: 
				{															   //74 Change pass code
					LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 63;
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisText(5,80,0,2,Content[Lang_Flag][50]);
					GUI_Rectangle(180,168,480,288);
				    GUI_Line(180,228,480,228);
					DisText(200,170,0,9,"********");
					DisText(200,230,0,6,Content[Lang_Flag][60]);
					SetFcolor(0x4A69);		   //灰色
					GUI_Rectangle(179,167,481,289);
					GUI_Rectangle(178,166,482,290);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}	break;
				case 25: 
				{															   //2查看储存信息
																			   //2查看储存信息-->右键-->左键
					if(LR_FlagStatus == 0)
					LR_FlagStatus++;
					if(LR_FlagStatus == 2)
					LR_FlagStatus--;												
					System_FlagStatus = 1;
					UD_FlagStatus = 0;
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][25]);
				    DisText(45,120,0,2,Content[Lang_Flag][61]);
					DisText(45,145,0,2,Content[Lang_Flag][62]);
					DisText(45,170,0,2,Content[Lang_Flag][63]);
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 26: 
				{															   //21 查看警报信息 
				    LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 10;
					
					Button_Dn = 2;
					
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(135,16,22,0);		
					DisText(271,23,0,7,Content[Lang_Flag][64]);
					DisText(15,85,0,3,Content[Lang_Flag][1]);
					DisText(340,85,0,3,Content[Lang_Flag][2]);
					DisText(470,85,0,3,Content[Lang_Flag][3]);
	
					if(Alarm_num>=Button_Dn)
				    {
				       Alarm_Display_A(1);
				       Alarm_Display_B(2);
					   GUI_Rectangle(465,115,600,175);
				    }
				    else if(Alarm_num==1)
				    {
					   Alarm_Display_A(1);
					   GUI_Rectangle(465,115,600,175);
				    }
				    else if(Alarm_num==0)
				    {
				        ;
				    }

					DisplyLayer(Layer);
					Layer=3-Layer;
							
				}   break;
				case 27:			
				{															   //21查看警报历史信息界面时，下键的功能
				    if(Alarm_num == Button_Dn)
					{
					    if ((Button_Dn-UD_FlagStatus) == 2)
						{
						    UD_FlagStatus++;
							WriteLayer(Layer);
							SetFcolor(0xFFFF);
							GUI_RectangleFill(131,1,499,72);
							GUI_RectangleFill(0,74,619,380);
							SetFcolor(0);
							DisArea_Image(135,16,22,0);		
							DisText(271,23,0,7,Content[Lang_Flag][64]);
							DisText(15,85,0,3,Content[Lang_Flag][1]);
							DisText(340,85,0,3,Content[Lang_Flag][2]);
							DisText(470,85,0,3,Content[Lang_Flag][3]);
							Alarm_Display_A(Button_Dn-1);			 
				     		Alarm_Display_B(Button_Dn);
							GUI_Rectangle(465,235,600,295);
							DisplyLayer(Layer);
						    Layer=3-Layer;
						}										
					}
					else
					{
					    if(Alarm_num>Button_Dn)	 
					    {   
						    UD_FlagStatus++;   
						    				                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	                        if (UD_FlagStatus%2 == 0)
							{
							    Button_Dn+=2;
							}                                                                                                                                                                                                                                                                                         
							WriteLayer(Layer);
							SetFcolor(0xFFFF);
							GUI_RectangleFill(131,1,499,72);
							GUI_RectangleFill(0,74,619,380);
							SetFcolor(0);
							DisArea_Image(135,16,22,0);		
							DisText(271,23,0,7,Content[Lang_Flag][64]);
							DisText(15,85,0,3,Content[Lang_Flag][1]);
							DisText(340,85,0,3,Content[Lang_Flag][2]);
							DisText(470,85,0,3,Content[Lang_Flag][3]);
		
						    if(Alarm_num>=Button_Dn)
				            {
				                Alarm_Display_A(Button_Dn-1);			 
				     		    Alarm_Display_B(Button_Dn);			     
					        }			        
							else if(Alarm_num<Button_Dn)
				     	    {
					       	    Alarm_Display_A(Button_Dn-1);			
				     	    }
	
							if (UD_FlagStatus%2 == 0)
							{
								GUI_Rectangle(465,115,600,175);                //在A处显示选择框
							}
							else
							{
							    GUI_Rectangle(465,235,600,295);                //在B处显示选择框
							}
							DisplyLayer(Layer);
						    Layer=3-Layer;
						}
						else
						{
						   ;
						}
					}
					
				}	break;
				case 28:			 
				{															   //21查看警报历史信息界面时，上键的功能
			     	UD_FlagStatus--;
				    if (UD_FlagStatus%2 != 0)
					{
						Button_Dn-=2;
					}
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(135,16,22,0);		
					DisText(271,23,0,7,Content[Lang_Flag][64]);
					DisText(15,85,0,3,Content[Lang_Flag][1]);
					DisText(340,85,0,3,Content[Lang_Flag][2]);
					DisText(470,85,0,3,Content[Lang_Flag][3]);
					Alarm_Display_A(Button_Dn-1);				
			        Alarm_Display_B(Button_Dn);			     	
					if (UD_FlagStatus%2 != 0)
					{
						GUI_Rectangle(465,235,600,295);                        //在B处显示选择框
					}
					else
					{
						GUI_Rectangle(465,115,600,175);                        //在A处显示选择框 
					}
					DisplyLayer(Layer);
					Layer=3-Layer;
				}	break;
				case 29: 
				{															   //75 删除历史信息
																			   //75 删除历史信息-->右键-->左键
					if(LR_FlagStatus == 1)
					LR_FlagStatus++;
					if(LR_FlagStatus == 3)
					LR_FlagStatus--;
					UD_FlagStatus = 0;
					System_FlagStatus = 64;
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][51]);
					DisText(45,120,0,2,Content[Lang_Flag][65]);
					DisText(45,145,0,2,Content[Lang_Flag][66]);
					DisText(45,170,0,2,Content[Lang_Flag][67]);
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;
	
				}   break;
				case 30: 
				{															   //751 删除警报历史信息
																				 
					LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 640;
					Confirm_FlagStatus = 0;
																	 																				 
					WriteLayer(Layer);											 
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][65]);					
					DisText(110,190,0,7,Content[Lang_Flag][68]);
					DisText(50,250,0,7,Content[Lang_Flag][69]);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
				case 31: 
				{															   //751 删除警报历史信息--确认删除
				    _24C08_I2C_HalfwordWrite(0,0);
					Alarm_num = 0;
					Confirm_FlagStatus = 1;									   //确保确认键只能按一次
											 																				 
					WriteLayer(Layer);
					SetFcolor(0x9FF9);					                                   //设置前景为灰色
	                GUI_CircleFill(666,80,5);			                                   //报警灯灭											 
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					GUI_RectangleFill(60,390,100,415);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][65]);					
					DisText(150,170,0,7,Content[Lang_Flag][70]);
					itoaT(Alarm_num,JINGBAO_CUT);
	                DisText(60,390,0,2,JINGBAO_CUT);
					DisplyLayer(Layer);
					CopyLayer(Layer,3-Layer);
					Layer=3-Layer;
				}   break;
				case 32: 		 
				{                                                              //4测试元件
				    LR_FlagStatus++;
					System_FlagStatus = 3;
					Confirm_FlagStatus = 0;
				    WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][27]);
					DisText(130,190,0,7,Content[Lang_Flag][71]);
					DisText(45,250,0,7,Content[Lang_Flag][69]);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
				case 33: 
				{															   //4测试元件--确认检测				   										
					Confirm_FlagStatus = 1;									   //确保确认键只能按一次
					Test_mode_Flag = 1;
					
					WriteLayer(Layer);											 
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][27]);					
					DisText(150,170,0,7,Content[Lang_Flag][72]);
                    DisText(200,250,0,7,Content[Lang_Flag][73]);
					DisplyLayer(Layer);
					Layer=3-Layer;

					FSMC_CPLD_Write(0x55,0x17);							 	   //开启测试信号
					Delay_ms(0xffff);
					Delay_ms(0xffff);


					while(Test_result_Flag ==0)
					{
					   ;
					}
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(0,125,619,380);
					SetFcolor(0);
					if(Test_result_Flag == 1)								   //故障
					{
						DisText(50,150,0,7,Content[Lang_Flag][99]);
						DisText(50,250,0,7,Content[Lang_Flag][101]);
						SetFcolor(0xF800);									   //故障灯亮
		                GUI_CircleFill(719,80,5);
						Fault_num = _24C08_I2C_HalfwordRead(48);
						Data_Save_Fault();
						_24C08_I2C_HalfwordWrite(Fault_num++,48);
						itoaT(Fault_num,GUZHANG_CUT);
					    SetTextView(1,39,GUZHANG_CUT);		   
					}
					if(Test_result_Flag == 2)								   //正常
					{
						DisText(50,150,0,7,Content[Lang_Flag][100]);
						DisText(50,250,0,7,Content[Lang_Flag][101]);
						SetFcolor(0x9FF9);
	                    GUI_CircleFill(719,80,5);
						Fault_num = 0;
						_24C08_I2C_HalfwordWrite(0,48);
						itoaT(Fault_num,GUZHANG_CUT);
					    SetTextView(1,39,GUZHANG_CUT);	   
					}
					DisplyLayer(Layer);
					CopyLayer(Layer,3-Layer);
					Layer=3-Layer;					
					FSMC_CPLD_Write(0x00,0x17);							 	   //关闭测试信号
					Delay_ms(0xffff);
					Poweron_TIM2_Enable_Flag = 0;					
					Test_result_Flag = 0;
				}   break;
				case 34: 		 
				{                                                              //3关闭功能
				    LR_FlagStatus++;
					System_FlagStatus = 2;
				    WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][26]);
					DisText(130,190,0,7,Content[Lang_Flag][74]);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
				case 35: 		 
				{                                                              //5维护模式
				    LR_FlagStatus++;
					System_FlagStatus = 4;
				    WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][28]);
					DisText(130,190,0,7,Content[Lang_Flag][74]);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
				case 36: 		 
				{                                                              //8关闭系统
				    LR_FlagStatus++;
					System_FlagStatus = 7;
				    WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][31]);
					DisText(130,190,0,7,Content[Lang_Flag][74]);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
				case 37: 
				{															   //71 更改语言
					LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 60;
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][47]);
					DisText(45,120,0,2,"711   Language English");
					DisText(45,145,0,2,"712   简体中文");
					if(Lang_Flag == 0)
					{
						GUI_Rectangle(93,128,103,138);
						GUI_RectangleFill(93,153,103,163);	
					}
					else
					{
						GUI_RectangleFill(93,128,103,138);
						GUI_Rectangle(93,153,103,163);
					}
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 38: 
				{															   //71 Change language--711 Language English
					Lang_Flag = 1;
					_24C08_I2C_HalfwordWrite(1,12);							  //保存此时状态到 EEPROM 用于下次开机使用
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][47]);
					DisText(45,120,0,2,"711   Language English");
					DisText(45,145,0,2,"712   简体中文");
					GUI_RectangleFill(93,128,103,138);
					GUI_Rectangle(93,153,103,163);
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 39: 
				{															   //71 Change language--712 简体中文
					Lang_Flag = 0;
					_24C08_I2C_HalfwordWrite(0,12);
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][47]);
					DisText(45,120,0,2,"711   Language English");
					DisText(45,145,0,2,"712   简体中文");
					GUI_Rectangle(93,128,103,138);
					GUI_RectangleFill(93,153,103,163);
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 40: 
				{															   //没有权限，禁止访问高级功能
					SetFcolor(0xFFFF);
					GUI_RectangleFill(45,320,500,370);
					SetFcolor(0);		
					DisText(45,330,0,2,Content[Lang_Flag][84]);	
				}   break;
				case 41: 
				{															   //77 开关内部喇叭
					LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 66;
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][53]);
					DisText(45,120,0,2,Content[Lang_Flag][97]);
					DisText(45,145,0,2,Content[Lang_Flag][98]);
					if(Buzzer_Flag == 1)
					{
						GUI_Rectangle(93,128,103,138);
						GUI_RectangleFill(93,153,103,163);	
					}
					else
					{
						GUI_RectangleFill(93,128,103,138);
						GUI_Rectangle(93,153,103,163);
					}
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 42: 
				{															   //77 开关内部喇叭--771 打开按键蜂鸣器
					Buzzer_Flag = 1;
					_24C08_I2C_HalfwordWrite(1,14);							  //保存此时状态到 EEPROM 用于下次开机使用
					SetTouchScreen(3);										  
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][53]);
					DisText(45,120,0,2,Content[Lang_Flag][97]);
					DisText(45,145,0,2,Content[Lang_Flag][98]);
					GUI_RectangleFill(93,128,103,138);
					GUI_Rectangle(93,153,103,163);
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 43: 
				{															   //77 开关内部喇叭--772 关闭按键蜂鸣器
					Buzzer_Flag = 0;
					_24C08_I2C_HalfwordWrite(0,14);
					SetTouchScreen(1);
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][53]);
					DisText(45,120,0,2,Content[Lang_Flag][97]);
					DisText(45,145,0,2,Content[Lang_Flag][98]);
					GUI_Rectangle(93,128,103,138);
					GUI_RectangleFill(93,153,103,163);
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 44: 
				{															   //22 查看故障信息 
				    LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 11;
					
					
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(135,16,22,0);		
					DisText(271,23,0,7,Content[Lang_Flag][64]);
					DisText(15,85,0,3,Content[Lang_Flag][1]);
					DisText(340,85,0,3,Content[Lang_Flag][2]);
					DisText(470,85,0,3,Content[Lang_Flag][3]);
	
				    Fault_Display_A(Fault_num);

					DisplyLayer(Layer);
					Layer=3-Layer;
							
				}   break;
				case 45: 
				{															   //752 删除故障历史信息
																				 
					LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 641;
					Confirm_FlagStatus = 0;
																	 																				 
					WriteLayer(Layer);											 
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][66]);					
					DisText(110,190,0,7,Content[Lang_Flag][106]);
					DisText(50,250,0,7,Content[Lang_Flag][69]);
					DisplyLayer(Layer);
					Layer=3-Layer;
				}   break;
				case 46: 
				{															   //752 删除故障历史信息--确认删除
				    _24C08_I2C_HalfwordWrite(0,48);
					Fault_num = 0;
					Confirm_FlagStatus = 1;									   //确保确认键只能按一次
											 																				 
					WriteLayer(Layer);
					SetFcolor(0x9FF9);					                                   //设置前景为灰色
	                GUI_CircleFill(719,80,5);			                                   //故障灯灭											 
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][66]);					
					DisText(150,170,0,7,Content[Lang_Flag][107]);
					itoaT(Fault_num,GUZHANG_CUT);
					SetTextView(1,39,GUZHANG_CUT);
					DisplyLayer(Layer);
					CopyLayer(Layer,3-Layer);
					Layer=3-Layer;
				}   break;
				case 47: 
				{															   //3关闭操作
																			   //3关闭操作-->右键-->左键
					if(LR_FlagStatus == 0)
					LR_FlagStatus++;
					if(LR_FlagStatus == 2)
					LR_FlagStatus--;												
					System_FlagStatus = 2;
					UD_FlagStatus = 0;
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][26]);
				    DisText(45,120,0,2,Content[Lang_Flag][108]);
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 48: 
				{															   //31 探头组开关操作
					LR_FlagStatus++;
					UD_FlagStatus = 0;
					System_FlagStatus = 20;
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][108]);
					DisText(45,120,0,2,Content[Lang_Flag][110]);
					DisText(45,145,0,2,Content[Lang_Flag][109]);
					if(Disable_num == 0)
					{
						GUI_Rectangle(93,128,103,138);
						GUI_RectangleFill(93,153,103,163);	
					}
					else
					{
						GUI_RectangleFill(93,128,103,138);
						GUI_Rectangle(93,153,103,163);
					}
					xitong_Dn=125;
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 49: 
				{															   //31 探头组开关操作--311 打开探头组
					Disable_num = 0;
					_24C08_I2C_HalfwordWrite(0,64);							  //保存此时状态到 EEPROM 用于下次开机使用
					TIM_Cmd(TIM2,ENABLE);								  
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][108]);
					DisText(45,120,0,2,Content[Lang_Flag][110]);
					DisText(45,145,0,2,Content[Lang_Flag][109]);
					GUI_Rectangle(93,128,103,138);
					GUI_RectangleFill(93,153,103,163);
					itoaT(Disable_num,GUANBI_CUT);	   
	                SetTextView(1,40,GUANBI_CUT);
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				case 50: 
				{															   //31 探头组开关操作--312 关闭探头组
					Disable_num = 1;
					_24C08_I2C_HalfwordWrite(0,64);
					TIM_Cmd(TIM2,DISABLE);
					Data_Save_Disable();
					WriteLayer(Layer);
					SetFcolor(0xFFFF);
					GUI_RectangleFill(131,1,499,72);
					GUI_RectangleFill(0,74,619,380);
					SetFcolor(0);
					DisArea_Image(143,23,21,0);		
					DisText(271,23,0,7,Content[Lang_Flag][23]);
					DisText(5,80,0,2,Content[Lang_Flag][108]);
					DisText(45,120,0,2,Content[Lang_Flag][110]);
					DisText(45,145,0,2,Content[Lang_Flag][109]);
					GUI_RectangleFill(93,128,103,138);
					GUI_Rectangle(93,153,103,163);
					itoaT(Disable_num,GUANBI_CUT);	   
	                SetTextView(1,40,GUANBI_CUT);
					DisArea_Image(20,xitong_Dn,2,0);
					DisplyLayer(Layer);
					Layer=3-Layer;	
				}   break;
				default:break;
			}
												
		}
	}
	else if(ScreenDataBuf.Count == 13)//查询触控按钮状态时返回的数据字节数
	{
		Status	= ScreenDataBuf.RcevData[8];//0x00表示弹起状态  0x01表示按下状态	
	}

}


void Power_on_Init()
{
	SetFcolor(0);	
	DisText(250,23,0,7,Content[Lang_Flag][102]);	
}

void Power_on_self_Test()													   //开机时检测传感器等元件是否工作正常
{
	Test_mode_Flag = 1;
	Delay_ms(0xffff);
	FSMC_CPLD_Write(0x55,0x17);							 	   //开启测试信号
	TIM_Cmd(TIM2,ENABLE);									   //打开定时器2
	while(Test_result_Flag ==0)
	{
	   ;
	}
	if(Test_result_Flag == 1)								   //故障
	{
	    Fault_num = _24C08_I2C_HalfwordRead(48);
		Data_Save_Fault();
		_24C08_I2C_HalfwordWrite(Fault_num++,48);

	}
	if(Test_result_Flag == 2)								   //工作正常
	{
	   	Fault_num = _24C08_I2C_HalfwordRead(48);
	}
	TIM_Cmd(TIM2,DISABLE);
	FSMC_CPLD_Write(0x00,0x17);							 	   //关闭测试信号	

	Test_result_Flag = 0;
}



void ScreenInit()                                                              //屏幕初始化程序
{	
	  Layer = 2;					   //图层计数
	  Menu_FlagStatus = 0;             //菜单键取值状态，取值0/1/2/3，对应4个菜单
	  System_FlagStatus = -1;		   //系统一级菜单的取值，0~7，-1为初始化
	  LR_FlagStatus = 0;               //左右键计数状态，按一次右键加1，按一次左键减1
	  UD_FlagStatus = 0;               //上下键计数状态，按一次下键加1，按一次上键减1
	  Button_Dn = 2;				   //警报界面时，上下键按一次更新显示另外2条信息

	  _24C08_I2C_HalfwordWrite(0,0);			                               //火花事件计数清0                       
	  _24C08_I2C_PageWrite(I2C_Data,16,4);		                               //初始密码定为1234
	  _24C08_I2C_ByteWrite(4,32); 				                               //初始密码长度定为4位
	  _24C08_I2C_ByteWrite(1,14);											   //蜂鸣器开关
	  Spark_DANGER_A = _24C08_I2C_HalfwordRead(2);							   //获取EEPROM中相关数据用以初始化
	  Spark_DANGER_B = _24C08_I2C_HalfwordRead(4);
	  Spark_DANGER_B_M = _24C08_I2C_HalfwordRead(6);
	  Spark_DANGER_C_N = _24C08_I2C_HalfwordRead(8);
	  Spray_TIME = _24C08_I2C_HalfwordRead(10);
	  Lang_Flag = _24C08_I2C_HalfwordRead(12);
	  Buzzer_Flag =	_24C08_I2C_HalfwordRead(14);							   


	  WriteLayer(1);					                                       //写图层一						
	   
	  SetFcolor(0xFFFF);
	  GUI_RectangleFill(131,1,499,72);
      GUI_RectangleFill(0,74,619,380);
	  GUI_RectangleFill(501,0,619,72);
	  Alarm_num = _24C08_I2C_HalfwordRead(0);								   //读取当前火花事件记录个数
	  if(Alarm_num != 0)
	  {
		  SetFcolor(0xF800);					                                   //设置前景为红色 
		  GUI_CircleFill(666,80,5);
	  }										                                   //报警灯亮
	  else
	  {
	      SetFcolor(0x9FF9);					                                   //设置前景为灰色
	      GUI_CircleFill(666,80,5);			                                   //报警灯灭
	  }

	  if(Fault_num != 0)
	  {
		  SetFcolor(0xF800);
		  GUI_CircleFill(719,80,5);
	  	  SetFcolor(0);
	      DisText(170,200,0,7,Content[Lang_Flag][103]);
	  }										                                   //故障灯亮
	  else
	  {
	      SetFcolor(0x9FF9);
	      GUI_CircleFill(719,80,5);			                                   //故障灯灭
		  SetFcolor(0);
		  DisText(180,200,0,9,Content[Lang_Flag][104]);
	  }
	  DisText(271,23,0,7,Content[Lang_Flag][105]);
	  itoaT(Alarm_num,JINGBAO_CUT);
	  itoaT(Fault_num,GUZHANG_CUT);
	  itoaT(Disable_num,GUANBI_CUT);	   
	  SetTextView(1,38,JINGBAO_CUT);
	  SetTextView(1,39,GUZHANG_CUT);
	  SetTextView(1,40,GUANBI_CUT);
	  if(Code_Flag2 == 0)
	  DisArea_Image(534,12,1,0);
	  if(Code_Flag2 != 0)
	  DisArea_Image(534,12,0,0);						 
	  DisplyLayer(1);					                                       //显示图层一
	  CopyLayer(1,2);					                                       //保留图层一的内容到图层二

	  TIM_Cmd(TIM2,ENABLE);													   //启动每0.1s一次的定时中断
}

void Screen_update()                                                           //屏幕数据更新显示程序
{
      Menu_FlagStatus = 0;          //菜单键取值状态，取值0/1/2/3，对应4个菜单
	  System_FlagStatus = -1;		//系统一级菜单的取值，0~7，-1为初始化
	  LR_FlagStatus = 0;            //左右键计数状态，按一次右键加1，按一次左键减1
	  UD_FlagStatus = 0;            //上下键计数状态，按一次下键加1，按一次上键减1
	  Button_Dn = 2;			    //警报界面时，上下键按一次更新显示另外2条信息


	  WriteLayer(Layer);					                                   //写图层						

	  SetFcolor(0xFFFF);
	  GUI_RectangleFill(131,1,499,72);
      GUI_RectangleFill(0,74,619,380);

	  Alarm_num = _24C08_I2C_HalfwordRead(0);								   //最新火花事件数更新显示
	  if(Alarm_num != 0)
	  {
	  SetFcolor(0xF800);					                                   //设置前景为红色 
	  GUI_CircleFill(666,80,5);
	  }										                                   //报警灯亮
	  else
	  {
	   SetFcolor(0x9FF9);					                                   //设置前景为灰色
	   GUI_CircleFill(666,80,5);			                                   //报警灯灭
	  }

	  itoaT(Alarm_num,JINGBAO_CUT);
	  SetTextView(1,38,JINGBAO_CUT);

	  DisArea_Image(135,16,22,0);
	  SetFcolor(0);		
	  DisText(271,23,0,7,Content[Lang_Flag][0]);				   
	  
	  if(Alarm_num >= 1000)
	  {
	      DisText(130,190,0,7,Content[Lang_Flag][75]);
		  DisText(135,250,0,7,Content[Lang_Flag][76]);
	  }		
	  else
	  {
		  DisText(15,85,0,3,Content[Lang_Flag][1]);
		  DisText(340,85,0,3,Content[Lang_Flag][2]);
		  DisText(470,85,0,3,Content[Lang_Flag][3]);
	
		  if(Alarm_num>=Button_Dn)
		  {
		     Alarm_Display_A(Alarm_num);
			 Alarm_Display_B(Alarm_num-1);
			 GUI_Rectangle(465,115,600,175);
		  }
		  else if(Alarm_num==1)
		  {
			 Alarm_Display_A(1);
			 GUI_Rectangle(465,115,600,175);
		  }
		  else if(Alarm_num==0)
		  {
		      ;
		  }
	  }								 
	  DisplyLayer(Layer);
	  CopyLayer(Layer,3-Layer);					   
	  Layer=3-Layer;					   
}
		

void Fault_Display_A(u8 num)					                               //故障显示A区域		num-第几个故障
{
        if(num <= 0)
		{
			;
		}
		else
		{
	        Data_Get_Once_Fault(num);
		    itoaD(Get_Fault_Info.calendar.year,YEAR1);
			itoaD(Get_Fault_Info.calendar.month,MONTH1);
			itoaD(Get_Fault_Info.calendar.date,DATE1);
			itoaD(Get_Fault_Info.calendar.hour,HOUR1);
			itoaD(Get_Fault_Info.calendar.min,MIN1);
			itoaD(Get_Fault_Info.calendar.second,SECOND1);
			strcat(strcat(strcat(strcat(HOUR1,":"),MIN1),":"),SECOND1);
			strcat(strcat(strcat(strcat(YEAR1,"."),MONTH1),"."),DATE1);		   //把后者连接到前者的串尾
		    SetFcolor(0xFFFF);					                               //白
			GUI_RectangleFill(1,120,610,350);								   //GUI_RectangleFill(1,220,499,320);
			SetFcolor(0);				                                       //黑				
			DisText(15,122,0,2,HOUR1);
			DisText(15,147,0,2,YEAR1);
			DisText(145,115,0,9,"DG");
			DisText(235,115,0,9,"001");
			itoaD(Get_Fault_Info.calendar.hour,HOUR1);
			itoaD(Get_Fault_Info.calendar.min,MIN1);
			itoaD(Get_Fault_Info.calendar.second+1,SECOND1);
			strcat(strcat(strcat(strcat(HOUR1,":"),MIN1),":"),SECOND1);
			itoaF(Get_Fault_Info.Event_Nr,EVENT_NR);
			DisText(340,122,0,2,HOUR1);
			DisText(340,147,0,2,YEAR1);
			DisText(470,115,0,9,EVENT_NR);
			DisText(15,175,0,3,Content[Lang_Flag][5]);
			DisText(15,205,0,2,Content[Lang_Flag][14]);
		}
}

void Disable_Display_A(u8 num)					                               //关闭显示A区域		num-第几个关闭事件
{
        if(num <= 0)
		{
			;
		}
		else
		{
	        Data_Get_Once_Disable(num);
		    itoaD(Get_Disable_Info.calendar.year,YEAR1);
			itoaD(Get_Disable_Info.calendar.month,MONTH1);
			itoaD(Get_Disable_Info.calendar.date,DATE1);
			itoaD(Get_Disable_Info.calendar.hour,HOUR1);
			itoaD(Get_Disable_Info.calendar.min,MIN1);
			itoaD(Get_Disable_Info.calendar.second,SECOND1);
			strcat(strcat(strcat(strcat(HOUR1,":"),MIN1),":"),SECOND1);
			strcat(strcat(strcat(strcat(YEAR1,"."),MONTH1),"."),DATE1);		   //把后者连接到前者的串尾
		    SetFcolor(0xFFFF);					                               //白
			GUI_RectangleFill(1,120,610,350);								   //GUI_RectangleFill(1,220,499,320);
			SetFcolor(0);				                                       //黑				
			DisText(15,122,0,2,HOUR1);
			DisText(15,147,0,2,YEAR1);
			DisText(145,115,0,9,"DG");
			DisText(235,115,0,9,"001");
			itoaD(Get_Disable_Info.calendar.hour,HOUR1);
			itoaD(Get_Disable_Info.calendar.min,MIN1);
			itoaD(Get_Disable_Info.calendar.second+1,SECOND1);
			strcat(strcat(strcat(strcat(HOUR1,":"),MIN1),":"),SECOND1);
			itoaF(Get_Disable_Info.Event_Nr,EVENT_NR);
			DisText(340,122,0,2,HOUR1);
			DisText(340,147,0,2,YEAR1);
			DisText(470,115,0,9,EVENT_NR);
			DisText(15,175,0,3,Content[Lang_Flag][5]);
			DisText(15,205,0,2,Content[Lang_Flag][111]);
		}
}

void Alarm_Display_A(u8 num)					                               //警报显示A区域		num-第几次报警
{
        if(num <= 0)
		{
			;
		}
		else
		{
	        Data_Get_Once(num);
		    itoaD(Get_Info.calendar.year,YEAR1);
			itoaD(Get_Info.calendar.month,MONTH1);
			itoaD(Get_Info.calendar.date,DATE1);
			itoaD(Get_Info.calendar.hour,HOUR1);
			itoaD(Get_Info.calendar.min,MIN1);
			itoaD(Get_Info.calendar.second,SECOND1);
			strcat(strcat(strcat(strcat(HOUR1,":"),MIN1),":"),SECOND1);
			strcat(strcat(strcat(strcat(YEAR1,"."),MONTH1),"."),DATE1);		   //把后者连接到前者的串尾
		    SetFcolor(0xFFFF);					                               //白
			GUI_RectangleFill(1,120,610,350);								   //GUI_RectangleFill(1,220,499,320);
			SetFcolor(0);				                                       //黑				
			DisText(15,122,0,2,HOUR1);
			DisText(15,147,0,2,YEAR1);
			DisText(145,115,0,9,"DG");
			DisText(235,115,0,9,"001");
			itoaD(Get_Info.calendar.hour,HOUR1);
			itoaD(Get_Info.calendar.min,MIN1);
			itoaD(Get_Info.calendar.second+1,SECOND1);
			strcat(strcat(strcat(strcat(HOUR1,":"),MIN1),":"),SECOND1);
			itoaF(Get_Info.Event_Nr,EVENT_NR);
			DisText(340,122,0,2,HOUR1);
			DisText(340,147,0,2,YEAR1);
			DisText(470,115,0,9,EVENT_NR);
			DisText(15,175,0,3,Content[Lang_Flag][5]);
			itoaT(Get_Info.spark_num,SPARK_NUM);
			strcat(strcat(strcat(SPARKS,Content[Lang_Flag][82]),SPARK_NUM),Content[Lang_Flag][83]);
			DisText(15,205,0,2,SPARKS);
			memset(SPARKS,'\0', sizeof(SPARKS));			                   //清零SPARKS字符串
		}
}														 


void Alarm_Display_B(u8 num)					                               //警报显示B区域		num-第几次报警
{
        if(num <= 0)
		{
			;
		}
		else
		{
	        Data_Get_Once(num);
		    itoaD(Get_Info.calendar.year,YEAR2);
			itoaD(Get_Info.calendar.month,MONTH2);
			itoaD(Get_Info.calendar.date,DATE2);
			itoaD(Get_Info.calendar.hour,HOUR2);
			itoaD(Get_Info.calendar.min,MIN2);
			itoaD(Get_Info.calendar.second,SECOND2);
			strcat(strcat(strcat(strcat(HOUR2,":"),MIN2),":"),SECOND2);
			strcat(strcat(strcat(strcat(YEAR2,"."),MONTH2),"."),DATE2);		   //把后者连接到前者的串尾
		    SetFcolor(0xFFFF);				                                   //白
			GUI_RectangleFill(1,241,610,330);
			SetFcolor(0);				                                       //黑								   			
			DisText(15,242,0,2,HOUR2);
			DisText(15,267,0,2,YEAR2);
			DisText(145,235,0,9,"DG");
			DisText(235,235,0,9,"001");
			itoaD(Get_Info.calendar.hour,HOUR2);
			itoaD(Get_Info.calendar.min,MIN2);
			itoaD(Get_Info.calendar.second+1,SECOND2);
			strcat(strcat(strcat(strcat(HOUR2,":"),MIN2),":"),SECOND2);
			itoaF(Get_Info.Event_Nr,EVENT_NR);
			DisText(340,242,0,2,HOUR2);
			DisText(340,267,0,2,YEAR2);
			DisText(470,235,0,9,EVENT_NR);
			DisText(15,290,0,3,Content[Lang_Flag][5]);
		    itoaT(Get_Info.spark_num,SPARK_NUM);
			strcat(strcat(strcat(SPARKS,Content[Lang_Flag][82]),SPARK_NUM),Content[Lang_Flag][83]);
			DisText(15,320,0,2,SPARKS);
			memset(SPARKS,'\0', sizeof(SPARKS));		                       //清零SPARKS字符串
		}

}	


void itoa(int num,char str[] )				                                   //将整型转换成字符串类型
{
    int sign = num,i = 0,j = 0;
    char temp[11];
    if(sign<0)//判断是否是一个负数
    {
        num = -num;
    };
    do
    {
        temp[i] = num%10+'0';        
        num/=10;
        i++;
    }while(num>0);
    if(sign<0)
    {
        temp[i++] = '-';//对于负数,要加以负号
    }
    temp[i] = '\0';
    i--;
    while(i>=0)//反向操作
    {
        str[j] = temp[i];
        j++;
        i--;
    }
    str[j] = '\0';
}


void itoaD(int num,char str[] )                                                //两位数显示，整型转换成字符串类型，不足用0补位
{ 
    if(0<=num&&num<=9)
	{
	    str[0]=0+'0';
		str[1]=num+'0';
		str[2]='\0';
	}
	if(10<=num&&num<=99)
	{
	    str[0]=num/10+'0';
		str[1]=num%10+'0';
		str[2]='\0';
	}
}


void itoaT(int num,char str[] )                                                //三位数显示，整型转换成字符串类型，不足用0补位
{ 
    if(0<=num&&num<=9)
	{
	    str[0]=0+'0';
		str[1]=0+'0';
		str[2]=num+'0';
		str[3]='\0';
	}
	if(10<=num&&num<=99)
	{
	    str[0]=0+'0';
	    str[1]=num/10+'0';
		str[2]=num%10+'0';
		str[3]='\0';
	}
	if(100<=num&&num<=999)
	{
	    str[0]=num/100+'0';
	    str[1]=num/10%10+'0';
		str[2]=num%10+'0';
		str[3]='\0';
	}
}


void itoaF(int num,char str[] )                                                //四位数显示，整型转换成字符串类型，不足用0补位
{ 
    if(0<=num&&num<=9)
	{	
	    str[0]=0+'0';
	    str[1]=0+'0';
		str[2]=0+'0';
		str[3]=num+'0';
		str[4]='\0';
	}
	if(10<=num&&num<=99)
	{
	    str[0]=0+'0';
		str[1]=0+'0';
	    str[2]=num/10+'0';
		str[3]=num%10+'0';
		str[4]='\0';
	}
	if(100<=num&&num<=999)
	{
	    str[0]=0+'0';
	    str[1]=num/100+'0';
	    str[2]=num/10%10+'0';
		str[3]=num%10+'0';
		str[4]='\0';
	}
	if(1000<=num&&num<=9999)
	{
	    str[0]=num/1000+'0';
	    str[1]=num/100%10+'0';
		str[2]=num/10%10+'0';
		str[3]=num%10+'0';
		str[4]='\0';
	}
}


void MemCopy(u8 * DestAddr, u8 *SourceAddr, u32 ulSize)
{
    while(ulSize--)
    { 
       *DestAddr++ = *SourceAddr++;
    }
    return;
}


void MemSet(u16 *Addr, u16 uiData, u32 ulSize)
{
	u32 i;
	for(i = 0; i < ulSize; i++)
	{
		*Addr++ = uiData;
	}
	return;
}


