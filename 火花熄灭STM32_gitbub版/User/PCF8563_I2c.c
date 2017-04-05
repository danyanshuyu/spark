#include "extern_variables.h"
#include "24C08_PCF8563_I2c.h"

//全局变量,程序初始化的时间，
//用途：1：初始化时间。2：读取返回时间 
//                    秒，分，时，天，星期，月份，年份。
u8 PCF8563_Time[7] = {11, 20, 10, 27,   1,    3,   17};


void PCF8563_I2C_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
 
    RCC_APB2PeriphClockCmd  (RCC_APB2Periph_GPIOB, ENABLE);   
  
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;		  //对应I2C1的两个复用引脚  PB6->SCL    PB7->SDA
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出，可以在不用改变成输入的情况下读取IO的电平
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    PCF8563_SCL_H();	//拉高								   //没有使用硬件I2C，而是用软件模拟I2C通信协议
    PCF8563_SDA_H();

    if((PCF8563_ReadAdress(0x02)&0x80)==0x80)  //   检查是否第一次启动，是则初始化时间，检查对象为秒	//地址(0x02)最高位VL为0则不初始化
	{																									//地址(0x02)最高位VL为1则初始化
		PCF8563_Set_Times();
	}
}



void PCF8563_I2C_Start(void)
{

        PCF8563_SCL_H();
        PCF8563_SDA_H();
        Delay_us(20);
        
        PCF8563_SDA_L();
        Delay_us(20);
       
        PCF8563_SCL_L();
        Delay_us(20);
       
}
void PCF8563_I2C_Stop(void)
{
        PCF8563_SCL_L();		//1
        Delay_us(20);	// 2
        PCF8563_SDA_L();		// 3. 1,2,3这三行不可缺少
        Delay_us(20);
        PCF8563_SCL_H();
        Delay_us(20);
        PCF8563_SDA_H();
        Delay_us(20);

}
void PCF8563_I2C_WaitAck(void)
{
	u16 k;

	PCF8563_SCL_L();
	PCF8563_SDA_H();
	Delay_us(20);

	PCF8563_SCL_H();
	k = 0;
	while((PCF8563_Read_SDA()!= 0) && (k < 60000))k++;
	Delay_us(20);
	PCF8563_SCL_L();
	Delay_us(20);	
}

void PCF8563_I2C_WriteByte(u8 byte)
{
	u8 i = 0;
	for(i = 0; i < 8; i++)
	{
	 PCF8563_SCL_L();
	 Delay_us(20);
	 if(byte & 0x80)
	 {
	 	PCF8563_SDA_H();
	 }
	 else
	 {
	 	PCF8563_SDA_L();
	 }
	 Delay_us(20);
	 PCF8563_SCL_H();
	 Delay_us(20);
	 byte<<=1;
	}
	 PCF8563_SCL_L();
	 Delay_us(20);
}

u8 PCF8563_I2C_ReadByte(void)
{
	u8 i,ReadByte;

	PCF8563_SDA_H();
	for(i = 0; i < 8; i++)
	{	
		ReadByte <<= 1;
		PCF8563_SCL_L();
		Delay_us(20);
		PCF8563_SCL_H();
		Delay_us(20);
		if(PCF8563_Read_SDA())				 //读取指定端口管脚的输入  实际是（GPIOB->IDR&GPIO_Pin_7）
		{
			ReadByte |= 0x01;			  //GPIOB.7高电平读取1
		}
		else
		{
			ReadByte &= ~(0x01);		  //GPIOB.7低电平读取0
		}
				
	}
	return ReadByte;
}
void PCF8563_I2C_Ack(void)
{        
        PCF8563_SCL_L();
        Delay_us(20);
        PCF8563_SDA_L();
        Delay_us(20);
        PCF8563_SCL_H();
        Delay_us(20);
        PCF8563_SCL_L();
        Delay_us(20);
}

void PCF8563_I2C_NoAck(void)
{        
        PCF8563_SCL_L();
        Delay_us(20);
        PCF8563_SDA_H();
        Delay_us(20);
        PCF8563_SCL_H();
        Delay_us(20);
        PCF8563_SCL_L();
        Delay_us(20);
}


u8 PCF8563_ReadAdress(u8 Adress)
{	
	u8 ReadData;

	PCF8563_I2C_Start();
	PCF8563_I2C_WriteByte(0xa2);  		 //WriteCode
	PCF8563_I2C_WaitAck();
	PCF8563_I2C_WriteByte(Adress);
	PCF8563_I2C_WaitAck();
	PCF8563_I2C_Start();
	PCF8563_I2C_WriteByte(0xa3);  		 //ReadCode
	PCF8563_I2C_WaitAck();
	ReadData = PCF8563_I2C_ReadByte();
	PCF8563_I2C_Stop();
	return ReadData;
}

void  PCF8563_WriteAdress(u8 Adress,u8 DataTX)
{
	PCF8563_I2C_Start();
	PCF8563_I2C_WriteByte(0xa2);		  //WriteCode
	PCF8563_I2C_WaitAck();
	PCF8563_I2C_WriteByte(Adress);
	PCF8563_I2C_WaitAck();
	PCF8563_I2C_WriteByte(DataTX);
	PCF8563_I2C_WaitAck();
	PCF8563_I2C_Stop();
}


void PCF8563_ReadTimes(void)			   //读取时间:秒，分，时，天，星期，月份，年份。
{
	
	PCF8563_I2C_Start();
	PCF8563_I2C_WriteByte(0xa2);	  //WriteCode
	PCF8563_I2C_WaitAck();
	PCF8563_I2C_WriteByte(0x02);
	PCF8563_I2C_WaitAck();
	PCF8563_I2C_Start();
	PCF8563_I2C_WriteByte(0xa3);	   //ReadCode
	PCF8563_I2C_WaitAck();
	PCF8563_Time[0] = PCF8563_I2C_ReadByte()&0x7f;		   //取一个字节的后7位，高位置零。
	PCF8563_I2C_Ack();
	
	PCF8563_Time[1] = PCF8563_I2C_ReadByte()&0x7f;
	PCF8563_I2C_Ack();

	PCF8563_Time[2] = PCF8563_I2C_ReadByte()&0x3f;		   //取一个字节的后6位，高位置零。
	PCF8563_I2C_Ack();

	PCF8563_Time[3] = PCF8563_I2C_ReadByte()&0x3f;
	PCF8563_I2C_Ack();
	
	PCF8563_Time[4] = PCF8563_I2C_ReadByte()&0x07;		   //取一个字节的后3位，高位置零。
	PCF8563_I2C_Ack();
	
	PCF8563_Time[5] = PCF8563_I2C_ReadByte()&0x1f;		   //取一个字节的后5位，高位置零。
	PCF8563_I2C_Ack();
	
	PCF8563_Time[6] = PCF8563_I2C_ReadByte();  
	PCF8563_I2C_NoAck();
	PCF8563_I2C_Stop();

	PCF8563_Time[0] = BCD_TO_HEX(PCF8563_Time[0]);		 //BCD（8421）码的一个字节转十进制数
	PCF8563_Time[1] = BCD_TO_HEX(PCF8563_Time[1]);
	PCF8563_Time[2] = BCD_TO_HEX(PCF8563_Time[2]);
	PCF8563_Time[3] = BCD_TO_HEX(PCF8563_Time[3]);
	PCF8563_Time[4] = BCD_TO_HEX(PCF8563_Time[4]);
	PCF8563_Time[5] = BCD_TO_HEX(PCF8563_Time[5]);
	PCF8563_Time[6] = BCD_TO_HEX(PCF8563_Time[6]);
}

void PCF8563_Set_Times(void)				 //初始化PCF8563的时间	//直接输入十进制数，函数内转换为BCD码
{
    PCF8563_WriteAdress(0x00, 0x20);	//禁止RTC source clock
	PCF8563_WriteAdress(0x02, HEX_TO_BCD(PCF8563_Time[0]));			   	
	PCF8563_WriteAdress(0x03, HEX_TO_BCD(PCF8563_Time[1]));
	PCF8563_WriteAdress(0x04, HEX_TO_BCD(PCF8563_Time[2]));
	PCF8563_WriteAdress(0x05, HEX_TO_BCD(PCF8563_Time[3]));
	PCF8563_WriteAdress(0x06, HEX_TO_BCD(PCF8563_Time[4]));				//星期不用转换
	PCF8563_WriteAdress(0x07, HEX_TO_BCD(PCF8563_Time[5]));
	PCF8563_WriteAdress(0x08, HEX_TO_BCD(PCF8563_Time[6]));
	PCF8563_WriteAdress(0x00, 0x00);	//Enable RTC sorce clock
}

//在CLKOUT上定时1S输出一个下降沿脉冲，经过验证，可以设置STM32的GPIO上拉输入，设置成下降沿中断，单片机每过1S产生一次中断
/*void PCF8563_CLKOUT_1s(void)
{
	PCF8563_WriteAdress(0x01, 0);	//禁止定时器输出，闹铃输出
	PCF8563_WriteAdress(0x0e, 0);	//关闭定时器等等
//	PCF8563_WriteAdress(0x0e, 0);	//写入1 

	PCF8563_WriteAdress(0x0d, 0x83);  //打开输出脉冲
}
*/

