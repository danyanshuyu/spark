
#ifndef SCREEN_H
#define SCREEN_H

#include "stm32f10x_lib.h"


/*****************************************************************
* 名    称： SendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节       
* 出口参数： 无                  
 *****************************************************************/
void  SendChar(u8 t); 

/*****************************************************************
* 名    称： SendStrings()
* 功    能： 发送字符串
* 入口参数： str  发送的字符串     
* 出口参数： 无               
 *****************************************************************/
void SendStrings(u8 *str);

/****************************************************************************
* 名    称： SendBeginCMD()
* 功    能： 发送帧头命令
* 入口参数： 无      
* 出口参数： 无
****************************************************************************/
void SendBeginCMD();

/****************************************************************************
* 名    称： SendEndCmd()
* 功    能： 发送帧尾命令
* 入口参数： 无      
* 出口参数： 无
****************************************************************************/
void SendEndCmd();

/****************************************************************************
* 名    称： SetHandShake()
* 功    能： 握手
* 入口参数： 无      
* 出口参数： 无
****************************************************************************/
void SetHandShake(void);


/****************************************************************************
* 名    称： SetFcolor()
* 功    能： 设置前景色
* 入口参数： fcolor  前景色RGB参数   
* 出口参数： 无
****************************************************************************/
void SetFcolor(u16 fcolor);

/****************************************************************************
* 名    称： SetBcolor()
* 功    能： 设置背景色
* 入口参数： bcolor  背景色RGB参数   
* 出口参数： 无
****************************************************************************/
void SetBcolor(u16 bcolor);

/****************************************************************************
* 名    称： ColorPicker()
* 功    能： 取屏幕点设置前景色/背景色
* 入口参数： mode 取当前显示屏幕(X,Y)处的颜色作为前景/背景色
             x       以点为单位的X轴坐标值
             y       以点为单位的Y轴坐标值
* 出口参数： 无
****************************************************************************/
void ColorPicker(u8 mode, u16 x,u16 y);

/****************************************************************************
* 名    称： GUI_CleanScreen()
* 功    能： 清屏
* 入口参数： 无  
* 出口参数： 无
****************************************************************************/
void GUI_CleanScreen();

/****************************************************************************
* 名    称： SetTextSpace()
* 功    能： 设置文字行列间距
* 入口参数： x_w     以点为单位的行间距，取值范围00~1F
             y_w     以点为单位的列间距，取值范围00~1F	
* 出口参数： 无
****************************************************************************/
void SetTextSpace(u8 x_w, u8 y_w);

/****************************************************************************
* 名    称： DisText_Region()
* 功    能： 限定文本显示范围
* 入口参数： x       以点为单位的X轴坐标值
             y       以点为单位的Y轴坐标值
             length  文本框限定的长度
	         width   文本框限定的高度
* 出口参数： 无
****************************************************************************/
void SetFont_Region(u8 enable,u16 length,u16 width );

/****************************************************************************
* 名    称： SetFilterColor()
* 功    能： 设置过滤颜色
* 入口参数： fillcolor_dwon   滤除颜色的下限值
             fillcolor_up     滤除颜色的上限值	
* 出口参数： 无
****************************************************************************/
void SetFilterColor(u16 fillcolor_dwon, u16 fillcolor_up);

/****************************************************************************
* 名    称： DisText()
* 功    能： 文本显示
* 入口参数： x         以点为单位的X轴坐标值
             y         以点为单位的Y轴坐标值
   	         back      背景色使能
	         font      字库编码
             strings   写入的字符串
* 出口参数： 无
****************************************************************************/
void DisText(u16 x, u16 y,u8 back,u8 font,u8 *strings );

/****************************************************************************
* 名    称： DisData()
* 功    能： 数据显示
* 入口参数： x       以点为单位的X轴坐标值
             y       以点为单位的Y轴坐标值
   	         back    背景色使能
	         font    字库编码
	         sdata   写入的数据
* 出口参数： 无
****************************************************************************/
void DisData(u16 x, u16 y,u8 back,u8 font,u8 sdata );

/****************************************************************************
* 名    称： DisCursor()
* 功    能： 光标显示
* 入口参数： enable
             x        以点为单位的X轴坐标值
             y        以点为单位的Y轴坐标值
             length   光标长度
	         width    光标宽度
* 出口参数： 无
****************************************************************************/
void DisCursor(u8 enable,u16 x, u16 y,u8 length,u8 width );

/****************************************************************************
* 名    称： DisFull_Image()
* 功    能： 全屏整幅图显示
* 入口参数： image_id   图片ID编号
             masken     屏蔽色使能
* 出口参数： 无
****************************************************************************/
void DisFull_Image(u16 image_id,u8 masken);

/****************************************************************************
* 名    称： DisArea_Image()
* 功    能： 区域图片显示
* 入口参数： x          以点为单位的X轴坐标值
             y          以点为单位的Y轴坐标值 
             image_id   图片ID编号
             masken     屏蔽色使能
* 出口参数： 无
****************************************************************************/
void DisArea_Image(u16 x,u16 y,u16 image_id,u8 masken);

/****************************************************************************
* 名    称： DisCut_Image()
* 功    能： 图片剪切
* 入口参数： x          以点为单位的X轴坐标值
             y          以点为单位的Y轴坐标值 
             image_id   图片编号
             image_x    图片内部X坐标
             image_y    图片内部Y坐标
             image_l    剪切长度
             image_w    剪切宽度
             masken     屏蔽色使能
* 出口参数： 无
****************************************************************************/
void DisCut_Image(u16 x,u16 y,u16 image_id,u16 image_x,u16 image_y,
                   u16 image_l, u16 image_w,u8 masken);

/****************************************************************************
* 名    称： DisFlashImgae()
* 功    能： 动画显示
* 入口参数： x               以点为单位的X轴坐标值
             y               以点为单位的Y轴坐标值 
             flashimage_id   GIF动画ID编号     
    	     enable          关闭/开启动作播放
	         playnum         动画播放次数 0:一直播放，1~FF:播放次数
* 出口参数： 无
****************************************************************************/
void DisFlashImage(u16 x,u16 y,u16 flashimage_id,u8 enable,u8 playnum);

/****************************************************************************
* 名    称： GUI_Dot()
* 功    能： 画点
* 入口参数： x       以点为单位的X轴坐标值
             y       以点为单位的Y轴坐标值           
* 出口参数： 无
****************************************************************************/
void GUI_Dot(u16 x,u16 y);


/****************************************************************************
* 名    称： GUI_Line()
* 功    能： 画线
* 入口参数： x0      以点为单位的X轴坐标值
             y0      以点为单位的Y轴坐标值    
             x1      以点为单位的X轴坐标值
             y1      以点为单位的Y轴坐标值  
* 出口参数： 无
****************************************************************************/
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1);

/****************************************************************************
* 名    称： GUI_ConDots()
* 功    能： 绘制折线
* 入口参数： mode     0:用前景色描绘 1：用背景色描绘    
	         pDot     指向点的指针   
* 出口参数： 无
****************************************************************************/
void GUI_ConDots(u8 mode,u8 *pDot);

/****************************************************************************
* 名    称： GUI_Circle()
* 功    能： 画空心圆
* 入口参数： x               以点为单位的X轴坐标值
             y               以点为单位的Y轴坐标值    
             r                空心圆的半径   
* 出口参数： 无
****************************************************************************/
void GUI_Circle(u16 x, u16 y, u16 r);

/****************************************************************************
* 名    称： GUI_CircleFill()
* 功    能： 画实心圆
* 入口参数： x               以点为单位的X轴坐标值
             y               以点为单位的Y轴坐标值    
             r                实心圆的半径   
* 出口参数： 无
****************************************************************************/
void GUI_CircleFill(u16 x, u16 y, u16 r);

/****************************************************************************
* 名    称： GUI_Arc()
* 功    能： 画圆弧
* 入口参数： x               以点为单位的X轴坐标值
             y               以点为单位的Y轴坐标值    
             r               圆的半径  
 	         sa              起始角度
	         ea              结束角度
* 出口参数： 无
****************************************************************************/
void GUI_Arc(u16 x,u16 y, u16 r,u16 sa, u16 ea);

/****************************************************************************
* 名    称： GUI_Rectangle()
* 功    能： 画空心矩形
* 入口参数： x0      以点为单位的空心矩形左上角X坐标值
             y0      以点为单位的空心矩形左上角Y坐标值   
             x1      以点为单位的空心矩形右下角X坐标值
             y1      以点为单位的空心矩形右下角Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_Rectangle(u16 x0, u16 y0, u16 x1,u16 y1 );

/****************************************************************************
* 名    称： RectangleFill()
* 功    能： 画实心矩形
* 入口参数： x0      以点为单位的实心矩形左上角X坐标值
             y0      以点为单位的实心矩形左上角Y坐标值   
             x1      以点为单位的实心矩形右下角X坐标值
             y1      以点为单位的实心矩形右下角Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_RectangleFill(u16 x0, u16 y0, u16 x1,u16 y1 );

/****************************************************************************
* 名    称： GUI_AreaInycolor()
* 功    能： 屏幕区域反色
* 入口参数： x0      以点为单位的实心矩形左上角X坐标值
             y0      以点为单位的实心矩形左上角Y坐标值   
             x1      以点为单位的实心矩形右下角X坐标值
             y1      以点为单位的实心矩形右下角Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_AreaInycolor(u16 x0, u16 y0, u16 x1,u16 y1 );

/****************************************************************************
* 名    称： GUI_Ellipse()
* 功    能： 画空心椭圆
* 入口参数： x0      以点为单位的空心椭圆最左端X坐标值
             y0      以点为单位的空心椭圆最上端Y坐标值   
             x1      以点为单位的空心椭圆最右端X坐标值
             y1      以点为单位的空心椭圆最下端Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_Ellipse (u16 x0, u16 y0, u16 x1,u16 y1 );

/****************************************************************************
* 名    称： GUI_EllipseFill()
* 功    能： 画实心椭圆
* 入口参数： x0      以点为单位的实心椭圆最左端X坐标值
             y0      以点为单位的实心椭圆最上端Y坐标值   
             x1      以点为单位的实心椭圆最右端X坐标值
             y1      以点为单位的实心椭圆最下端Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_EllipseFill (u16 x0, u16 y0, u16 x1,u16 y1 );

/****************************************************************************
* 名    称： GUI_PolygonFill()
* 功    能： 多边形填充
* 入口参数： x               以点为单位的X轴坐标值
             y               以点为单位的Y轴坐标值    
             color            填充颜色   
* 出口参数： 无
****************************************************************************/
void GUI_PolygonFill(u16 x, u16 y, u16 color);

/****************************************************************************
* 名    称： SetBackLight()
* 功    能： 背光调节
* 入口参数： light_level
* 出口参数： 无
****************************************************************************/
void SetBackLight(u8 light_level);

/****************************************************************************
* 名    称： SetBuzzer()
* 功    能： 蜂鸣器控制
* 入口参数： light_level
* 出口参数： 无
****************************************************************************/
void SetBuzzer(u8 time);

/****************************************************************************
* 名    称： SetCommBps()
* 功    能： 设置通讯波特率
* 入口参数： Baud
* 出口参数： 无
****************************************************************************/
void SetCommBps(u8 baudset);  

/****************************************************************************
* 名    称： SetTouchScreen(u8 cmd)
* 功    能： 触摸屏控制
* 入口参数： cmd   Bit0:1/0 触摸开/闭; Bit1 : 1/0 蜂鸣器开/闭; Bit2: 上传方式  
* 出口参数： 无
****************************************************************************/
void SetTouchScreen(u8 cmd);

/****************************************************************************
* 名    称： SetTouchScreen_Adj()
* 功    能： 触摸屏校准
* 入口参数： 无
* 出口参数： 无
****************************************************************************/
void SetTouchScreen_Adj();

/****************************************************************************
* 名    称： TestTouchScreen()
* 功    能： 触摸屏体验
* 入口参数： 无
* 出口参数： 无
****************************************************************************/
void TestTouchScreen();


/****************************************************************************
* 名    称： WriteLayer()
* 功    能： 写图层操作
* 入口参数： layer         写入的图层数
* 出口参数： 无
****************************************************************************/
void WriteLayer(u8 layer);

/****************************************************************************
* 名    称： WriteLayer()
* 功    能： 切换图层显示
* 入口参数： layer         切换显示的图层
* 出口参数： 无
****************************************************************************/
void DisplyLayer(u8 layer);

//控件类指令 
/*********************************************
* 名    称： 	DisScreen()
* 功    能： 	切换到指定的画面ID显示
* 入口参数：	Screen_id(2 个字节)：画面编号           　　　　　           
* 返回格式： 	无
**************************************/
void DisScreen(u16 Screen_id);

/*********************************************
* 名    称： 	C_GetScreen()
* 功    能： 	获取当前显示画面的Screen_id值
* 入口参数：	无           　　　　　           
* 返回格式： 	指令返回格式：EE B1 01 Screen_id FF FC FF FF
		Screen_id(2 个字节)：当前画面的编号
**************************************/
void GetScreen(void);

/*********************************************
* 名    称： 	C_SetButton()
* 功    能： 	更新触控按钮状态
* 入口参数：	将按下或弹起的按钮设置为弹起或按下状态
		Screen_id(2 个字节)：画面编号
		Control_id(2 个字节)：控件编号
		Status (1 个字节)：按钮状态
			0x00：按键由按下变成弹起状态
			0x01：按键从弹起变成按下状态              　　　　　           
* 返回格式： 	无
**************************************/
void SetButtonStatus(u16 Screen_id,u16 Control_id,u8 Status);


/*********************************************
* 名    称： 	C_GetButton()
* 功    能： 	获取触控按钮状态
* 入口参数：	查询某个触控按钮当前是按下还是弹起状态
		Screen_id(2 个字节)：画面编号
		Control_id(2 个字节)：控件编号             　　　　　           
* 返回格式： 	返回指令格式：EE B1 11 Screen_id   Control_id   Control_type   Subtype Status FF FC FF FF 
**************************************/
void GetButtonStatus(u16 Screen_id,u16 Control_id);

/****************************************************************************
* 名    称：	C_SetTextView(u16 Screen_id,u16 Control_id,char *string)  
* 功    能： 	更新文本控件数值
* 入口参数：   	对指定的文本控件写入显示的数值
		Screen_id(2个字节)：画面编号 
		Control_id(2个字节)：控件编号 
		Strings（不定长）： 用户写入的文本数值；   
* 返回格式： 	无
****************************************************************************/
void SetTextView(u16 Screen_id,u16 Control_id,char *string); 

/****************************************************************************
* 名    称： 	C_GetTextView(u16 Screen_id,u16 Control_id) 
* 功    能： 	获取文本控件
* 入口参数：  	Screen_id(2个字节)：画面编号 
		Control_id(2个字节)：控件编号   
* 返回格式： 	返回指令格式：EE B1 11 Screen_id Control_id Control_type Strings 00 FF FC FF FF
		返回参数：Control_type(1个字节)：固定值0x11，表示为文本控件类型
		Strings（不定长）：当前显示的文本值
		00：保留位
****************************************************************************/
void GetTextView(u16 Screen_id,u16 Control_id); 


/****************************************************************************
* 名    称： 	C_SetTextViewFocus(u16 Screen_id,u16 Control_id,u8 Focus) 
* 功    能： 	设置文本控件焦点
* 入口参数：  	Screen_id(2个字节)：画面编号 
		Control_id(2个字节)：控件编号  
		Focus (1个字节):
			0x00:取消焦点
			0x01:设置焦点
* 返回格式： 	无
****************************************************************************/
void SetTextViewFocus(u16 Screen_id,u16 Control_id,u8 Focus); 

/*********************************************
* 名    称： 	C_SetProgressValue(u16 Screen_id,u16 Control_id,u16 Progressvalue_H,u16 Progressvalue_L)
* 功    能： 	更新进度条控件数值
* 入口参数：	Screen_id(2个字节)：画面编号 
		Control_id(2个字节)：控件编号 
		Progressvalue（4个字节）：新的进度条值 通过高16位和低16位进行输入           　　　　　           
* 返回格式： 	无
**************************************/
void SetProgressValue(u16 Screen_id,u16 Control_id,u32 Progressvalue);

/*********************************************
* 名    称： 	C_GetProgressValue(u16 Screen_id,u16 Control_id)
* 功    能： 	读取进度条控件数值
* 入口参数：	Screen_id(2个字节)：画面编号 
		Control_id(2个字节)：控件编号            　　　　　           
* 返回格式： 	返回指令格式：EE B1 11 Screen_id Control_id Control_type Progressvalue FF FC FF FF
		返回参数：Control_type（1个字节）：固定值0x12，表示为进度条控件类型
		Progressvalue（4个字节）：当前进度条值
**************************************/
void GetProgressValue(u16 Screen_id,u16 Control_id);

/*********************************************
* 名    称： 	C_SetSlidervalue(u16 Screen_id,u16 Control_id,u32 Slidervalue)
* 功    能： 	更新滑块控件数值
* 入口参数：	Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号
		Slidervalue(4字节) ：新的滑块值                　　　　　           
* 返回格式： 	无
**************************************/
void SetSlidervalue(u16 Screen_id,u16 Control_id,u32 Slidervalue);

/*********************************************
* 名    称： 	C_GetSlidervalue(u16 Screen_id,u16 Control_id)
* 功    能： 	获取滑块控件数值
* 入口参数：	Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号          　　　　　           
* 返回格式： 	返回指令格式：EE B1 11 Screen_id Control_id Control_type Slidervalue FF FC FF FF
		返回参数：Control_type（1个字节）：固定值0x13，表示为滑动条控件类型
		Slidervalue（4个字节）：当前显示的滑动条值
**************************************/
void GetSlidervalue(u16 Screen_id,u16 Control_id);

/*********************************************
* 名    称： 	C_SetMeterValue(u16 Screen_id,u16 Control_id,u32 MeterValue)
* 功    能： 	更新仪表控件数值
* 入口参数：	Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号
		MeterValue(4字节) ：新的仪表值         　　　　　           
* 返回格式： 	无
**************************************/
void SetMeterValue(u16 Screen_id,u16 Control_id,u32 MeterValue);

/*********************************************
* 名    称： 	C_GetMeterValue(u16 Screen_id,u16 Control_id)
* 功    能： 	读取仪表控件数值
* 入口参数：	Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号              　　　　　           
* 返回格式： 	返回指令格式：EE B1 11 Screen_id Control_id Control_type Metervalue FF FC FF FF
		返回参数：Control_type（1个字节）：固定值0x14，表示为仪表控件类型
		Metervalue（4个字节）：当前显示的仪表值
		注：返回参数的意义同更新仪表控件数值
**************************************/
void GetMeterValue(u16 Screen_id,u16 Control_id);

/*********************************************
* 名    称： 	C_AnimationControl(u8 Cmd,u16 Screen_id,u16 Control_id)
* 功    能： 	动画控制
* 入口参数：	Cmd（1 个字节）：
			0x20 启动
			0x21 停止
			0x22 暂停
			0x23 上一帧
			0x24 下一帧
		Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号              　　　　　           
* 返回格式： 	无
**************************************/
void AnimationControl(u8 Cmd,u16 Screen_id,u16 Control_id);

/*********************************************
* 名    称： 	C_AnimationControlFrame(u16 Screen_id,u16 Control_id,u8 Pic_ID)
* 功    能： 	播放指定帧
* 入口参数：	Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号
		Pic_ID	:动画的帧ID		            　　　　　           
* 返回格式： 	无
**************************************/
void AnimationControlFrame(u16 Screen_id,u16 Control_id,u8 Pic_ID);

/*********************************************
* 名    称： 	C_AddCurve(u16 Screen_id,u16 Control_id,u8 Cmd,u16 Color)
* 功    能： 	添加曲线
* 入口参数：	screen_id(2个字节)：画面编号 
		control_id(2个字节)：控件编号
		cmd(1个字节)：对应曲线通道
		Color（2个字节）：曲线显示颜色                 　　　　　           
* 返回格式： 	无
**************************************/
void AddCurve(u16 Screen_id,u16 Control_id,u8 Cmd,u16 Color);

/*********************************************
* 名    称： 	C_DelCurve(u16 Screen_id,u16 Control_id,u8 Cmd)
* 功    能： 	删除曲线
* 入口参数：	screen_id(2个字节)：画面编号 
		control_id(2个字节)：控件编号
		cmd(1个字节)：对应曲线通道                　　　　　           
* 返回格式： 	无
**************************************/
void DelCurve(u16 Screen_id,u16 Control_id,u8 Cmd);


/*********************************************
* 名    称： 	C_AddCurveData()
* 功    能： 	添加曲线数据
* 入口参数：	screen_id(2个字节)：画面编号 
		control_id(2个字节)：控件编号
		cmd(1个字节)：对应曲线通道
		Lendth（2个字节）：数据长度	 
		Data：（1个字节）数据，可多个数据同时输入     　　　　　           
* 返回格式： 	无
**************************************/
void AddCurveData(u16 Screen_id,u16 Control_id,u8 Cmd,u16 Lendth,u8 Data);


/*********************************************
* 名    称： 	C_DelCurveData(u16 Screen_id,u16 Control_id,u8 Cmd)	
* 功    能： 	删除曲线数据
* 入口参数：	screen_id(2个字节)：画面编号 
		control_id(2个字节)：控件编号
		cmd(1个字节)：对应曲线通道                　　　　　           
* 返回格式： 	无
**************************************/
void DelCurveData(u16 Screen_id,u16 Control_id,u8 Cmd);

/*********************************************
* 名    称： 	C_ZoomShift()
* 功    能： 	曲线缩放与偏移
* 入口参数：	screen_id(2个字节)：画面编号 
		control_id(2个字节)：控件编号
		L_shift(2个字节)：水平偏移
		L_zoom(2个字节)：水平缩放
		V_shift(2个字节)：垂直偏移
		V_zoom(2个字节)：垂直缩放              　　　　　           
* 返回格式：	无
**************************************/
void  ZoomShift(u16 Screen_id,u16 Control_id,u16 L_shift,u16 L_zoom,u16 V_shift,u16 V_zoom);

#endif 


