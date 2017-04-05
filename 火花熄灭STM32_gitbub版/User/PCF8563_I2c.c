#include "extern_variables.h"
#include "24C08_PCF8563_I2c.h"

//ȫ�ֱ���,�����ʼ����ʱ�䣬
//��;��1����ʼ��ʱ�䡣2����ȡ����ʱ�� 
//                    �룬�֣�ʱ���죬���ڣ��·ݣ���ݡ�
u8 PCF8563_Time[7] = {11, 20, 10, 27,   1,    3,   17};


void PCF8563_I2C_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
 
    RCC_APB2PeriphClockCmd  (RCC_APB2Periph_GPIOB, ENABLE);   
  
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;		  //��ӦI2C1��������������  PB6->SCL    PB7->SDA
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //��©����������ڲ��øı�����������¶�ȡIO�ĵ�ƽ
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    PCF8563_SCL_H();	//����								   //û��ʹ��Ӳ��I2C�����������ģ��I2Cͨ��Э��
    PCF8563_SDA_H();

    if((PCF8563_ReadAdress(0x02)&0x80)==0x80)  //   ����Ƿ��һ�������������ʼ��ʱ�䣬������Ϊ��	//��ַ(0x02)���λVLΪ0�򲻳�ʼ��
	{																									//��ַ(0x02)���λVLΪ1���ʼ��
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
        PCF8563_SDA_L();		// 3. 1,2,3�����в���ȱ��
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
		if(PCF8563_Read_SDA())				 //��ȡָ���˿ڹܽŵ�����  ʵ���ǣ�GPIOB->IDR&GPIO_Pin_7��
		{
			ReadByte |= 0x01;			  //GPIOB.7�ߵ�ƽ��ȡ1
		}
		else
		{
			ReadByte &= ~(0x01);		  //GPIOB.7�͵�ƽ��ȡ0
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


void PCF8563_ReadTimes(void)			   //��ȡʱ��:�룬�֣�ʱ���죬���ڣ��·ݣ���ݡ�
{
	
	PCF8563_I2C_Start();
	PCF8563_I2C_WriteByte(0xa2);	  //WriteCode
	PCF8563_I2C_WaitAck();
	PCF8563_I2C_WriteByte(0x02);
	PCF8563_I2C_WaitAck();
	PCF8563_I2C_Start();
	PCF8563_I2C_WriteByte(0xa3);	   //ReadCode
	PCF8563_I2C_WaitAck();
	PCF8563_Time[0] = PCF8563_I2C_ReadByte()&0x7f;		   //ȡһ���ֽڵĺ�7λ����λ���㡣
	PCF8563_I2C_Ack();
	
	PCF8563_Time[1] = PCF8563_I2C_ReadByte()&0x7f;
	PCF8563_I2C_Ack();

	PCF8563_Time[2] = PCF8563_I2C_ReadByte()&0x3f;		   //ȡһ���ֽڵĺ�6λ����λ���㡣
	PCF8563_I2C_Ack();

	PCF8563_Time[3] = PCF8563_I2C_ReadByte()&0x3f;
	PCF8563_I2C_Ack();
	
	PCF8563_Time[4] = PCF8563_I2C_ReadByte()&0x07;		   //ȡһ���ֽڵĺ�3λ����λ���㡣
	PCF8563_I2C_Ack();
	
	PCF8563_Time[5] = PCF8563_I2C_ReadByte()&0x1f;		   //ȡһ���ֽڵĺ�5λ����λ���㡣
	PCF8563_I2C_Ack();
	
	PCF8563_Time[6] = PCF8563_I2C_ReadByte();  
	PCF8563_I2C_NoAck();
	PCF8563_I2C_Stop();

	PCF8563_Time[0] = BCD_TO_HEX(PCF8563_Time[0]);		 //BCD��8421�����һ���ֽ�תʮ������
	PCF8563_Time[1] = BCD_TO_HEX(PCF8563_Time[1]);
	PCF8563_Time[2] = BCD_TO_HEX(PCF8563_Time[2]);
	PCF8563_Time[3] = BCD_TO_HEX(PCF8563_Time[3]);
	PCF8563_Time[4] = BCD_TO_HEX(PCF8563_Time[4]);
	PCF8563_Time[5] = BCD_TO_HEX(PCF8563_Time[5]);
	PCF8563_Time[6] = BCD_TO_HEX(PCF8563_Time[6]);
}

void PCF8563_Set_Times(void)				 //��ʼ��PCF8563��ʱ��	//ֱ������ʮ��������������ת��ΪBCD��
{
    PCF8563_WriteAdress(0x00, 0x20);	//��ֹRTC source clock
	PCF8563_WriteAdress(0x02, HEX_TO_BCD(PCF8563_Time[0]));			   	
	PCF8563_WriteAdress(0x03, HEX_TO_BCD(PCF8563_Time[1]));
	PCF8563_WriteAdress(0x04, HEX_TO_BCD(PCF8563_Time[2]));
	PCF8563_WriteAdress(0x05, HEX_TO_BCD(PCF8563_Time[3]));
	PCF8563_WriteAdress(0x06, HEX_TO_BCD(PCF8563_Time[4]));				//���ڲ���ת��
	PCF8563_WriteAdress(0x07, HEX_TO_BCD(PCF8563_Time[5]));
	PCF8563_WriteAdress(0x08, HEX_TO_BCD(PCF8563_Time[6]));
	PCF8563_WriteAdress(0x00, 0x00);	//Enable RTC sorce clock
}

//��CLKOUT�϶�ʱ1S���һ���½������壬������֤����������STM32��GPIO�������룬���ó��½����жϣ���Ƭ��ÿ��1S����һ���ж�
/*void PCF8563_CLKOUT_1s(void)
{
	PCF8563_WriteAdress(0x01, 0);	//��ֹ��ʱ��������������
	PCF8563_WriteAdress(0x0e, 0);	//�رն�ʱ���ȵ�
//	PCF8563_WriteAdress(0x0e, 0);	//д��1 

	PCF8563_WriteAdress(0x0d, 0x83);  //���������
}
*/

