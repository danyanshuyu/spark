/****************************************************************
版权：
工程名：
作者：			版本：			时间：
文件描述：STM32F103ZET6+MAXII1270火花系统

其他：
函数说明：
修改历史：
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

u8 I2C_Data[17] = {49,50,51,52,53,54,6,7,8,9,10,11,12,13,14,15,16};	 //十七个字节
u8 I2C_Data1[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
/*******************************************************************************
* 函数名  		: main
* 函数描述    	: 主函数
* 输入参数      : 无
* 输出结果     	: 无
* 返回值        : 无
*******************************************************************************/

int main(void)
{
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);

  	/* 设置系统时钟 */
  	RCC_Configuration();

  	/* 设置 GPIO 端口 */
  	GPIO_Configuration();
	
	/*设置 FSMC 控制 SRAM 和 CPLD	*/
	FSMC_SRAM_Init();
	FSMC_CPLD_Init();											   
   
   	/* 设置 USART */
	USART_Configuration();

	/* 设置 TIM2*/
    TIM2_Configuration();
	
	/* 设置 TIM3*/
    TIM3_Configuration();	    

	/*配置外部中断*/
	EXTI_Configuration();

	/*设置 中断*/
	NVIC_Configuration();

	/*设置 外接PCF8563时钟*/   //模拟I2C
    PCF8563_I2C_Init();

	/*设置 外接24C08 EEPROM*/  //模拟I2C
    _24C08_I2C_Init();

	/*配置RTC*/
    RTC_Configuration();       //配置RTC

				
	  Delay_ms(0xFFFF);
	  DisScreen(1);
	  Power_on_Init();
	  _24C08_I2C_HalfwordWrite(0,48);			 //初始化故障数为0

	  Power_on_self_Test();						 //上电自检
	  //I2C_EEPROM测试
//	  _24C08_I2C_HalfwordWrite(0,0);			 //	火花事件计数清0
//	  Delay_ms(0x58);	                         //读写操作之间必须有等待时间
//	  _24C08_I2C_PageWrite(I2C_Data,16,4);		 //初始密码定为1234
//	  Delay_ms(0x58);
//    _24C08_I2C_ByteRead(u8 ReadAddr); 
//	  _24C08_I2C_ByteWrite(4,32); 				  //初始密码长度定为4位
			 	        
	  //FSMC_SRAM测试
/*      *(vu16*) (0x68000000) = 0x1111;
      *(vu16*) (0x68000002) = 0x1234;
      *(vu16*) (0x68000004) = 0x1234;
      Buffer1	= *(vu16*) (0x68000000);
      Buffer2	= *(vu16*) (0x68000002);	 */  

 	  //FSMC_CPLD测试
	  //Delay_ms(0xFFFF);
      //FSMC_CPLD_Write(CPLD_0x840_Status|=0x03,0x840);	   //0x01开关量一灯亮 ，0x02开关量二灯亮，0x04开关量三灯亮
								           //0x08开关量四灯亮 ，0x10开关量无灯亮，0x20开关量六灯亮
	  //FSMC_CPLD_Write(0x00,0x840);	   //0x00开关量输出0 ，灯全灭
      //FSMC_CPLD_Write(0xFF,0x847);	   //测试小灯
	  //FSMC_CPLD_Write(0x55,0x17);	       //测试传感器
	  //FSMC_CPLD_Write(0x00,0x17);		   //关闭传感器测试信号														
	  ScreenInit();

	while(1)
	{

		//检测到火花后，立即更新屏幕显示
	    if(Screen_update_Flag == 1)
		{
             Screen_update();
			 Screen_update_Flag = 0;	
		}									


		//RTC用秒中断显示
	   	if(TimeDisplay==1)
		{
			RTC_Display();
			TimeDisplay = 0;


			if(Spark_test_Enable_Flag == 0)				                 //开机上电时，火花检测程序延迟启动
			{												             //开关测试信号，火花检测程序延迟启动
				Spark_test_Enable_Cut++;
				if(Spark_test_Enable_Cut == 3)
				{
	  				Test_mode_Flag = 0;				                     //火花检测模式                  
				    Spark_test_Enable_Cut = 0;
					Spark_test_Enable_Flag = 1;
				}
			}
     	}								
			
		//串口屏测试				   
		//接收串口屏数据
		if(ScreenData.TranFinsh == 1)//接收数据完成
		{
		 	ScreenDataRecv_Process();
		}											
	  

	}											
}  				 

/*******************************************************************************
* 函数名	: EXTI_Configuration
* 函数描述  : 设置外部中断
* 输入参数  : 无
* 输出结果  : 无
* 返回值    : 无
*******************************************************************************/
void EXTI_Configuration()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	ErrorStatus HSEStartUpStatus;

	//GPIOG5作为外部中断线			   
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource5);
	//配置触发方式
/*	EXTI_DeInit();
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 			   //中断线使能
	EXTI_Init(&EXTI_InitStructure); 					   //初始化中断
	EXTI_ClearITPendingBit(EXTI_Line1);
*/	//EXTI_GenerateSWInterrupt(EXTI_Line1); 			   //产生软中断


}

/*******************************************************************************
* 函数名	: RCC_Configuration
* 函数描述  : 设置系统各部分时钟
* 输入参数  : 无
* 输出结果  : 无
* 返回值    : 无
*******************************************************************************/
void RCC_Configuration(void)
{
	/* 定义枚举类型变量 HSEStartUpStatus */
	ErrorStatus HSEStartUpStatus;

  	/* 复位系统时钟设置 */
  	RCC_DeInit();
  	/* 开启 HSE */
  	RCC_HSEConfig(RCC_HSE_ON);
  	/* 等待 HSE 起振并稳定 */
  	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	/* 判断 HSE 起是否振成功，是则进入if()内部 */
  	if(HSEStartUpStatus == SUCCESS)
  	{
    	/* 选择 HCLK（AHB）时钟源为SYSCLK 1分频 */
    	RCC_HCLKConfig(RCC_SYSCLK_Div1);//配置AHB时钟=SYSCLK=72MHz

    	/* 选择 PCLK2 时钟源为 HCLK（AHB）1分频 */
    	RCC_PCLK2Config(RCC_HCLK_Div1); //配置APB2时钟=SYSCLK=72MHz

    	/* 选择 PCLK1 时钟源为 HCLK（AHB）2分频 */
    	RCC_PCLK1Config(RCC_HCLK_Div2);	//配置APB1时钟=SYSCLK/2=36MHZ，APB1的预分频系数为2 

    	/* 设置 FLASH 延时周期数为2 */
    	FLASH_SetLatency(FLASH_Latency_2);

    	/* 使能 FLASH 预取缓存 */
    	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    	/* 选择锁相环（PLL）时钟源为 HSE 1分频，倍频数为9，则PLL输出频率为 8MHz * 9 = 72MHz */
    	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    	/* 使能 PLL */ 
    	RCC_PLLCmd(ENABLE);

    	/* 等待 PLL 输出稳定 */
    	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    	/* 选择 SYSCLK 时钟源为 PLL */
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//SYSCLK=PLL时钟

    	/* 等待 PLL 成为 SYSCLK 时钟源 */
    	while(RCC_GetSYSCLKSource() != 0x08);
  	}

  	/* 使能各个用到的外设时钟 */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOD | 
							RCC_APB2Periph_GPIOG| RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF|
						    RCC_APB2Periph_AFIO ,ENABLE);	//使能GPIOA，GPIOB，
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2,ENABLE);  //使能串口2
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);      //使能FSMC时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);    //使能TIM2,TIM3时钟

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);//使能PWR和BKP时钟
}

/*******************************************************************************
* 函数名  		: GPIO_Configuration
* 函数描述    	: 设置各GPIO端口功能
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/

void GPIO_Configuration(void)
{
	/* 定义 GPIO 初始化结构体 GPIO_InitStructure */
  	GPIO_InitTypeDef GPIO_InitStructure;
	

	/* 设置普通GPIO引脚（PF.8,PF.10）为输出引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;	//PF.8蜂鸣器，PF.10报警灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF , &GPIO_InitStructure);

	/* 设置 USART2 的Tx脚（PA.2）为第二功能推挽输出功能 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);
    
  	/* 设置 USART2 的Rx脚（PA.3）为浮空输入脚 */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA , &GPIO_InitStructure);

	//外部中断线GPIOG5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //配置浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

}

/*******************************************************************************
* 函数名  		: USART_Configuration
* 函数描述    	: 设置USART2
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void USART_Configuration(void)
{
	/* 定义 USART 初始化结构体 USART_InitStructure */
  	USART_InitTypeDef USART_InitStructure;

	/*定义USART时钟结构体*/
	USART_ClockInitTypeDef USART_ClockInitStructure;

	/*	波特率为9600bps;
	*	8位数据长度;
	*	1个停止位，无校验;
	*	禁用硬件流控制;
	*	禁止USART时钟;
	*	时钟极性低;
	*	在第2个边沿捕获数据
	*	最后一位数据的时钟脉冲不从 SCLK 输出； 
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
	
    USART_Init(USART2 , &USART_InitStructure);//初始化串口

  	/* 使能USART2接受中断*/
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	/* 使能 USART2 */
  	USART_Cmd(USART2,ENABLE); //
}


/*******************************************************************************
* 函数名  		: TIM2_Configuration
* 函数描述    	: 设置定时器2
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
* 说明			：定时器的计数频率
				TIMx_CLK = CK_INT/(TIM_Prescaler+1)
				CK_INT:内部时钟频率，APB1的倍频器送出时钟72MHZ
				TIM_Prescalar:设置的预分频系数，0-65535

				设置TIM_Prescalar=36000
				TIMx_CLK=72MHz/36000=2000HZ,则定时器的计数周期=1/2000HZ=0.5ms
				如果定时1s，则需要计数2000次，自动重装载值
*******************************************************************************/
void TIM2_Configuration(void)							 //按周期循环检测火花
{
	/*定义定时器初始化结构体*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_DeInit(TIM2);//将Timer设置为缺省值

	TIM_InternalClockConfig(TIM2);//采用内部时钟给TIM2提供时钟源

	/*初始化定时器*/
	TIM_TimeBaseStructure.TIM_Period = 1000;//设置计数溢出大小，每计1000个产生一个更新事件	 //自动重装周期1000*0.5ms=500ms
	TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;//7199;//预分频计数值为36000-1，这样计数器时钟为72MHz/36000=2000Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //TIM_CKD_DIV1;设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//清除标志
	
	TIM_ARRPreloadConfig(TIM2,DISABLE);//预装载寄存器的内容立即传送到影子寄存器

	TIM_ITConfig(TIM2,TIM_IT_Update | TIM_IT_Trigger, ENABLE);//使能指定的TIM中断

	TIM_Cmd(TIM2,DISABLE);//暂时关闭定时器2，在需要时开启
} 

/*******************************************************************************
* 函数名  		: TIM3_Configuration
* 函数描述    	: 设置定时器3
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
* 说明			：定时器的计数频率
				TIMx_CLK = CK_INT/(TIM_Prescaler+1)
				CK_INT:内部时钟频率，APB1的倍频器送出时钟72MHZ
				TIM_Prescalar:设置的预分频系数，0-65535

				设置TIM_Prescalar=36000
				TIMx_CLK=72MHz/36000=2000HZ,则定时器的计数周期=1/2000HZ=0.5ms
				如果定时1s，则需要计数2000次，自动重装载值
*******************************************************************************/
void TIM3_Configuration(void)							 //按周期中断延时功能
{
	/*定义定时器初始化结构体*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_DeInit(TIM3);//将Timer设置为缺省值

	TIM_InternalClockConfig(TIM3);//采用内部时钟给TIM3提供时钟源

	/*初始化定时器*/
	TIM_TimeBaseStructure.TIM_Period = 200;//设置计数溢出大小，每计200个产生一个更新事件	 //自动重装周期200*0.5ms=100ms
	TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;//7199;//预分频计数值为36000-1，这样计数器时钟为72MHz/36000=2000Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //TIM_CKD_DIV1;设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);//清除标志
	
	TIM_ARRPreloadConfig(TIM3,DISABLE);//预装载寄存器的内容立即传送到影子寄存器

	TIM_ITConfig(TIM3,TIM_IT_Update | TIM_IT_Trigger, ENABLE);//使能指定的TIM中断

	TIM_Cmd(TIM3,DISABLE);//暂时关闭定时器3，在需要时开启
}

/*******************************************************************************
* 函数名  		: NVIC_Configuration
* 函数描述    	: 设置定时器中断
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void NVIC_Configuration(void)
{
	/*定义NVIC初始化结构体*/
	NVIC_InitTypeDef NVIC_InitStructure;

//	#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
//	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
//	#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
//	#endif

	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0); //设定中断向量表机制0x08000000

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//先占优先级2位（4种），从占优先级2位（4种）

	//设置串口中断
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;// | EXTI9_5_IRQChannel;	//串口中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级4级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//从优先级4级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	


	//定时器TIM2中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级4级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//从优先级4级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	//定时器TIM3中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //先占优先级4级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//从优先级4级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	//Enable the RTC Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}



/*******************************************************************************
* 函数名  		: fputc
* 函数描述    	: 将printf函数重定位到USATR2
* 输入参数    	: 无
* 输出结果    	: 无
* 返回值		: 无
*******************************************************************************/
																		    
int fputc(int ch, FILE *f)
{
	USART_SendData(USART2, (u8) ch); //
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	 //
	return ch;
}
/*******************************************************************************
* 函数名  		: Delay_ms(u16 time)
* 函数描述    	: 微秒级延迟函数
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
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
* 函数名  		: Delay_us(u16 time)
* 函数描述    	: 微秒级延迟函数
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
*******************************************************************************/
void Delay_us(u32 time)
{
	while(time--);  
}
/*******************************************************************************
* 函数名  		: RTC_Display
* 函数描述    	: 时钟实时显示
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
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






