
#include "fsmc_cpld_sram.h"
#include "extern_variables.h"

#define Bank1_SRAM3_ADDR  ((u32)0x68000000)	  //SRAM  //0x68000000~0x6807FFFF//����512K*8Bit 524288Byte	524288/12= 43960������
#define Bank1_SRAM4_ADDR  ((u32)0x6c000000)	  //CPLD

/*******************************************************************************
* Function Name  : FSMC_CPLD_SRAM_Init
* Description    : Configures the FSMC and GPIOs to interface with the SRAM memory.
*                  This function must be called before any write/read operation
*                  on the SRAM.
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef p;							  //�����������ں����Ŀ�ͷ

	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |  RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | 
	 				RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_AFIO , ENABLE);
	
	//SRAM�����ߣ�16����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
	                            GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);


	//SRAM��ַ�� (18��)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |
	                              GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
	                              GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |  GPIO_Pin_4 | 
	                              GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	
	//�����ź���
	/* NOE and NWE configuration ��дʹ���ź���*/  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);					   
	
	// NE3 configuration  Ƭѡ�����ź���(SRAM)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* NBL0, NBL1 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 


	//FSMC_SRAM����
	p.FSMC_AddressSetupTime = 0;  //��ַ����ʱ�䣬Ϊ��HCLK��0-15��,
	p.FSMC_AddressHoldTime = 0;   
	p.FSMC_DataSetupTime = 5;	  //���ݱ���ʱ�䣬һ��HCLKΪ13.8ns(1/72MHz),
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;	//����ģʽ��ģʽA		   

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;				    //ʹ���ĸ��洢��
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;			//�洢�����ͣ���ʾSRAM
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;  //�洢���������߿�� 16λ
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;//��ֹͻ������ģʽ
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;//дʹ��
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;   //��չģʽʹ��λ���Ƿ��������д��ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
	
	//ʹ��FSMC Bank1_SRAM Bank 
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  		//�洢��3ʹ��, ʹ�ⲿsram��ַӳ�䵽STM32�ܷ��ʵ�ַ�ϣ�0x68000000~

}
/*******************************************************************************
* Function Name  : FSMC_CPLD_Init
* Description    : Configures the FSMC and GPIOs to interface with the CPLD.
*                  This function must be called before any write/read operation
*                  on the CPLD.
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_CPLD_Init(void)
{
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef p;							  //�����������ں����Ŀ�ͷ

	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |  RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | 
	 				RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_AFIO , ENABLE);
	
	//SRAM�����ߣ�8����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | 
	                            GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOE, &GPIO_InitStructure);


	//SRAM��ַ�� (12��)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |
	                              GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
	                              GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	
	//�����ź���
	/* NOE and NWE configuration ��дʹ���ź���*/  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);					   
	
	// NE4 configuration  Ƭѡ�����ź���(CPLD)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* NBL0, NBL1 configuration */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
//	GPIO_Init(GPIOE, &GPIO_InitStructure); 


	//FSMC_CPLD����
	p.FSMC_AddressSetupTime = 0;  //��ַ����ʱ�䣬Ϊ��HCLK��0-15��,
	p.FSMC_AddressHoldTime = 0;   
	p.FSMC_DataSetupTime = 5;	  //���ݱ���ʱ�䣬һ��HCLKΪ13.8ns(1/72MHz),
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;	//����ģʽ��ģʽA		   

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;				    //ʹ���ĸ��洢��
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;			//�洢�����ͣ���ʾSRAM
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;  //�洢���������߿�� 8λ
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;//��ֹͻ������ģʽ
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;//дʹ��
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;   //��չģʽʹ��λ���Ƿ��������д��ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
	
	//ʹ��FSMC Bank1_SRAM Bank 
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  		//�洢��4ʹ��, ʹ�ⲿCPLD��ַӳ�䵽STM32�ܷ��ʵ�ַ�ϣ�0x6C000000~
}

/*******************************************************************************
* Function Name  : FSMC_SRAM_WriteStruct
* Description    : Writes a byte buffer to the FSMC SRAM memory. 
* Input          : - pBuffer : pointer to buffer. 
*                  - WriteAddr : SRAM memory internal address from which the data 
*                    will be written.
*                    
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_WriteStruct(u8* pBuffer, u32 WriteAddr)
{
  u8 words = sizeof(Save_Info);

  for(; words != 0; words--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(u8 *) (Bank1_SRAM3_ADDR + WriteAddr) = *pBuffer++;
    
    /* Increment the address*/  
    WriteAddr++;
  }   
}


/*******************************************************************************
* Function Name  : FSMC_SRAM_ReadStruct
* Description    : Reads a block of data from the FSMC SRAM memory.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the SRAM memory.
*                  - ReadAddr : SRAM memory internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_ReadStruct(u8* pBuffer, u32 ReadAddr, u32 NumbyteToRead)
{
  for(; NumbyteToRead != 0; NumbyteToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(vu8*) (Bank1_SRAM3_ADDR + ReadAddr);

    /* Increment the address*/  
    ReadAddr++;
  }  
}


/*******************************************************************************
* Function Name  : FSMC_SRAM_WriteBuffer
* Description    : Writes a Half-word buffer to the FSMC SRAM memory. 
* Input          : - pBuffer : pointer to buffer. 
*                  - WriteAddr : SRAM memory internal address from which the data 
*                    will be written.
*                  - NumHalfwordToWrite : number of half-words to write. 
*                    
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite)
{
  for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(u16 *) (Bank1_SRAM3_ADDR + WriteAddr) = *pBuffer++;
    
    /* Increment the address*/  
    WriteAddr += 2;
  }   
}


/*******************************************************************************
* Function Name  : FSMC_SRAM_ReadBuffer
* Description    : Reads a block of data from the FSMC SRAM memory.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the SRAM memory.
*                  - ReadAddr : SRAM memory internal address to read from.
*                  - NumHalfwordToRead : number of half-words to read.
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead)
{
  for(; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(vu16*) (Bank1_SRAM3_ADDR + ReadAddr);

    /* Increment the address*/  
    ReadAddr += 2;
  }  
}

/******************************************************************************* 
���ƣ�FSMC_CPLD_Write 
���ܣ�CPLDдʱ�� 
������uint8_t pBuffer-д������� uint32_t WriteAddr-д��ĵ�ַ 
ʱ�䣺 
�汾��
ע�⣺��Ӳ�������ʹ��12����ַ�ߺ�8�������ߣ�����԰�λ������д�� 
*******************************************************************************/  
void FSMC_CPLD_Write(u8 pBuffer, u32 WriteAddr)  
{  
    *(u8 *) (Bank1_SRAM4_ADDR + WriteAddr) = pBuffer;    
}  
/******************************************************************************* 
���ƣ�uint8_t FSMC_CPLD_Read(uint32_t ReadAddr) 
���ܣ�CPLD�� 
������uint32_t ReadAddr��Ҫ��ȡ�ĵ�ַ�����ض�ȡ��ֵ 
ʱ�䣺
�汾�� 
ע�⣺��Ӳ�������ʹ����12����ַ�ߺ�8�������ߣ�����԰�λ������д�� 
*******************************************************************************/  
u8 FSMC_CPLD_Read(u32 ReadAddr)  
{  
    u8 pBuffer;   
    pBuffer = *(u8*) (Bank1_SRAM4_ADDR + ReadAddr);  
    return pBuffer;   
}  




