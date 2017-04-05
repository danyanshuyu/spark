
#include "stm32f10x_lib.h"
#include "extern_variables.h"
#include "24C08_PCF8563_I2c.h"
//用模拟I2C使用24C08（片外EEPROM）
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//从机地址   0xA0   /* Device Address */	  //共4个块，地址分别为0xA0，0xA2，0xA4，0xA6
//24C08    1024x8 (four blocks of 256 bytes)  256=16x16	 (一块16页，每页16字节)
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;        //开漏输出，可以在不用改变成输入的情况下读取IO的电平
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	_24C08_SCL_H();	//拉高								   //没有使用硬件I2C，而是用软件模拟I2C通信协议
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
	Delay_ms(0x58);											//连续的读与写操作之间必不可少的时间间隔，否则读写出错
	_24C08_I2C_Start();
	_24C08_I2C_WriteByte(0xA0);
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte(WriteAddr);				        //数据地址			   
  	_24C08_I2C_WaitAck();		   
	_24C08_I2C_WriteByte((u8)(Buffer>>8));					//数据高8位
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte((u8)(Buffer&0x00ff));			    //数据低8位
	_24C08_I2C_WaitAck();
    _24C08_I2C_Stop();
}

/**
  * @brief  Reads a Half-word from the I2C EEPROM.
  * @param ReadAddr : EEPROM's internal address to read from.
  * @retval : 读取指定地址的16位整型值。 
  */
u16 _24C08_I2C_HalfwordRead(u8 ReadAddr)
{
    u16 Buffer;
	Delay_ms(0x58);
	_24C08_I2C_Start();
	_24C08_I2C_WriteByte(0xA0);
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte(ReadAddr);				            //数据地址			   
  	_24C08_I2C_WaitAck();
	_24C08_I2C_Start();
    _24C08_I2C_WriteByte(0xA1);
	_24C08_I2C_WaitAck();		   
    Buffer = (((u16)_24C08_I2C_ReadByte())<<8);				//数据高8位	
    _24C08_I2C_Ack();
	Buffer |= (u16)_24C08_I2C_ReadByte();        			//数据低8位
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
	_24C08_I2C_WriteByte(WriteAddr);				        //数据地址			   
  	_24C08_I2C_WaitAck();		   
	_24C08_I2C_WriteByte(Buffer);
	_24C08_I2C_WaitAck();
    _24C08_I2C_Stop();
}

/**
  * @brief  Reads one byte from the I2C EEPROM.
  * @param WriteAddr : EEPROM's internal address to read from.
  * @retval : 读取指定地址的8位整型值。 
  */
u8 _24C08_I2C_ByteRead(u8 ReadAddr)
{
    u8 Buffer;
	Delay_ms(0x58);
	_24C08_I2C_Start();
	_24C08_I2C_WriteByte(0xA0);
	_24C08_I2C_WaitAck();
	_24C08_I2C_WriteByte(ReadAddr);				        //数据地址			   
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
	_24C08_I2C_WriteByte(WriteAddr);				        //数据地址			   
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
	_24C08_I2C_WriteByte(ReadAddr);				        //数据地址			   
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


void _24C08_I2C_Start(void)	   //开始条件
{

        _24C08_SCL_H();
        _24C08_SDA_H();
        Delay_us(20);
        
        _24C08_SDA_L();
        Delay_us(20);
       
        _24C08_SCL_L();
        Delay_us(20);
       
}
void _24C08_I2C_Stop(void)		//停止条件
{
        _24C08_SCL_L();		//1
        Delay_us(20);	// 2
        _24C08_SDA_L();		// 3. 1,2,3这三行不可缺少
        Delay_us(20);
        _24C08_SCL_H();
        Delay_us(20);
        _24C08_SDA_H();
        Delay_us(20);

}
void _24C08_I2C_WaitAck(void)	  //等待从机响应
{
	u16 k;

	_24C08_SCL_L();
	_24C08_SDA_H();
	Delay_us(20);

	_24C08_SCL_H();
	k = 0;
	while((_24C08_Read_SDA()!= 0) && (k < 60000))k++;	  //从机是否拉低电平
	Delay_us(20);
	_24C08_SCL_L();
	Delay_us(20);	
}

void _24C08_I2C_Ack(void)		   //主机响应，表示继续接受数据
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

void _24C08_I2C_NoAck(void)			  //主机不响应，表示接受数据完毕
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

void _24C08_I2C_WriteByte(u8 byte)		  //写数据时序	 ，主机控制数据线高低
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

u8 _24C08_I2C_ReadByte(void)			 //读数据时序		 从机控制数据线（拉高拉低）
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
		if(_24C08_Read_SDA())				 //读取指定端口管脚的输入   //看从机拉低还是拉高电平分别取0或1
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


