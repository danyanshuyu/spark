
#include "stm32f10x_lib.h"
#include "extern_variables.h"
#include "24C08_PCF8563_I2c.h"
//��ģ��I2Cʹ��24C08��Ƭ��EEPROM��
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//�ӻ���ַ   0xA0   /* Device Address */	  //��4���飬��ַ�ֱ�Ϊ0xA0��0xA2��0xA4��0xA6
//24C08    1024x8 (four blocks of 256 bytes)  256=16x16	 (һ��16ҳ��ÿҳ16�ֽ�)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval : None
  */
void _24C08_I2C_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd  (RCC_APB2Periph_GPIOB, ENABLE);   
  
    /* Configure  pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;        //��©����������ڲ��øı�����������¶�ȡIO�ĵ�ƽ
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	_24C08_SCL_H();	//����								   //û��ʹ��Ӳ��I2C�����������ģ��I2Cͨ��Э��
    _24C08_SDA_H();

}

/**
  * @brief  Writes a Half-word to the I2C EEPROM.
  * @param pBuffer : the buffer  containing the data to be 
  *   written to the EEPROM.
  * @param WriteAddr : EEPROM's internal address to write to.
  * @retval : None
  */
void _24C08_I2C_HalfwordWrite(u16 Buffer, u8 WriteAddr)
{
	Delay_ms(0x58);											//�����Ķ���д����֮��ز����ٵ�ʱ�����������д����
	_24C08_I2C_Start();
	_24C08_I2C_WriteByte(0xA0);
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte(WriteAddr);				        //���ݵ�ַ			   
  	_24C08_I2C_WaitAck();		   
	_24C08_I2C_WriteByte((u8)(Buffer>>8));					//���ݸ�8λ
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte((u8)(Buffer&0x00ff));			    //���ݵ�8λ
	_24C08_I2C_WaitAck();
    _24C08_I2C_Stop();
}

/**
  * @brief  Reads a Half-word from the I2C EEPROM.
  * @param ReadAddr : EEPROM's internal address to read from.
  * @retval : ��ȡָ����ַ��16λ����ֵ�� 
  */
u16 _24C08_I2C_HalfwordRead(u8 ReadAddr)
{
    u16 Buffer;
	Delay_ms(0x58);
	_24C08_I2C_Start();
	_24C08_I2C_WriteByte(0xA0);
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte(ReadAddr);				            //���ݵ�ַ			   
  	_24C08_I2C_WaitAck();
	_24C08_I2C_Start();
    _24C08_I2C_WriteByte(0xA1);
	_24C08_I2C_WaitAck();		   
    Buffer = (((u16)_24C08_I2C_ReadByte())<<8);				//���ݸ�8λ	
    _24C08_I2C_Ack();
	Buffer |= (u16)_24C08_I2C_ReadByte();        			//���ݵ�8λ
    _24C08_I2C_NoAck();
    _24C08_I2C_Stop();
	return Buffer;
}

/**
  * @brief  Writes one byte to the I2C EEPROM.
  * @param pBuffer : the buffer  containing the data to be 
  *   written to the EEPROM.
  * @param WriteAddr : EEPROM's internal address to write to.
  * @retval : None
  */
void _24C08_I2C_ByteWrite(u8 Buffer, u8 WriteAddr)
{
	Delay_ms(0x58);
	_24C08_I2C_Start();
	_24C08_I2C_WriteByte(0xA0);
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte(WriteAddr);				        //���ݵ�ַ			   
  	_24C08_I2C_WaitAck();		   
	_24C08_I2C_WriteByte(Buffer);
	_24C08_I2C_WaitAck();
    _24C08_I2C_Stop();
}

/**
  * @brief  Reads one byte from the I2C EEPROM.
  * @param WriteAddr : EEPROM's internal address to read from.
  * @retval : ��ȡָ����ַ��8λ����ֵ�� 
  */
u8 _24C08_I2C_ByteRead(u8 ReadAddr)
{
    u8 Buffer;
	Delay_ms(0x58);
	_24C08_I2C_Start();
	_24C08_I2C_WriteByte(0xA0);
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte(ReadAddr);				        //���ݵ�ַ			   
  	_24C08_I2C_WaitAck();		   
	_24C08_I2C_Start();
    _24C08_I2C_WriteByte(0xA1);
	_24C08_I2C_WaitAck();		   
	Buffer = _24C08_I2C_ReadByte();        			
    _24C08_I2C_NoAck();
    _24C08_I2C_Stop();
	return Buffer;
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE
  *   cycle. The number of byte can't exceed the EEPROM page size.
  * @param pBuffer : pointer to the buffer containing the data to be 
  *   written to the EEPROM.
  * @param WriteAddr : EEPROM's internal address to write to.
  * @param NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval : None
  */
void _24C08_I2C_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
	u8 cnt;
	Delay_ms(0x58);
	_24C08_I2C_Start();
    _24C08_I2C_WriteByte(0xA0);
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte(WriteAddr);				        //���ݵ�ַ			   
  	_24C08_I2C_WaitAck();
    for(cnt=0;cnt<NumByteToWrite;cnt++)  				      /* While there is data to be written */
    {
	    _24C08_I2C_WriteByte(*pBuffer);						  /* Send the current byte */
	    _24C08_I2C_WaitAck();
	    pBuffer++; 										      /* Point to the next byte to be written */
    }
    _24C08_I2C_Stop();
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param pBuffer : pointer to the buffer that receives the data read 
  *   from the EEPROM.
  * @param ReadAddr : EEPROM's internal address to read from.
  * @param NumByteToRead : number of bytes to read from the EEPROM.
  * @retval : None
  */
void _24C08_I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u8 NumByteToRead)
{  
	Delay_ms(0x58);
    _24C08_I2C_Start();
    _24C08_I2C_WriteByte(0xA0);
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte(ReadAddr);				        //���ݵ�ַ			   
  	_24C08_I2C_WaitAck();
    _24C08_I2C_Start();
    _24C08_I2C_WriteByte(0xA1);
	_24C08_I2C_WaitAck();

    while(NumByteToRead)  									  /* While there is data to be read */
    {
	      *pBuffer = _24C08_I2C_ReadByte();
	      pBuffer++; 
	      NumByteToRead--;
		  if(NumByteToRead==0)
		  _24C08_I2C_NoAck();
		  else
		  _24C08_I2C_Ack();          
    }
    _24C08_I2C_Stop();
}


void _24C08_I2C_Start(void)	   //��ʼ����
{

        _24C08_SCL_H();
        _24C08_SDA_H();
        Delay_us(20);
        
        _24C08_SDA_L();
        Delay_us(20);
       
        _24C08_SCL_L();
        Delay_us(20);
       
}
void _24C08_I2C_Stop(void)		//ֹͣ����
{
        _24C08_SCL_L();		//1
        Delay_us(20);	// 2
        _24C08_SDA_L();		// 3. 1,2,3�����в���ȱ��
        Delay_us(20);
        _24C08_SCL_H();
        Delay_us(20);
        _24C08_SDA_H();
        Delay_us(20);

}
void _24C08_I2C_WaitAck(void)	  //�ȴ��ӻ���Ӧ
{
	u16 k;

	_24C08_SCL_L();
	_24C08_SDA_H();
	Delay_us(20);

	_24C08_SCL_H();
	k = 0;
	while((_24C08_Read_SDA()!= 0) && (k < 60000))k++;	  //�ӻ��Ƿ����͵�ƽ
	Delay_us(20);
	_24C08_SCL_L();
	Delay_us(20);	
}

void _24C08_I2C_Ack(void)		   //������Ӧ����ʾ������������
{        
        _24C08_SCL_L();
        Delay_us(20);
        _24C08_SDA_L();
        Delay_us(20);
        _24C08_SCL_H();
        Delay_us(20);
        _24C08_SCL_L();
        Delay_us(20);
}

void _24C08_I2C_NoAck(void)			  //��������Ӧ����ʾ�����������
{        
        _24C08_SCL_L();
        Delay_us(20);
        _24C08_SDA_H();
        Delay_us(20);
        _24C08_SCL_H();
        Delay_us(20);
        _24C08_SCL_L();
        Delay_us(20);
}

void _24C08_I2C_WriteByte(u8 byte)		  //д����ʱ��	 ���������������߸ߵ�
{
	u8 i = 0;
	for(i = 0; i < 8; i++)
	{
	 _24C08_SCL_L();
	 if(byte & 0x80)
	 {
	 	_24C08_SDA_H();
	 }
	 else
	 {
	 	_24C08_SDA_L();
	 }
	 Delay_us(20);
	 _24C08_SCL_H();
	 Delay_us(20);
	 byte<<=1;
	}
	 _24C08_SCL_L();
	 Delay_us(20);
}

u8 _24C08_I2C_ReadByte(void)			 //������ʱ��		 �ӻ����������ߣ��������ͣ�
{
	u8 i,ReadByte;

	_24C08_SDA_H();
	for(i = 0; i < 8; i++)
	{	
		ReadByte <<= 1;
		_24C08_SCL_L();
		Delay_us(20);
		_24C08_SCL_H();
		Delay_us(20);
		if(_24C08_Read_SDA())				 //��ȡָ���˿ڹܽŵ�����   //���ӻ����ͻ������ߵ�ƽ�ֱ�ȡ0��1
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


