

#include "Screen.h"
#include "stm32f10x_lib.h"
/*****************************************************************
* ��    �ƣ� SendChar()
* ��    �ܣ� ����1���ֽ�
* ��ڲ����� t  ���͵��ֽ�       
* ���ڲ����� ��                  
 *****************************************************************/
void  SendChar(u8 t) 
{	
	USART_SendData(USART2,t); 
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);   
}

/*****************************************************************
* ��    �ƣ� SendStrings()
* ��    �ܣ� �����ַ���
* ��ڲ����� str  ���͵��ַ���     
* ���ڲ����� ��               
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
* ��    �ƣ� SendBeginCMD()
* ��    �ܣ� ����֡ͷ����
* ��ڲ����� ��      
* ���ڲ����� ��
****************************************************************************/
void SendBeginCMD()
{
	SendChar(0xEE);
}

/****************************************************************************
* ��    �ƣ� SendEndCmd()
* ��    �ܣ� ����֡β����
* ��ڲ����� ��      
* ���ڲ����� ��
****************************************************************************/
void SendEndCmd()
{
	SendChar(0xFF);
	SendChar(0xFC);
	SendChar(0xFF);
	SendChar(0xFF);
}

/****************************************************************************
* ��    �ƣ� SetHandShake()
* ��    �ܣ� ����
* ��ڲ����� ��      
* ���ڲ����� ��
****************************************************************************/
void SetHandShake(void)
{
	SendBeginCMD();
	SendChar(0x00);
	SendEndCmd();
}

/****************************************************************************
* ��    �ƣ� SetFcolor()
* ��    �ܣ� ����ǰ��ɫ
* ��ڲ����� fcolor  ǰ��ɫRGB����   
* ���ڲ����� ��
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
* ��    �ƣ� SetBcolor()
* ��    �ܣ� ���ñ���ɫ
* ��ڲ����� bcolor  ����ɫRGB����   
* ���ڲ����� ��
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
* ��    �ƣ� ColorPicker()
* ��    �ܣ� ȡ��Ļ������ǰ��ɫ/����ɫ
* ��ڲ����� mode ȡ��ǰ��ʾ��Ļ(X,Y)������ɫ��Ϊǰ��/����ɫ
             x       �Ե�Ϊ��λ��X������ֵ
             y       �Ե�Ϊ��λ��Y������ֵ
* ���ڲ����� ��
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
* ��    �ƣ� GUI_CleanScreen()
* ��    �ܣ� ����
* ��ڲ����� ��  
* ���ڲ����� ��
****************************************************************************/
void GUI_CleanScreen()
{
	SendBeginCMD();
	SendChar(0x01);
	SendEndCmd();
}

/****************************************************************************
* ��    �ƣ� SetTextSpace()
* ��    �ܣ� �����������м��
* ��ڲ����� x_w     �Ե�Ϊ��λ���м�࣬ȡֵ��Χ00~1F
             y_w     �Ե�Ϊ��λ���м�࣬ȡֵ��Χ00~1F	
* ���ڲ����� ��
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
* ��    �ƣ� DisText_Region()
* ��    �ܣ� �޶��ı���ʾ��Χ
* ��ڲ����� x       �Ե�Ϊ��λ��X������ֵ
             y       �Ե�Ϊ��λ��Y������ֵ
             length  �ı����޶��ĳ���
	         width   �ı����޶��ĸ߶�
* ���ڲ����� ��
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
* ��    �ƣ� SetFilterColor()
* ��    �ܣ� ���ù�����ɫ
* ��ڲ����� fillcolor_dwon   �˳���ɫ������ֵ
             fillcolor_up     �˳���ɫ������ֵ	
* ���ڲ����� ��
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
* ��    �ƣ� DisText()
* ��    �ܣ� �ı���ʾ
* ��ڲ����� x         �Ե�Ϊ��λ��X������ֵ
             y         �Ե�Ϊ��λ��Y������ֵ
   	         back      ����ɫʹ��
	         font      �ֿ����
             strings   д����ַ���
* ���ڲ����� ��
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
* ��    �ƣ� DisData()
* ��    �ܣ� ������ʾ
* ��ڲ����� x       �Ե�Ϊ��λ��X������ֵ
             y       �Ե�Ϊ��λ��Y������ֵ
   	         back    ����ɫʹ��
	         font    �ֿ����
	         sdata   д�������
* ���ڲ����� ��
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
* ��    �ƣ� DisCursor()
* ��    �ܣ� �����ʾ
* ��ڲ����� enable
             x        �Ե�Ϊ��λ��X������ֵ
             y        �Ե�Ϊ��λ��Y������ֵ
             length   ��곤��
	         width    �����
* ���ڲ����� ��
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
* ��    �ƣ� DisFull_Image()
* ��    �ܣ� ȫ������ͼ��ʾ
* ��ڲ����� image_id   ͼƬID���
             masken     ����ɫʹ��
* ���ڲ����� ��
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
* ��    �ƣ� DisArea_Image()
* ��    �ܣ� ����ͼƬ��ʾ
* ��ڲ����� x          �Ե�Ϊ��λ��X������ֵ
             y          �Ե�Ϊ��λ��Y������ֵ 
             image_id   ͼƬID���
             masken     ����ɫʹ��
* ���ڲ����� ��
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
* ��    �ƣ� DisCut_Image()
* ��    �ܣ� ͼƬ����
* ��ڲ����� x          �Ե�Ϊ��λ��X������ֵ
             y          �Ե�Ϊ��λ��Y������ֵ 
             image_id   ͼƬ���
             image_x    ͼƬ�ڲ�X����
             image_y    ͼƬ�ڲ�Y����
             image_l    ���г���
             image_w    ���п��
             masken     ����ɫʹ��
* ���ڲ����� ��
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
* ��    �ƣ� DisFlashImgae()
* ��    �ܣ� ������ʾ
* ��ڲ����� x               �Ե�Ϊ��λ��X������ֵ
             y               �Ե�Ϊ��λ��Y������ֵ 
             flashimage_id   GIF����ID���     
    	     enable          �ر�/������������
	         playnum         �������Ŵ��� 0:һֱ���ţ�1~FF:���Ŵ���
* ���ڲ����� ��
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
* ��    �ƣ� GUI_Dot()
* ��    �ܣ� ����
* ��ڲ����� x       �Ե�Ϊ��λ��X������ֵ
             y       �Ե�Ϊ��λ��Y������ֵ           
* ���ڲ����� ��
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
* ��    �ƣ� GUI_Line()
* ��    �ܣ� ����
* ��ڲ����� x0      �Ե�Ϊ��λ��X������ֵ
             y0      �Ե�Ϊ��λ��Y������ֵ    
             x1      �Ե�Ϊ��λ��X������ֵ
             y1      �Ե�Ϊ��λ��Y������ֵ  
* ���ڲ����� ��
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
* ��    �ƣ� GUI_ConDots()
* ��    �ܣ� ��������
* ��ڲ����� mode     0:��ǰ��ɫ��� 1���ñ���ɫ���    
	         pDot     ָ����ָ��   
* ���ڲ����� ��
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
* ��    �ƣ� GUI_Circle()
* ��    �ܣ� ������Բ
* ��ڲ����� x               �Ե�Ϊ��λ��X������ֵ
             y               �Ե�Ϊ��λ��Y������ֵ    
             r                ����Բ�İ뾶   
* ���ڲ����� ��
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
* ��    �ƣ� GUI_CircleFill()
* ��    �ܣ� ��ʵ��Բ
* ��ڲ����� x               �Ե�Ϊ��λ��X������ֵ
             y               �Ե�Ϊ��λ��Y������ֵ    
             r                ʵ��Բ�İ뾶   
* ���ڲ����� ��
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
* ��    �ƣ� GUI_Arc()
* ��    �ܣ� ��Բ��
* ��ڲ����� x               �Ե�Ϊ��λ��X������ֵ
             y               �Ե�Ϊ��λ��Y������ֵ    
             r               Բ�İ뾶  
 	         sa              ��ʼ�Ƕ�
	         ea              �����Ƕ�
* ���ڲ����� ��
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
* ��    �ƣ� GUI_Rectangle()
* ��    �ܣ� �����ľ���
* ��ڲ����� x0      �Ե�Ϊ��λ�Ŀ��ľ������Ͻ�X����ֵ
             y0      �Ե�Ϊ��λ�Ŀ��ľ������Ͻ�Y����ֵ   
             x1      �Ե�Ϊ��λ�Ŀ��ľ������½�X����ֵ
             y1      �Ե�Ϊ��λ�Ŀ��ľ������½�Y����ֵ   
* ���ڲ����� ��
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
* ��    �ƣ� RectangleFill()
* ��    �ܣ� ��ʵ�ľ���
* ��ڲ����� x0      �Ե�Ϊ��λ��ʵ�ľ������Ͻ�X����ֵ
             y0      �Ե�Ϊ��λ��ʵ�ľ������Ͻ�Y����ֵ   
             x1      �Ե�Ϊ��λ��ʵ�ľ������½�X����ֵ
             y1      �Ե�Ϊ��λ��ʵ�ľ������½�Y����ֵ   
* ���ڲ����� ��
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
* ��    �ƣ� GUI_AreaInycolor()
* ��    �ܣ� ��Ļ����ɫ
* ��ڲ����� x0      �Ե�Ϊ��λ��ʵ�ľ������Ͻ�X����ֵ
             y0      �Ե�Ϊ��λ��ʵ�ľ������Ͻ�Y����ֵ   
             x1      �Ե�Ϊ��λ��ʵ�ľ������½�X����ֵ
             y1      �Ե�Ϊ��λ��ʵ�ľ������½�Y����ֵ   
* ���ڲ����� ��
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
* ��    �ƣ� GUI_Ellipse()
* ��    �ܣ� ��������Բ
* ��ڲ����� x0      �Ե�Ϊ��λ�Ŀ�����Բ�����X����ֵ
             y0      �Ե�Ϊ��λ�Ŀ�����Բ���϶�Y����ֵ   
             x1      �Ե�Ϊ��λ�Ŀ�����Բ���Ҷ�X����ֵ
             y1      �Ե�Ϊ��λ�Ŀ�����Բ���¶�Y����ֵ   
* ���ڲ����� ��
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
* ��    �ƣ� GUI_EllipseFill()
* ��    �ܣ� ��ʵ����Բ
* ��ڲ����� x0      �Ե�Ϊ��λ��ʵ����Բ�����X����ֵ
             y0      �Ե�Ϊ��λ��ʵ����Բ���϶�Y����ֵ   
             x1      �Ե�Ϊ��λ��ʵ����Բ���Ҷ�X����ֵ
             y1      �Ե�Ϊ��λ��ʵ����Բ���¶�Y����ֵ   
* ���ڲ����� ��
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
* ��    �ƣ� GUI_PolygonFill()
* ��    �ܣ� ��������
* ��ڲ����� x               �Ե�Ϊ��λ��X������ֵ
             y               �Ե�Ϊ��λ��Y������ֵ    
             color            �����ɫ   
* ���ڲ����� ��
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
* ��    �ƣ� SetBackLight()
* ��    �ܣ� �������
* ��ڲ����� light_level
* ���ڲ����� ��
****************************************************************************/
void SetBackLight(u8 light_level)
{
	SendBeginCMD();
	SendChar(0x60);
	SendChar(light_level);
	SendEndCmd();
}
/****************************************************************************
* ��    �ƣ� SetBuzzer()
* ��    �ܣ� ����������
* ��ڲ����� light_level
* ���ڲ����� ��
****************************************************************************/
void SetBuzzer(u8 time)
{
	SendBeginCMD();
	SendChar(0x61);
	SendChar(time);
	SendEndCmd();
}
/****************************************************************************
* ��    �ƣ� SetCommBps()
* ��    �ܣ� ����ͨѶ������
* ��ڲ����� Baud
* ���ڲ����� ��
****************************************************************************/
void SetCommBps(u8 baudset)  
{
	SendBeginCMD();
	SendChar(0xA0);
	SendChar(baudset);
	SendEndCmd();
}
/****************************************************************************
* ��    �ƣ� SetTouchScreen(u8 cmd)
* ��    �ܣ� ����������
* ��ڲ����� cmd   Bit0:1/0 ������/��; Bit1 : 1/0 ��������/��; Bit2: �ϴ���ʽ  
* ���ڲ����� ��
****************************************************************************/
void SetTouchScreen(u8 cmd)
{
	SendBeginCMD();
	SendChar(0x70);
	SendChar(cmd);
	SendEndCmd();
}
/****************************************************************************
* ��    �ƣ� SetTouchScreen_Adj()
* ��    �ܣ� ������У׼
* ��ڲ����� ��
* ���ڲ����� ��
****************************************************************************/
void SetTouchScreen_Adj()
{
	SendBeginCMD();
	SendChar(0x72);
	SendEndCmd();
}
/****************************************************************************
* ��    �ƣ� TestTouchScreen()
* ��    �ܣ� ����������
* ��ڲ����� ��
* ���ڲ����� ��
****************************************************************************/
void TestTouchScreen()
{
	SendBeginCMD();
	SendChar(0x73);
	SendEndCmd();
}

/****************************************************************************
* ��    �ƣ� WriteLayer()
* ��    �ܣ� дͼ�����
* ��ڲ����� layer         д���ͼ����
* ���ڲ����� ��
****************************************************************************/
void WriteLayer(u8 layer)
{
	SendBeginCMD();
	SendChar(0xa1);
	SendChar(layer);
	SendEndCmd();
}
/****************************************************************************
* ��    �ƣ� WriteLayer()
* ��    �ܣ� �л�ͼ����ʾ
* ��ڲ����� layer         �л���ʾ��ͼ��
* ���ڲ����� ��
****************************************************************************/
void DisplyLayer(u8 layer)
{
	SendBeginCMD();
	SendChar(0xa2);
	SendChar(layer);
	SendEndCmd();
}

/****************************************************************************
* ��    �ƣ� 	C_CopyLayer(uchar src_layer,uchar dest_layer)
* ��    �ܣ�	������ͼ�㿽��������һ��ͼ�㣬���Զ����е�����ʾ
* ��ڲ�����	src_layer (1 ���ֽ�)��Դͼ��
		dest_layer (1 ���ֽ�)��Ŀ��ͼ��   
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_ClearLayer(uchar layer)
* ��    �ܣ� 	���ĳ��ͼ������
* ��ڲ�����	Layer(1 ���ֽ�)��д���ͼ��   
* ���ظ�ʽ�� 	��
****************************************************************************/
void ClearLayer(u8 layer)
{
	SendBeginCMD();
	SendChar(0x05);
	SendChar(layer);
	SendEndCmd();
}

/****************************************************************************
* ��    �ƣ� 	C_AutoClearLayer_en(uchar en)
* ��    �ܣ� 	�����л�����ʱ�Ƿ���Ҫ�Զ������ǰ�û���
* ��ڲ�����	En��1 ���ֽڣ� ��ʹ���ź�
		0x01���Զ����ͼ��
		0x00����ֹ���ͼ�� 
* ���ظ�ʽ�� 	��
****************************************************************************/
void AutoClearLayer_en(u8 en)
{
	SendBeginCMD();
	SendChar(0x06);
	SendChar(en);
	SendEndCmd();
}


//�ؼ���ָ�� 
/*********************************************
* ��    �ƣ� 	DisScreen()
* ��    �ܣ� 	�л���ָ���Ļ���ID��ʾ
* ��ڲ�����	Screen_id(2 ���ֽ�)��������           ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_GetScreen()
* ��    �ܣ� 	��ȡ��ǰ��ʾ�����Screen_idֵ
* ��ڲ�����	��           ����������           
* ���ظ�ʽ�� 	ָ��ظ�ʽ��EE B1 01 Screen_id FF FC FF FF
		Screen_id(2 ���ֽ�)����ǰ����ı��
**************************************/
void 	GetScreen(void)
{
	SendBeginCMD();
	SendChar(0xB1);
	SendChar(0x01);
	SendEndCmd();
}

/*********************************************
* ��    �ƣ� 	C_SetButton()
* ��    �ܣ� 	���´��ذ�ť״̬
* ��ڲ�����	�����»���İ�ť����Ϊ�������״̬
		Screen_id(2 ���ֽ�)��������
		Control_id(2 ���ֽ�)���ؼ����
		Status (1 ���ֽ�)����ť״̬
			0x00�������ɰ��±�ɵ���״̬
			0x01�������ӵ����ɰ���״̬              ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_GetButton()
* ��    �ܣ� 	��ȡ���ذ�ť״̬
* ��ڲ�����	��ѯĳ�����ذ�ť��ǰ�ǰ��»��ǵ���״̬
		Screen_id(2 ���ֽ�)��������
		Control_id(2 ���ֽ�)���ؼ����             ����������           
* ���ظ�ʽ�� 	����ָ���ʽ��EE B1 11 Screen_id   Control_id   Control_type   Subtype Status FF FC FF FF 
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
* ��    �ƣ�	C_SetTextView(u16 Screen_id,u16 Control_id,char *string)  
* ��    �ܣ� 	�����ı��ؼ���ֵ
* ��ڲ�����   	��ָ�����ı��ؼ�д����ʾ����ֵ
		Screen_id(2���ֽ�)�������� 
		Control_id(2���ֽ�)���ؼ���� 
		Strings������������ �û�д����ı���ֵ��   
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_GetTextView(u16 Screen_id,u16 Control_id) 
* ��    �ܣ� 	��ȡ�ı��ؼ�
* ��ڲ�����  	Screen_id(2���ֽ�)�������� 
		Control_id(2���ֽ�)���ؼ����   
* ���ظ�ʽ�� 	����ָ���ʽ��EE B1 11 Screen_id Control_id Control_type Strings 00 FF FC FF FF
		���ز�����Control_type(1���ֽ�)���̶�ֵ0x11����ʾΪ�ı��ؼ�����
		Strings��������������ǰ��ʾ���ı�ֵ
		00������λ
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
* ��    �ƣ� 	C_SetTextViewFocus(u16 Screen_id,u16 Control_id,u8 Focus) 
* ��    �ܣ� 	�����ı��ؼ�����
* ��ڲ�����  	Screen_id(2���ֽ�)�������� 
		Control_id(2���ֽ�)���ؼ����  
		Focus (1���ֽ�):
			0x00:ȡ������
			0x01:���ý���
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_SetProgressValue(u16 Screen_id,u16 Control_id,u16 Progressvalue_H,u16 Progressvalue_L)
* ��    �ܣ� 	���½������ؼ���ֵ
* ��ڲ�����	Screen_id(2���ֽ�)�������� 
		Control_id(2���ֽ�)���ؼ���� 
		Progressvalue��4���ֽڣ����µĽ�����ֵ ͨ����16λ�͵�16λ��������           ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_GetProgressValue(u16 Screen_id,u16 Control_id)
* ��    �ܣ� 	��ȡ�������ؼ���ֵ
* ��ڲ�����	Screen_id(2���ֽ�)�������� 
		Control_id(2���ֽ�)���ؼ����            ����������           
* ���ظ�ʽ�� 	����ָ���ʽ��EE B1 11 Screen_id Control_id Control_type Progressvalue FF FC FF FF
		���ز�����Control_type��1���ֽڣ����̶�ֵ0x12����ʾΪ�������ؼ�����
		Progressvalue��4���ֽڣ�����ǰ������ֵ
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
* ��    �ƣ� 	C_SetSlidervalue(u16 Screen_id,u16 Control_id,u32 Slidervalue)
* ��    �ܣ� 	���»���ؼ���ֵ
* ��ڲ�����	Screen_id(2�ֽ�) ��������
		Control_id(2�ֽ�) ���ؼ����
		Slidervalue(4�ֽ�) ���µĻ���ֵ                ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_GetSlidervalue(u16 Screen_id,u16 Control_id)
* ��    �ܣ� 	��ȡ����ؼ���ֵ
* ��ڲ�����	Screen_id(2�ֽ�) ��������
		Control_id(2�ֽ�) ���ؼ����          ����������           
* ���ظ�ʽ�� 	����ָ���ʽ��EE B1 11 Screen_id Control_id Control_type Slidervalue FF FC FF FF
		���ز�����Control_type��1���ֽڣ����̶�ֵ0x13����ʾΪ�������ؼ�����
		Slidervalue��4���ֽڣ�����ǰ��ʾ�Ļ�����ֵ
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
* ��    �ƣ� 	C_SetMeterValue(u16 Screen_id,u16 Control_id,u32 MeterValue)
* ��    �ܣ� 	�����Ǳ�ؼ���ֵ
* ��ڲ�����	Screen_id(2�ֽ�) ��������
		Control_id(2�ֽ�) ���ؼ����
		MeterValue(4�ֽ�) ���µ��Ǳ�ֵ         ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_GetMeterValue(u16 Screen_id,u16 Control_id)
* ��    �ܣ� 	��ȡ�Ǳ�ؼ���ֵ
* ��ڲ�����	Screen_id(2�ֽ�) ��������
		Control_id(2�ֽ�) ���ؼ����              ����������           
* ���ظ�ʽ�� 	����ָ���ʽ��EE B1 11 Screen_id Control_id Control_type Metervalue FF FC FF FF
		���ز�����Control_type��1���ֽڣ����̶�ֵ0x14����ʾΪ�Ǳ�ؼ�����
		Metervalue��4���ֽڣ�����ǰ��ʾ���Ǳ�ֵ
		ע�����ز���������ͬ�����Ǳ�ؼ���ֵ
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
* ��    �ƣ� 	C_AnimationControl(u8 Cmd,u16 Screen_id,u16 Control_id)
* ��    �ܣ� 	��������
* ��ڲ�����	Cmd��1 ���ֽڣ���
			0x20 ����
			0x21 ֹͣ
			0x22 ��ͣ
			0x23 ��һ֡
			0x24 ��һ֡
		Screen_id(2�ֽ�) ��������
		Control_id(2�ֽ�) ���ؼ����              ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_AnimationControlFrame(u16 Screen_id,u16 Control_id,u8 Pic_ID)
* ��    �ܣ� 	����ָ��֡
* ��ڲ�����	Screen_id(2�ֽ�) ��������
		Control_id(2�ֽ�) ���ؼ����
		Pic_ID	:������֡ID		            ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_AddCurve(u16 Screen_id,u16 Control_id,u8 Cmd,u16 Color)
* ��    �ܣ� 	�������
* ��ڲ�����	screen_id(2���ֽ�)�������� 
		control_id(2���ֽ�)���ؼ����
		cmd(1���ֽ�)����Ӧ����ͨ��
		Color��2���ֽڣ���������ʾ��ɫ                 ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_DelCurve(u16 Screen_id,u16 Control_id,u8 Cmd)
* ��    �ܣ� 	ɾ������
* ��ڲ�����	screen_id(2���ֽ�)�������� 
		control_id(2���ֽ�)���ؼ����
		cmd(1���ֽ�)����Ӧ����ͨ��                ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_AddCurveData()
* ��    �ܣ� 	�����������
* ��ڲ�����	screen_id(2���ֽ�)�������� 
		control_id(2���ֽ�)���ؼ����
		cmd(1���ֽ�)����Ӧ����ͨ��
		Lendth��2���ֽڣ������ݳ���	 
		Data����1���ֽڣ����ݣ��ɶ������ͬʱ����     ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_DelCurveData(u16 Screen_id,u16 Control_id,u8 Cmd)	
* ��    �ܣ� 	ɾ����������
* ��ڲ�����	screen_id(2���ֽ�)�������� 
		control_id(2���ֽ�)���ؼ����
		cmd(1���ֽ�)����Ӧ����ͨ��                ����������           
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_ZoomShift()
* ��    �ܣ� 	����������ƫ��
* ��ڲ�����	screen_id(2���ֽ�)�������� 
		control_id(2���ֽ�)���ؼ����
		L_shift(2���ֽ�)��ˮƽƫ��
		L_zoom(2���ֽ�)��ˮƽ����
		V_shift(2���ֽ�)����ֱƫ��
		V_zoom(2���ֽ�)����ֱ����              ����������           
* ���ظ�ʽ��	��
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
* ��    �ƣ� 	C_DisTime(u8 DisTimeEN,u8 DisTimeMode,u8 DisTimeFont,u16 DisTimeColor,u16 DisTimeX,u16 DisTimeY)
* ��    �ܣ� 	RTC��ʾ����
* ��ڲ�����	DisTimeEN:ʹ���ź�
		0 : RTC �ر�	1��RTC ����
		DisMode(1 ���ֽ�)����ʾģʽ
			0x00 : ��ʽ HH:MM:SS
			0x01 : ��ʽ 20XX-MM-DD HH:MM:SS
		DisTimeFont (1 ���ֽ�) ������ѡ��
			0x00 : 0 ������
			0x01 : 1 ������
			0x02 : 2 ������
			0x03 : 3 ������
		DisTimeColor(2 ���ֽ�) : ��ʾ��ɫ
		DisTimeX (2 ���ֽ�): �Ե�Ϊ��λ�� X ������ֵ
		DisTimeY (2 ���ֽ�): �Ե�Ϊ��λ�� Y ������ֵ
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_SetTime(u8 sec,u8 min,u8 hour,u8 day,u8 week,u8 mon,u8 year)
* ��    �ܣ� 	ʱ������趨
* ��ڲ�����	Sec:������	Min: �����ã�	Hour:Сʱ����	Day: ������
		Week:��������	Mon: ������	Year:������
		�����û�ֱ��ʹ��ʱ�ӿؼ������ RTC ����ʾ��ʱ���У׼��
		��ע���� 1 ���ֽڣ��� BCD ���ʾ������������Ϊ 0x00
* ���ظ�ʽ�� 	��
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
* ��    �ƣ� 	C_ReadTime(void)
* ��    �ܣ� 	��ȡRTCʱ��
* ��ڲ�����	��
* ���ظ�ʽ�� 	���������ʽ��EE +0xF7 +Year +Mon +Week +Day +Hour +Min +Sec +FF FC FF FF
****************************************************************************/
void 	ReadTime(void)
{
	SendBeginCMD();
	SendChar(0x81);	
	SendEndCmd();
}


 