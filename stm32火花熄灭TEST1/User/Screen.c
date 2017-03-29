

#include "Screen.h"
#include "stm32f10x_lib.h"
/*****************************************************************
* 名    称： SendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节       
* 出口参数： 无                  
 *****************************************************************/
void  SendChar(u8 t) 
{	
	USART_SendData(USART2,t); 
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);   
}

/*****************************************************************
* 名    称： SendStrings()
* 功    能： 发送字符串
* 入口参数： str  发送的字符串     
* 出口参数： 无               
 *****************************************************************/
void SendStrings(u8 *str)
{
    while(*str)
	{
		SendChar(*str);
		str++;
	}
} 

/****************************************************************************
* 名    称： SendBeginCMD()
* 功    能： 发送帧头命令
* 入口参数： 无      
* 出口参数： 无
****************************************************************************/
void SendBeginCMD()
{
	SendChar(0xEE);
}

/****************************************************************************
* 名    称： SendEndCmd()
* 功    能： 发送帧尾命令
* 入口参数： 无      
* 出口参数： 无
****************************************************************************/
void SendEndCmd()
{
	SendChar(0xFF);
	SendChar(0xFC);
	SendChar(0xFF);
	SendChar(0xFF);
}

/****************************************************************************
* 名    称： SetHandShake()
* 功    能： 握手
* 入口参数： 无      
* 出口参数： 无
****************************************************************************/
void SetHandShake(void)
{
	SendBeginCMD();
	SendChar(0x00);
	SendEndCmd();
}

/****************************************************************************
* 名    称： SetFcolor()
* 功    能： 设置前景色
* 入口参数： fcolor  前景色RGB参数   
* 出口参数： 无
****************************************************************************/
void SetFcolor(u16 fcolor)
{
	SendBeginCMD();
	SendChar(0x41);
	SendChar((fcolor>>8)&0xff);
	SendChar(fcolor&0xff);
	SendEndCmd();
}

/****************************************************************************
* 名    称： SetBcolor()
* 功    能： 设置背景色
* 入口参数： bcolor  背景色RGB参数   
* 出口参数： 无
****************************************************************************/
void SetBcolor(u16 bcolor)
{
	SendBeginCMD();
	SendChar(0x42);
	SendChar((bcolor>>8)&0xff);
	SendChar(bcolor&0xff);
	SendEndCmd();
}

/****************************************************************************
* 名    称： ColorPicker()
* 功    能： 取屏幕点设置前景色/背景色
* 入口参数： mode 取当前显示屏幕(X,Y)处的颜色作为前景/背景色
             x       以点为单位的X轴坐标值
             y       以点为单位的Y轴坐标值
* 出口参数： 无
****************************************************************************/
void ColorPicker(u8 mode, u16 x,u16 y)
{
	SendBeginCMD();
	SendChar(0xa3);
	SendChar(mode);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： GUI_CleanScreen()
* 功    能： 清屏
* 入口参数： 无  
* 出口参数： 无
****************************************************************************/
void GUI_CleanScreen()
{
	SendBeginCMD();
	SendChar(0x01);
	SendEndCmd();
}

/****************************************************************************
* 名    称： SetTextSpace()
* 功    能： 设置文字行列间距
* 入口参数： x_w     以点为单位的行间距，取值范围00~1F
             y_w     以点为单位的列间距，取值范围00~1F	
* 出口参数： 无
****************************************************************************/
void SetTextSpace(u8 x_w, u8 y_w)
{
	SendBeginCMD();
	SendChar(0x43);
	SendChar(x_w);
	SendChar(y_w);
	SendEndCmd();
}
/****************************************************************************
* 名    称： DisText_Region()
* 功    能： 限定文本显示范围
* 入口参数： x       以点为单位的X轴坐标值
             y       以点为单位的Y轴坐标值
             length  文本框限定的长度
	         width   文本框限定的高度
* 出口参数： 无
****************************************************************************/
void SetFont_Region(u8 enable,u16 length,u16 width )
{
	SendBeginCMD();
	SendChar(0x45);
	SendChar(enable);
	SendChar((length>>8)&0xff);
	SendChar(length&0xff);
	SendChar((width>>8)&0xff);
	SendChar(width&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： SetFilterColor()
* 功    能： 设置过滤颜色
* 入口参数： fillcolor_dwon   滤除颜色的下限值
             fillcolor_up     滤除颜色的上限值	
* 出口参数： 无
****************************************************************************/
void SetFilterColor(u16 fillcolor_dwon, u16 fillcolor_up)
{
	SendBeginCMD();
	SendChar(0x44);
	SendChar((fillcolor_dwon>>8)&0xff);
	SendChar(fillcolor_dwon&0xff);
	SendChar((fillcolor_up>>8)&0xff);
	SendChar(fillcolor_up&0xff);
	SendEndCmd();
}
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
void DisText(u16 x, u16 y,u8 back,u8 font,u8 *strings )
{
	SendBeginCMD();
	SendChar(0x20);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar(back);
	SendChar(font);
	SendStrings(strings);
	SendEndCmd();
}
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
void DisData(u16 x, u16 y,u8 back,u8 font,u8 sdata )
{
	SendBeginCMD();
	SendChar(0x20);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar(back);
	SendChar(font);
	SendChar(sdata);
	SendEndCmd();
}
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
void DisCursor(u8 enable,u16 x, u16 y,u8 length,u8 width )
{
	SendBeginCMD();
	SendChar(0x21);
	SendChar(enable);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar(length&0xff);
	SendChar(width&0xff);
	SendEndCmd();
}

/****************************************************************************
* 名    称： DisFull_Image()
* 功    能： 全屏整幅图显示
* 入口参数： image_id   图片ID编号
             masken     屏蔽色使能
* 出口参数： 无
****************************************************************************/
void DisFull_Image(u16 image_id,u8 masken)
{
	SendBeginCMD();
	SendChar(0x31);
	SendChar((image_id>>8)&0xff);
	SendChar(image_id&0xff); 
	SendChar(masken);
	SendEndCmd();
}

/****************************************************************************
* 名    称： DisArea_Image()
* 功    能： 区域图片显示
* 入口参数： x          以点为单位的X轴坐标值
             y          以点为单位的Y轴坐标值 
             image_id   图片ID编号
             masken     屏蔽色使能
* 出口参数： 无
****************************************************************************/
void DisArea_Image(u16 x,u16 y,u16 image_id,u8 masken)
{
	SendBeginCMD();
	SendChar(0x32);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar((image_id>>8)&0xff);
	SendChar(image_id&0xff);
	SendChar(masken);
	SendEndCmd();
}

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
                   u16 image_l, u16 image_w,u8 masken)
{
	SendBeginCMD();
	SendChar(0x33);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar((image_id>>8)&0xff);
	SendChar(image_id&0xff);
	SendChar((image_x>>8)&0xff);
	SendChar(image_x&0xff);
	SendChar((image_y>>8)&0xff);
	SendChar(image_y&0xff);
	SendChar((image_l>>8)&0xff);
	SendChar(image_l&0xff);
	SendChar((image_w>>8)&0xff);
	SendChar(image_w&0xff);
	SendChar(masken);
	SendEndCmd();
}
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
void DisFlashImage(u16 x,u16 y,u16 flashimage_id,u8 enable,u8 playnum)
{
	SendBeginCMD();
	SendChar(0x80);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar((flashimage_id>>8)&0xff);
	SendChar(flashimage_id&0xff);
	SendChar(enable);
	SendChar(playnum);
	SendEndCmd();
}

/****************************************************************************
* 名    称： GUI_Dot()
* 功    能： 画点
* 入口参数： x       以点为单位的X轴坐标值
             y       以点为单位的Y轴坐标值           
* 出口参数： 无
****************************************************************************/
void GUI_Dot(u16 x,u16 y)
{
	SendBeginCMD();
	SendChar(0x50);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendEndCmd();
}

/****************************************************************************
* 名    称： GUI_Line()
* 功    能： 画线
* 入口参数： x0      以点为单位的X轴坐标值
             y0      以点为单位的Y轴坐标值    
             x1      以点为单位的X轴坐标值
             y1      以点为单位的Y轴坐标值  
* 出口参数： 无
****************************************************************************/
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1)
{
	SendBeginCMD();
	SendChar(0x51);
	SendChar((x0>>8)&0xff);
	SendChar(x0&0xff);
	SendChar((y0>>8)&0xff);
	SendChar(y0&0xff);
	SendChar((x1>>8)&0xff);
	SendChar(x1&0xff);
	SendChar((y1>>8)&0xff);
	SendChar(y1&0xff);
	SendEndCmd();
}

/****************************************************************************
* 名    称： GUI_ConDots()
* 功    能： 绘制折线
* 入口参数： mode     0:用前景色描绘 1：用背景色描绘    
	         pDot     指向点的指针   
* 出口参数： 无
****************************************************************************/
void GUI_ConDots(u8 mode,u8 *pDot)
{
	SendBeginCMD();
	SendChar(0x63);
	SendChar(mode);
	SendStrings(pDot); 
	SendEndCmd();
}
/****************************************************************************
* 名    称： GUI_Circle()
* 功    能： 画空心圆
* 入口参数： x               以点为单位的X轴坐标值
             y               以点为单位的Y轴坐标值    
             r                空心圆的半径   
* 出口参数： 无
****************************************************************************/
void GUI_Circle(u16 x, u16 y, u16 r)
{
	SendBeginCMD();
	SendChar(0x52);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar((r>>8)&0xff);
	SendChar(r&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： GUI_CircleFill()
* 功    能： 画实心圆
* 入口参数： x               以点为单位的X轴坐标值
             y               以点为单位的Y轴坐标值    
             r                实心圆的半径   
* 出口参数： 无
****************************************************************************/
void GUI_CircleFill(u16 x, u16 y, u16 r)
{
	SendBeginCMD();
	SendChar(0x53);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar((r>>8)&0xff);
	SendChar(r&0xff);
	SendEndCmd();
}
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
void GUI_Arc(u16 x,u16 y, u16 r,u16 sa, u16 ea)
{
	SendBeginCMD();
	SendChar(0x67);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar((r>>8)&0xff);
	SendChar(r&0xff);
	SendChar((sa>>8)&0xff);
	SendChar(sa&0xff);
	SendChar((ea>>8)&0xff);
	SendChar(ea&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： GUI_Rectangle()
* 功    能： 画空心矩形
* 入口参数： x0      以点为单位的空心矩形左上角X坐标值
             y0      以点为单位的空心矩形左上角Y坐标值   
             x1      以点为单位的空心矩形右下角X坐标值
             y1      以点为单位的空心矩形右下角Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_Rectangle(u16 x0, u16 y0, u16 x1,u16 y1 )
{
	SendBeginCMD();
	SendChar(0x54);
	SendChar((x0>>8)&0xff);
	SendChar(x0&0xff);
	SendChar((y0>>8)&0xff);
	SendChar(y0&0xff);
	SendChar((x1>>8)&0xff);
	SendChar(x1&0xff);
	SendChar((y1>>8)&0xff);
	SendChar(y1&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： RectangleFill()
* 功    能： 画实心矩形
* 入口参数： x0      以点为单位的实心矩形左上角X坐标值
             y0      以点为单位的实心矩形左上角Y坐标值   
             x1      以点为单位的实心矩形右下角X坐标值
             y1      以点为单位的实心矩形右下角Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_RectangleFill(u16 x0, u16 y0, u16 x1,u16 y1 )
{
	SendBeginCMD();
	SendChar(0x55);
	SendChar((x0>>8)&0xff);
	SendChar(x0&0xff);
	SendChar((y0>>8)&0xff);
	SendChar(y0&0xff);
	SendChar((x1>>8)&0xff);
	SendChar(x1&0xff);
	SendChar((y1>>8)&0xff);
	SendChar(y1&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： GUI_AreaInycolor()
* 功    能： 屏幕区域反色
* 入口参数： x0      以点为单位的实心矩形左上角X坐标值
             y0      以点为单位的实心矩形左上角Y坐标值   
             x1      以点为单位的实心矩形右下角X坐标值
             y1      以点为单位的实心矩形右下角Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_AreaInycolor(u16 x0, u16 y0, u16 x1,u16 y1 )
{
	SendBeginCMD();
	SendChar(0x65);
	SendChar((x0>>8)&0xff);
	SendChar(x0&0xff);
	SendChar((y0>>8)&0xff);
	SendChar(y0&0xff);
	SendChar((x1>>8)&0xff);
	SendChar(x1&0xff);
	SendChar((y1>>8)&0xff);
	SendChar(y1&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： GUI_Ellipse()
* 功    能： 画空心椭圆
* 入口参数： x0      以点为单位的空心椭圆最左端X坐标值
             y0      以点为单位的空心椭圆最上端Y坐标值   
             x1      以点为单位的空心椭圆最右端X坐标值
             y1      以点为单位的空心椭圆最下端Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_Ellipse (u16 x0, u16 y0, u16 x1,u16 y1 )
{
	SendBeginCMD();
	SendChar(0x56);
	SendChar((x0>>8)&0xff);
	SendChar(x0&0xff);
	SendChar((y0>>8)&0xff);
	SendChar(y0&0xff);
	SendChar((x1>>8)&0xff);
	SendChar(x1&0xff);
	SendChar((y1>>8)&0xff);
	SendChar(y1&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： GUI_EllipseFill()
* 功    能： 画实心椭圆
* 入口参数： x0      以点为单位的实心椭圆最左端X坐标值
             y0      以点为单位的实心椭圆最上端Y坐标值   
             x1      以点为单位的实心椭圆最右端X坐标值
             y1      以点为单位的实心椭圆最下端Y坐标值   
* 出口参数： 无
****************************************************************************/
void GUI_EllipseFill (u16 x0, u16 y0, u16 x1,u16 y1 )
{
	SendBeginCMD();
	SendChar(0x57);
	SendChar((x0>>8)&0xff);
	SendChar(x0&0xff);
	SendChar((y0>>8)&0xff);
	SendChar(y0&0xff);
	SendChar((x1>>8)&0xff);
	SendChar(x1&0xff);
	SendChar((y1>>8)&0xff);
	SendChar(y1&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： GUI_PolygonFill()
* 功    能： 多边形填充
* 入口参数： x               以点为单位的X轴坐标值
             y               以点为单位的Y轴坐标值    
             color            填充颜色   
* 出口参数： 无
****************************************************************************/
void GUI_PolygonFill(u16 x, u16 y, u16 color)
{
	SendBeginCMD();
	SendChar(0x64);
	SendChar((x>>8)&0xff);
	SendChar(x&0xff);
	SendChar((y>>8)&0xff);
	SendChar(y&0xff);
	SendChar((color>>8)&0xff);
	SendChar(color&0xff);
	SendEndCmd();
}
/****************************************************************************
* 名    称： SetBackLight()
* 功    能： 背光调节
* 入口参数： light_level
* 出口参数： 无
****************************************************************************/
void SetBackLight(u8 light_level)
{
	SendBeginCMD();
	SendChar(0x60);
	SendChar(light_level);
	SendEndCmd();
}
/****************************************************************************
* 名    称： SetBuzzer()
* 功    能： 蜂鸣器控制
* 入口参数： light_level
* 出口参数： 无
****************************************************************************/
void SetBuzzer(u8 time)
{
	SendBeginCMD();
	SendChar(0x61);
	SendChar(time);
	SendEndCmd();
}
/****************************************************************************
* 名    称： SetCommBps()
* 功    能： 设置通讯波特率
* 入口参数： Baud
* 出口参数： 无
****************************************************************************/
void SetCommBps(u8 baudset)  
{
	SendBeginCMD();
	SendChar(0xA0);
	SendChar(baudset);
	SendEndCmd();
}
/****************************************************************************
* 名    称： SetTouchScreen(u8 cmd)
* 功    能： 触摸屏控制
* 入口参数： cmd   Bit0:1/0 触摸开/闭; Bit1 : 1/0 蜂鸣器开/闭; Bit2: 上传方式  
* 出口参数： 无
****************************************************************************/
void SetTouchScreen(u8 cmd)
{
	SendBeginCMD();
	SendChar(0x70);
	SendChar(cmd);
	SendEndCmd();
}
/****************************************************************************
* 名    称： SetTouchScreen_Adj()
* 功    能： 触摸屏校准
* 入口参数： 无
* 出口参数： 无
****************************************************************************/
void SetTouchScreen_Adj()
{
	SendBeginCMD();
	SendChar(0x72);
	SendEndCmd();
}
/****************************************************************************
* 名    称： TestTouchScreen()
* 功    能： 触摸屏体验
* 入口参数： 无
* 出口参数： 无
****************************************************************************/
void TestTouchScreen()
{
	SendBeginCMD();
	SendChar(0x73);
	SendEndCmd();
}

/****************************************************************************
* 名    称： WriteLayer()
* 功    能： 写图层操作
* 入口参数： layer         写入的图层数
* 出口参数： 无
****************************************************************************/
void WriteLayer(u8 layer)
{
	SendBeginCMD();
	SendChar(0xa1);
	SendChar(layer);
	SendEndCmd();
}
/****************************************************************************
* 名    称： WriteLayer()
* 功    能： 切换图层显示
* 入口参数： layer         切换显示的图层
* 出口参数： 无
****************************************************************************/
void DisplyLayer(u8 layer)
{
	SendBeginCMD();
	SendChar(0xa2);
	SendChar(layer);
	SendEndCmd();
}

/****************************************************************************
* 名    称： 	C_CopyLayer(uchar src_layer,uchar dest_layer)
* 功    能：	将任意图层拷贝到另外一个图层，并自动进行叠加显示
* 入口参数：	src_layer (1 个字节)：源图层
		dest_layer (1 个字节)：目标图层   
* 返回格式： 	无
****************************************************************************/
void CopyLayer(u8 src_layer,u8 dest_layer)
{
	SendBeginCMD();
	SendChar(0xa4);
	SendChar(src_layer);
	SendChar(dest_layer);
	SendEndCmd();
}

/****************************************************************************
* 名    称： 	C_ClearLayer(uchar layer)
* 功    能： 	清除某个图层内容
* 入口参数：	Layer(1 个字节)：写入的图层   
* 返回格式： 	无
****************************************************************************/
void ClearLayer(u8 layer)
{
	SendBeginCMD();
	SendChar(0x05);
	SendChar(layer);
	SendEndCmd();
}

/****************************************************************************
* 名    称： 	C_AutoClearLayer_en(uchar en)
* 功    能： 	设置切换画面时是否需要自动清除当前用户层
* 入口参数：	En（1 个字节） ：使能信号
		0x01：自动清除图层
		0x00：禁止清除图层 
* 返回格式： 	无
****************************************************************************/
void AutoClearLayer_en(u8 en)
{
	SendBeginCMD();
	SendChar(0x06);
	SendChar(en);
	SendEndCmd();
}


//控件类指令 
/*********************************************
* 名    称： 	DisScreen()
* 功    能： 	切换到指定的画面ID显示
* 入口参数：	Screen_id(2 个字节)：画面编号           　　　　　           
* 返回格式： 	无
**************************************/
void 	DisScreen(u16 Screen_id)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x00);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendEndCmd();
}

/*********************************************
* 名    称： 	C_GetScreen()
* 功    能： 	获取当前显示画面的Screen_id值
* 入口参数：	无           　　　　　           
* 返回格式： 	指令返回格式：EE B1 01 Screen_id FF FC FF FF
		Screen_id(2 个字节)：当前画面的编号
**************************************/
void 	GetScreen(void)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x01);
	SendEndCmd();
}

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
void SetButtonStatus(u16 Screen_id,u16 Control_id,u8 Status)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x10);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(Status);
	SendEndCmd();
}

/*********************************************
* 名    称： 	C_GetButton()
* 功    能： 	获取触控按钮状态
* 入口参数：	查询某个触控按钮当前是按下还是弹起状态
		Screen_id(2 个字节)：画面编号
		Control_id(2 个字节)：控件编号             　　　　　           
* 返回格式： 	返回指令格式：EE B1 11 Screen_id   Control_id   Control_type   Subtype Status FF FC FF FF 
**************************************/
void 	GetButtonStatus(u16 Screen_id,u16 Control_id)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x11);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendEndCmd();
}

/****************************************************************************
* 名    称：	C_SetTextView(u16 Screen_id,u16 Control_id,char *string)  
* 功    能： 	更新文本控件数值
* 入口参数：   	对指定的文本控件写入显示的数值
		Screen_id(2个字节)：画面编号 
		Control_id(2个字节)：控件编号 
		Strings（不定长）： 用户写入的文本数值；   
* 返回格式： 	无
****************************************************************************/
void 	SetTextView(u16 Screen_id,u16 Control_id,char *string) 
{
	SendBeginCMD();
	SendChar(0xb1);
	SendChar(0x10);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	do
	{
		SendChar(*string);
	}
	while(*string++);
	SendEndCmd();
}


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
void GetTextView(u16 Screen_id,u16 Control_id) 
{
	SendBeginCMD();
	SendChar(0xb1);
	SendChar(0x11);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendEndCmd();
}

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
void 	SetTextViewFocus(u16 Screen_id,u16 Control_id,u8 Focus) 
{
	SendBeginCMD();
	SendChar(0xb1);
	SendChar(0x02);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(Focus);
	SendEndCmd();
}

/*********************************************
* 名    称： 	C_SetProgressValue(u16 Screen_id,u16 Control_id,u16 Progressvalue_H,u16 Progressvalue_L)
* 功    能： 	更新进度条控件数值
* 入口参数：	Screen_id(2个字节)：画面编号 
		Control_id(2个字节)：控件编号 
		Progressvalue（4个字节）：新的进度条值 通过高16位和低16位进行输入           　　　　　           
* 返回格式： 	无
**************************************/
void 	SetProgressValue(u16 Screen_id,u16 Control_id,u32 Progressvalue)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x10);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(Progressvalue>>24);
	SendChar(Progressvalue>>16);
	SendChar(Progressvalue>>8);
	SendChar(Progressvalue);
	SendEndCmd();
}

/*********************************************
* 名    称： 	C_GetProgressValue(u16 Screen_id,u16 Control_id)
* 功    能： 	读取进度条控件数值
* 入口参数：	Screen_id(2个字节)：画面编号 
		Control_id(2个字节)：控件编号            　　　　　           
* 返回格式： 	返回指令格式：EE B1 11 Screen_id Control_id Control_type Progressvalue FF FC FF FF
		返回参数：Control_type（1个字节）：固定值0x12，表示为进度条控件类型
		Progressvalue（4个字节）：当前进度条值
**************************************/
void 	GetProgressValue(u16 Screen_id,u16 Control_id)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x11);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendEndCmd();
}

/*********************************************
* 名    称： 	C_SetSlidervalue(u16 Screen_id,u16 Control_id,u32 Slidervalue)
* 功    能： 	更新滑块控件数值
* 入口参数：	Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号
		Slidervalue(4字节) ：新的滑块值                　　　　　           
* 返回格式： 	无
**************************************/
void SetSlidervalue(u16 Screen_id,u16 Control_id,u32 Slidervalue)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x10);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(Slidervalue>>24);
	SendChar(Slidervalue>>16);
	SendChar(Slidervalue>>8);
	SendChar(Slidervalue);
	SendEndCmd();
}


/*********************************************
* 名    称： 	C_GetSlidervalue(u16 Screen_id,u16 Control_id)
* 功    能： 	获取滑块控件数值
* 入口参数：	Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号          　　　　　           
* 返回格式： 	返回指令格式：EE B1 11 Screen_id Control_id Control_type Slidervalue FF FC FF FF
		返回参数：Control_type（1个字节）：固定值0x13，表示为滑动条控件类型
		Slidervalue（4个字节）：当前显示的滑动条值
**************************************/
void GetSlidervalue(u16 Screen_id,u16 Control_id)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x11);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendEndCmd();
}
/*********************************************
* 名    称： 	C_SetMeterValue(u16 Screen_id,u16 Control_id,u32 MeterValue)
* 功    能： 	更新仪表控件数值
* 入口参数：	Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号
		MeterValue(4字节) ：新的仪表值         　　　　　           
* 返回格式： 	无
**************************************/
void SetMeterValue(u16 Screen_id,u16 Control_id,u32 MeterValue)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x10);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(MeterValue>>24);
	SendChar(MeterValue>>16);
	SendChar(MeterValue>>8);
	SendChar(MeterValue);
	SendEndCmd();
}

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
void GetMeterValue(u16 Screen_id,u16 Control_id)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x11);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendEndCmd();
}

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
void AnimationControl(u8 Cmd,u16 Screen_id,u16 Control_id)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(Cmd);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendEndCmd();	
}

/*********************************************
* 名    称： 	C_AnimationControlFrame(u16 Screen_id,u16 Control_id,u8 Pic_ID)
* 功    能： 	播放指定帧
* 入口参数：	Screen_id(2字节) ：画面编号
		Control_id(2字节) ：控件编号
		Pic_ID	:动画的帧ID		            　　　　　           
* 返回格式： 	无
**************************************/
void AnimationControlFrame(u16 Screen_id,u16 Control_id,u8 Pic_ID)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x23);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(Pic_ID);
	SendEndCmd();	
	
}

/*********************************************
* 名    称： 	C_AddCurve(u16 Screen_id,u16 Control_id,u8 Cmd,u16 Color)
* 功    能： 	添加曲线
* 入口参数：	screen_id(2个字节)：画面编号 
		control_id(2个字节)：控件编号
		cmd(1个字节)：对应曲线通道
		Color（2个字节）：曲线显示颜色                 　　　　　           
* 返回格式： 	无
**************************************/
void AddCurve(u16 Screen_id,u16 Control_id,u8 Cmd,u16 Color)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x30);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(Cmd);
	SendChar(Color>>8);
	SendChar(Color);
	SendEndCmd();
}

/*********************************************
* 名    称： 	C_DelCurve(u16 Screen_id,u16 Control_id,u8 Cmd)
* 功    能： 	删除曲线
* 入口参数：	screen_id(2个字节)：画面编号 
		control_id(2个字节)：控件编号
		cmd(1个字节)：对应曲线通道                　　　　　           
* 返回格式： 	无
**************************************/
void DelCurve(u16 Screen_id,u16 Control_id,u8 Cmd)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x31);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(Cmd);
	SendEndCmd();
}


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
void AddCurveData(u16 Screen_id,u16 Control_id,u8 Cmd,u16 Lendth,u8 Data)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x32);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(Cmd);
	SendChar(Lendth>>8);
	SendChar(Lendth);
	SendChar(Data);
	SendEndCmd();
}

/*********************************************
* 名    称： 	C_DelCurveData(u16 Screen_id,u16 Control_id,u8 Cmd)	
* 功    能： 	删除曲线数据
* 入口参数：	screen_id(2个字节)：画面编号 
		control_id(2个字节)：控件编号
		cmd(1个字节)：对应曲线通道                　　　　　           
* 返回格式： 	无
**************************************/
void DelCurveData(u16 Screen_id,u16 Control_id,u8 Cmd)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x33);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(Cmd);
	SendEndCmd();
}

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
void  ZoomShift(u16 Screen_id,u16 Control_id,u16 L_shift,u16 L_zoom,u16 V_shift,u16 V_zoom)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x34);
	SendChar(Screen_id>>8);
	SendChar(Screen_id);
	SendChar(Control_id>>8);
	SendChar(Control_id);
	SendChar(L_shift>>8);
	SendChar(L_shift);
	SendChar(L_zoom>>8);
	SendChar(L_zoom);
	SendChar(V_shift>>8);
	SendChar(V_shift);
	SendChar(V_zoom>>8);
	SendChar(V_zoom);
	SendEndCmd();
}









/****************************************************************************
* 名    称： 	C_DisTime(u8 DisTimeEN,u8 DisTimeMode,u8 DisTimeFont,u16 DisTimeColor,u16 DisTimeX,u16 DisTimeY)
* 功    能： 	RTC显示设置
* 入口参数：	DisTimeEN:使能信号
		0 : RTC 关闭	1：RTC 开启
		DisMode(1 个字节)：显示模式
			0x00 : 格式 HH:MM:SS
			0x01 : 格式 20XX-MM-DD HH:MM:SS
		DisTimeFont (1 个字节) ：字体选择
			0x00 : 0 号字体
			0x01 : 1 号字体
			0x02 : 2 号字体
			0x03 : 3 号字体
		DisTimeColor(2 个字节) : 显示颜色
		DisTimeX (2 个字节): 以点为单位的 X 轴坐标值
		DisTimeY (2 个字节): 以点为单位的 Y 轴坐标值
* 返回格式： 	无
****************************************************************************/
void 	DisTime(u8 DisTimeEN,u8 DisTimeMode,u8 DisTimeFont,u16 DisTimeColor,u16 DisTimeX,u16 DisTimeY)
{			   
	SendBeginCMD();
	SendChar(0x85);
	SendChar(DisTimeEN);
	SendChar(DisTimeMode);
	SendChar(DisTimeFont);
	SendChar(DisTimeColor>>8);
	SendChar(DisTimeColor);
	SendChar(DisTimeX>>8);
	SendChar(DisTimeX);
	SendChar(DisTimeY>>8);
	SendChar(DisTimeY);
	SendEndCmd();
}

/****************************************************************************
* 名    称： 	C_SetTime(u8 sec,u8 min,u8 hour,u8 day,u8 week,u8 mon,u8 year)
* 功    能： 	时间参数设定
* 入口参数：	Sec:秒设置	Min: 分设置；	Hour:小时设置	Day: 日设置
		Week:星期设置	Mon: 月设置	Year:年设置
		建议用户直接使用时钟控件来完成 RTC 的显示和时间的校准。
		备注：各 1 个字节，以 BCD 码表示，星期天设置为 0x00
* 返回格式： 	无
****************************************************************************/
void 	SetTime(u8 sec,u8 min,u8 hour,u8 day,u8 week,u8 mon,u8 year)
{
	SendBeginCMD();
	SendChar(0x85);
	SendChar(sec);
	SendChar(min);
	SendChar(hour);
	SendChar(day);
	SendChar(week);
	SendChar(mon);
	SendChar(year);	
	SendEndCmd();
}

/****************************************************************************
* 名    称： 	C_ReadTime(void)
* 功    能： 	读取RTC时钟
* 入口参数：	无
* 返回格式： 	数据输出格式：EE +0xF7 +Year +Mon +Week +Day +Hour +Min +Sec +FF FC FF FF
****************************************************************************/
void 	ReadTime(void)
{
	SendBeginCMD();
	SendChar(0x81);	
	SendEndCmd();
}


 