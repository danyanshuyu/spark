/****************************************************************
��Ȩ��
��������
���ߣ�			�汾��			ʱ�䣺
�ļ�������STM32F103ZET6+MAXII1270��ϵͳ

������
����˵����
�޸���ʷ��
****************************************************************/
#include "stm32f10x_lib.h"
#include "extern_variables.h"
#include "Screen.h"
#include <stdio.h>
#include <string.h>

			   
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void SPI_Configuration(void);
void TIM2_Configuration(void);
void TIM3_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);
extern void Delay_ms(u16);
void Delay_us(u32);
void RTC_Display();


u32 Write_To_Flash[5];
u32 Read_From_Flash[5];

u16 Buffer1;
u16 Buffer2;					

u8 I2C_Data[17] = {49,50,51,52,53,54,6,7,8,9,10,11,12,13,14,15,16};	 //ʮ�߸��ֽ�
u8 I2C_Data1[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
/*******************************************************************************
* ������  		: main
* ��������    	: ������
* �������      : ��
* ������     	: ��
* ����ֵ        : ��
*******************************************************************************/

int main(void)
{
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);

  	/* ����ϵͳʱ�� */
  	RCC_Configuration();

  	/* ���� GPIO �˿� */
  	GPIO_Configuration();
	
	/*���� FSMC ���� SRAM �� CPLD	*/
	FSMC_SRAM_Init();
	FSMC_CPLD_Init();											   
   
   	/* ���� USART */
	USART_Configuration();

	/* ���� TIM2*/
    TIM2_Configuration();
	
	/* ���� TIM3*/
    TIM3_Configuration();	    

	/*�����ⲿ�ж�*/
	EXTI_Configuration();

	/*���� �ж�*/
	NVIC_Configuration();

	/*���� ���PCF8563ʱ��*/   //ģ��I2C
    PCF8563_I2C_Init();

	/*���� ���24C08 EEPROM*/  //ģ��I2C
    _24C08_I2C_Init();

	/*����RTC*/
    RTC_Configuration();       //����RTC

				
	  Delay_ms(0xFFFF);
	  DisScreen(1);
	  Power_on_Init();
	  _24C08_I2C_HalfwordWrite(0,48);			 //��ʼ��������Ϊ0

	  Power_on_self_Test();						 //�ϵ��Լ�
	  //I2C_EEPROM����
//	  _24C08_I2C_HalfwordWrite(0,0);			 //	���¼�������0
//	  Delay_ms(0x58);	                         //��д����֮������еȴ�ʱ��
//	  _24C08_I2C_PageWrite(I2C_Data,16,4);		 //��ʼ���붨Ϊ1234
//	  Delay_ms(0x58);
//    _24C08_I2C_ByteRead(u8 ReadAddr); 
//	  _24C08_I2C_ByteWrite(4,32); 				  //��ʼ���볤�ȶ�Ϊ4λ
			 	        
	  //FSMC_SRAM����
/*      *(vu16*) (0x68000000) = 0x1111;
      *(vu16*) (0x68000002) = 0x1234;
      *(vu16*) (0x68000004) = 0x1234;
      Buffer1	= *(vu16*) (0x68000000);
      Buffer2	= *(vu16*) (0x68000002);	 */  

 	  //FSMC_CPLD����
	  //Delay_ms(0xFFFF);
      //FSMC_CPLD_Write(CPLD_0x840_Status|=0x03,0x840);	   //0x01������һ���� ��0x02��������������0x04������������
								           //0x08�������ĵ��� ��0x10�������޵�����0x20������������
	  //FSMC_CPLD_Write(0x00,0x840);	   //0x00���������0 ����ȫ��
      //FSMC_CPLD_Write(0xFF,0x847);	   //����С��
	  //FSMC_CPLD_Write(0x55,0x17);	       //���Դ�����
	  //FSMC_CPLD_Write(0x00,0x17);		   //�رմ����������ź�														
	  ScreenInit();

	while(1)
	{

		//��⵽�𻨺�����������Ļ��ʾ
	    if(Screen_update_Flag == 1)
		{
             Screen_update();
			 Screen_update_Flag = 0;	
		}									


		//RTC�����ж���ʾ
	   	if(TimeDisplay==1)
		{
			RTC_Display();
			TimeDisplay = 0;


			if(Spark_test_Enable_Flag == 0)				                 //�����ϵ�ʱ���𻨼������ӳ�����
			{												             //���ز����źţ��𻨼������ӳ�����
				Spark_test_Enable_Cut++;
				if(Spark_test_Enable_Cut == 3)
				{
	  				Test_mode_Flag = 0;				                     //�𻨼��ģʽ                  
				    Spark_test_Enable_Cut = 0;
					Spark_test_Enable_Flag = 1;
				}
			}
     	}								
			
		//����������				   
		//���մ���������
		if(ScreenData.TranFinsh == 1)//�����������
		{
		 	ScreenDataRecv_Process();
		}											
	  

	}											
}  				 

/*******************************************************************************
* ������	: EXTI_Configuration
* ��������  : �����ⲿ�ж�
* �������  : ��
* ������  : ��
* ����ֵ    : ��
*******************************************************************************/
void EXTI_Configuration()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	ErrorStatus HSEStartUpStatus;

	//GPIOG5��Ϊ�ⲿ�ж���			   
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource5);
	//���ô�����ʽ
/*	EXTI_DeInit();
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 			   //�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure); 					   //��ʼ���ж�
	EXTI_ClearITPendingBit(EXTI_Line1);
*/	//EXTI_GenerateSWInterrupt(EXTI_Line1); 			   //�������ж�


}

/*******************************************************************************
* ������	: RCC_Configuration
* ��������  : ����ϵͳ������ʱ��
* �������  : ��
* ������  : ��
* ����ֵ    : ��
*******************************************************************************/
void RCC_Configuration(void)
{
	/* ����ö�����ͱ��� HSEStartUpStatus */
	ErrorStatus HSEStartUpStatus;

  	/* ��λϵͳʱ������ */
  	RCC_DeInit();
  	/* ���� HSE */
  	RCC_HSEConfig(RCC_HSE_ON);
  	/* �ȴ� HSE �����ȶ� */
  	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	/* �ж� HSE ���Ƿ���ɹ����������if()�ڲ� */
  	if(HSEStartUpStatus == SUCCESS)
  	{
    	/* ѡ�� HCLK��AHB��ʱ��ԴΪSYSCLK 1��Ƶ */
    	RCC_HCLKConfig(RCC_SYSCLK_Div1);//����AHBʱ��=SYSCLK=72MHz

    	/* ѡ�� PCLK2 ʱ��ԴΪ HCLK��AHB��1��Ƶ */
    	RCC_PCLK2Config(RCC_HCLK_Div1); //����APB2ʱ��=SYSCLK=72MHz

    	/* ѡ�� PCLK1 ʱ��ԴΪ HCLK��AHB��2��Ƶ */
    	RCC_PCLK1Config(RCC_HCLK_Div2);	//����APB1ʱ��=SYSCLK/2=36MHZ��APB1��Ԥ��Ƶϵ��Ϊ2 

    	/* ���� FLASH ��ʱ������Ϊ2 */
    	FLASH_SetLatency(FLASH_Latency_2);

    	/* ʹ�� FLASH Ԥȡ���� */
    	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    	/* ѡ�����໷��PLL��ʱ��ԴΪ HSE 1��Ƶ����Ƶ��Ϊ9����PLL���Ƶ��Ϊ 8MHz * 9 = 72MHz */
    	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    	/* ʹ�� PLL */ 
    	RCC_PLLCmd(ENABLE);

    	/* �ȴ� PLL ����ȶ� */
    	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    	/* ѡ�� SYSCLK ʱ��ԴΪ PLL */
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//SYSCLK=PLLʱ��

    	/* �ȴ� PLL ��Ϊ SYSCLK ʱ��Դ */
    	while(RCC_GetSYSCLKSource() != 0x08);
  	}

  	/* ʹ�ܸ����õ�������ʱ�� */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOD | 
							RCC_APB2Periph_GPIOG| RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF|
						    RCC_APB2Periph_AFIO ,ENABLE);	//ʹ��GPIOA��GPIOB��
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2,ENABLE);  //ʹ�ܴ���2
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);      //ʹ��FSMCʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);    //ʹ��TIM2,TIM3ʱ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//ʹ��PWR��BKPʱ��
}

/*******************************************************************************
* ������  		: GPIO_Configuration
* ��������    	: ���ø�GPIO�˿ڹ���
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/

void GPIO_Configuration(void)
{
	/* ���� GPIO ��ʼ���ṹ�� GPIO_InitStructure */
  	GPIO_InitTypeDef GPIO_InitStructure;
	

	/* ������ͨGPIO���ţ�PF.8,PF.10��Ϊ�������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;	//PF.8��������PF.10������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF , &GPIO_InitStructure);

	/* ���� USART2 ��Tx�ţ�PA.2��Ϊ�ڶ���������������� */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
    
  	/* ���� USART2 ��Rx�ţ�PA.3��Ϊ��������� */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);

	//�ⲿ�ж���GPIOG5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //���ø�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

}

/*******************************************************************************
* ������  		: USART_Configuration
* ��������    	: ����USART2
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void USART_Configuration(void)
{
	/* ���� USART ��ʼ���ṹ�� USART_InitStructure */
  	USART_InitTypeDef USART_InitStructure;

	/*����USARTʱ�ӽṹ��*/
	USART_ClockInitTypeDef USART_ClockInitStructure;

	/*	������Ϊ9600bps;
	*	8λ���ݳ���;
	*	1��ֹͣλ����У��;
	*	����Ӳ��������;
	*	��ֹUSARTʱ��;
	*	ʱ�Ӽ��Ե�;
	*	�ڵ�2�����ز�������
	*	���һλ���ݵ�ʱ�����岻�� SCLK ����� 
	*/
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART2,&USART_ClockInitStructure);

	USART_InitStructure.USART_BaudRate = 115200;                   //9600/38400/115200/218750/437500/875000/1000000/2000000
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
    USART_Init(USART2 , &USART_InitStructure);//��ʼ������

  	/* ʹ��USART2�����ж�*/
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	/* ʹ�� USART2 */
  	USART_Cmd(USART2,ENABLE); //
}


/*******************************************************************************
* ������  		: TIM2_Configuration
* ��������    	: ���ö�ʱ��2
* �������      : ��
* ������      : ��
* ����ֵ        : ��
* ˵��			����ʱ���ļ���Ƶ��
				TIMx_CLK = CK_INT/(TIM_Prescaler+1)
				CK_INT:�ڲ�ʱ��Ƶ�ʣ�APB1�ı�Ƶ���ͳ�ʱ��72MHZ
				TIM_Prescalar:���õ�Ԥ��Ƶϵ����0-65535

				����TIM_Prescalar=36000
				TIMx_CLK=72MHz/36000=2000HZ,��ʱ���ļ�������=1/2000HZ=0.5ms
				�����ʱ1s������Ҫ����2000�Σ��Զ���װ��ֵ
*******************************************************************************/
void TIM2_Configuration(void)							 //������ѭ������
{
	/*���嶨ʱ����ʼ���ṹ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_DeInit(TIM2);//��Timer����Ϊȱʡֵ

	TIM_InternalClockConfig(TIM2);//�����ڲ�ʱ�Ӹ�TIM2�ṩʱ��Դ

	/*��ʼ����ʱ��*/
	TIM_TimeBaseStructure.TIM_Period = 1000;//���ü��������С��ÿ��1000������һ�������¼�	 //�Զ���װ����1000*0.5ms=500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;//7199;//Ԥ��Ƶ����ֵΪ36000-1������������ʱ��Ϊ72MHz/36000=2000Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //TIM_CKD_DIV1;����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//�����־
	
	TIM_ARRPreloadConfig(TIM2,DISABLE);//Ԥװ�ؼĴ����������������͵�Ӱ�ӼĴ���

	TIM_ITConfig(TIM2,TIM_IT_Update | TIM_IT_Trigger, ENABLE);//ʹ��ָ����TIM�ж�

	TIM_Cmd(TIM2,DISABLE);//��ʱ�رն�ʱ��2������Ҫʱ����
} 

/*******************************************************************************
* ������  		: TIM3_Configuration
* ��������    	: ���ö�ʱ��3
* �������      : ��
* ������      : ��
* ����ֵ        : ��
* ˵��			����ʱ���ļ���Ƶ��
				TIMx_CLK = CK_INT/(TIM_Prescaler+1)
				CK_INT:�ڲ�ʱ��Ƶ�ʣ�APB1�ı�Ƶ���ͳ�ʱ��72MHZ
				TIM_Prescalar:���õ�Ԥ��Ƶϵ����0-65535

				����TIM_Prescalar=36000
				TIMx_CLK=72MHz/36000=2000HZ,��ʱ���ļ�������=1/2000HZ=0.5ms
				�����ʱ1s������Ҫ����2000�Σ��Զ���װ��ֵ
*******************************************************************************/
void TIM3_Configuration(void)							 //�������ж���ʱ����
{
	/*���嶨ʱ����ʼ���ṹ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_DeInit(TIM3);//��Timer����Ϊȱʡֵ

	TIM_InternalClockConfig(TIM3);//�����ڲ�ʱ�Ӹ�TIM3�ṩʱ��Դ

	/*��ʼ����ʱ��*/
	TIM_TimeBaseStructure.TIM_Period = 200;//���ü��������С��ÿ��200������һ�������¼�	 //�Զ���װ����200*0.5ms=100ms
	TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;//7199;//Ԥ��Ƶ����ֵΪ36000-1������������ʱ��Ϊ72MHz/36000=2000Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //TIM_CKD_DIV1;����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);//�����־
	
	TIM_ARRPreloadConfig(TIM3,DISABLE);//Ԥװ�ؼĴ����������������͵�Ӱ�ӼĴ���

	TIM_ITConfig(TIM3,TIM_IT_Update | TIM_IT_Trigger, ENABLE);//ʹ��ָ����TIM�ж�

	TIM_Cmd(TIM3,DISABLE);//��ʱ�رն�ʱ��3������Ҫʱ����
}

/*******************************************************************************
* ������  		: NVIC_Configuration
* ��������    	: ���ö�ʱ���ж�
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void NVIC_Configuration(void)
{
	/*����NVIC��ʼ���ṹ��*/
	NVIC_InitTypeDef NVIC_InitStructure;

//	#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
//	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
//	#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
//	#endif

	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0); //�趨�ж����������0x08000000

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//��ռ���ȼ�2λ��4�֣�����ռ���ȼ�2λ��4�֣�

	//���ô����ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;// | EXTI9_5_IRQChannel;	//�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//�����ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	


	//��ʱ��TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//�����ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	//��ʱ��TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//�����ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	//Enable the RTC Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}



/*******************************************************************************
* ������  		: fputc
* ��������    	: ��printf�����ض�λ��USATR2
* �������    	: ��
* ������    	: ��
* ����ֵ		: ��
*******************************************************************************/
																		    
int fputc(int ch, FILE *f)
{
	USART_SendData(USART2, (u8) ch); //
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	 //
	return ch;
}
/*******************************************************************************
* ������  		: Delay_ms(u16 time)
* ��������    	: ΢�뼶�ӳٺ���
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void Delay_ms(u16 time)
{
	u16 i = 0;
	while(time--)
	{
		i = 120;
		while(i--);
	}
}
/*******************************************************************************
* ������  		: Delay_us(u16 time)
* ��������    	: ΢�뼶�ӳٺ���
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void Delay_us(u32 time)
{
	while(time--);  
}
/*******************************************************************************
* ������  		: RTC_Display
* ��������    	: ʱ��ʵʱ��ʾ
* �������      : ��
* ������      : ��
* ����ֵ        : ��
*******************************************************************************/
void RTC_Display()
{
	char SECOND[15],YEAR[15],MONTH[15],DATE[15],HOUR[15],MIN[15];
        Get_Cur_Time();
		if(RTCInitFlagStatus == 1)
        {  
		    itoa(calendar.year,YEAR);
  	        itoaD(calendar.month,MONTH);
	        itoaD(calendar.date,DATE);
		    strcat(strcat(strcat(strcat(YEAR,"."),MONTH),"."),DATE);
		    itoaD(calendar.hour,HOUR);
		    itoaD(calendar.min,MIN);
		    itoaD(calendar.second,SECOND);		
		    strcat(strcat(strcat(strcat(HOUR,":"),MIN),":"),SECOND);	
   	        SetTextView(1,3,YEAR);
	        SetTextView(1,30,HOUR);
		    RTCInitFlagStatus = 0;
		}
		else
		{ 
		    if(calendar.second == 0 && calendar.min == 0 && calendar.hour == 0)
		    {
			    itoa(calendar.year,YEAR);
                itoaD(calendar.month,MONTH);
		        itoaD(calendar.date,DATE);
			    strcat(strcat(strcat(strcat(YEAR,"."),MONTH),"."),DATE);
				SetTextView(1,3,YEAR);     		   
		    }		
		    itoaD(calendar.hour,HOUR);
		    itoaD(calendar.min,MIN);	
			itoaD(calendar.second,SECOND);
			strcat(strcat(strcat(strcat(HOUR,":"),MIN),":"),SECOND);
			SetTextView(1,30,HOUR);	
		}
						  									
}






