#ifndef _24C08_PCF8563_I2C_H_
#define _24C08_PCF8563_I2C_H_
#include "stm32f10x_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*#define I2C_Speed              100000
#define I2C1_SLAVE_ADDRESS7    0xA0
#define I2C_PageSize           256 */


//PCF8563   的  SCL PB6	 SDA PB7
#define PCF8563_SCL_H()	        GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define PCF8563_SCL_L()         GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define PCF8563_SDA_H()	        GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define PCF8563_SDA_L()         GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define PCF8563_Read_SDA()      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)	//返回0，和1
#define PCF8563_WriteCode 0xa2
#define PCF8563_ReadCode 0xa3
#define         BCD_TO_HEX(bcd) ((((bcd)>>4)*10)+((bcd)&0x0F))		 
#define 	    HEX_TO_BCD(hex) ((((hex)/10)<<4)+((hex)%10))

void PCF8563_I2C_Init();
u8   PCF8563_ReadAdress(u8 Adress);
void PCF8563_WriteAdress(u8 Adress, u8 DataTX);
void PCF8563_ReadTimes(void);//取回八个字节的数据:秒，分，时，天，星期，月份，年份。
void PCF8563_CLKOUT_1s(void);	//在CLKOUT上定时1S输出一个下降沿脉冲
void PCF8563_Set_Times(void);

u8   PCF8563_I2C_ReadByte(void);
void PCF8563_I2C_WriteByte(u8 byte);
void PCF8563_I2C_WaitAck(void);
void PCF8563_I2C_Stop(void);
void PCF8563_I2C_Start(void);
void PCF8563_I2C_Init(void);
void PCF8563_I2C_Ack(void);
void PCF8563_I2C_NoAck(void);

//24C08   的  SCL PB10	 SDA PB11
#define _24C08_SCL_H()	        GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define _24C08_SCL_L()         GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define _24C08_SDA_H()	        GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define _24C08_SDA_L()         GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define _24C08_Read_SDA()      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)	//返回0，和1
#define _24C08_WriteCode 0xa0
#define _24C08_ReadCode 0xa1

u8   _24C08_I2C_ReadByte(void);
void _24C08_I2C_WriteByte(u8 byte);
void _24C08_I2C_WaitAck(void);
void _24C08_I2C_Stop(void);
void _24C08_I2C_Start(void);
void _24C08_I2C_Init(void);
void _24C08_I2C_Ack(void);
void _24C08_I2C_NoAck(void);

void _24C08_I2C_Init();
void _24C08_I2C_ByteWrite(u8 Buffer, u8 WriteAddr);
void _24C08_I2C_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
void _24C08_I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u8 NumByteToRead);

#endif

	