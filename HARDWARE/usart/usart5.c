#include "includes.h"

uint8_t DMA_UART5_TX_BUF[512];
uint8_t DMA_UART5_RX_BUF[512];

uint16_t UART5_RX_LEN;

_Data_Rx uart5_rx;

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DriveUART5GpioInit
函数功能：串口1GPIO初始化
输入参数：BaudRate波特率
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
static void DriveUART5GpioInit(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	/****************************************初始化端口***********************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //使能UART5时钟

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); //将端口进行复用
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct); //初始化端口
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStruct); //初始化端口
										/****************************************初始化串口***********************************/
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_Init(UART5, &USART_InitStruct); //初始化串口

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;		  // IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //使能通道
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE); //配置串口中断
	USART_Cmd(UART5, ENABLE);
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA_UART5_TX_INIT
函数功能：UART5+DMA发送初始化
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
static void DMA_UART5_TX_INIT(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA2时钟使能

	DMA_DeInit(DMA1_Stream7);
	while (DMA_GetCmdStatus(DMA1_Stream7) != DISABLE)
		; //等待DMA可配置

	/* 配置DMA2 Stream7，UART5发送 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;							//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART5->DR;			// DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DMA_UART5_TX_BUF;			// DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = DMA_UART5_TX_BUF_LEN;				//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//高等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输

	DMA_Init(DMA1_Stream7, &DMA_InitStructure);	   //初始化DMA Stream7
	DMA_ITConfig(DMA1_Stream7, DMA_IT_TC, ENABLE); // DMA2传输完成中断
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE); //使能UART5DMA发送

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;	  //串口1发送中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	while (DMA_GetCmdStatus(DMA1_Stream0) != DISABLE)
	{
	}
	DMA_Cmd(DMA1_Stream7, DISABLE); //不使能
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA_UART5_RX_INIT
函数功能：UART5+DMA接收初始化
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
static void DMA_UART5_RX_INIT(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA2时钟使能

	DMA_DeInit(DMA1_Stream0);
	while (DMA_GetCmdStatus(DMA1_Stream0) != DISABLE)
		;																	//等待DMA可配置
																			/* 配置 DMA2 Stream5，UART5接收 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;							//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART5->DR;			// DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DMA_UART5_RX_BUF;			// DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;					//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = DMA_UART5_RX_BUF_LEN;				//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
	DMA_Init(DMA1_Stream0, &DMA_InitStructure);							//初始化DMA Stream

	
	USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE); //使能UART5DMA接收

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;	  //串口1发送中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 11;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	while (DMA_GetCmdStatus(DMA1_Stream0) != DISABLE)
	{
	}
	DMA_Cmd(DMA1_Stream0, ENABLE);
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DriveUART5Init
函数功能：UART5+DMA初始化
输入参数：BaudRate
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DriveUART5Init(uint32_t BaudRate)
{
	DriveUART5GpioInit(BaudRate);
	DMA_UART5_TX_INIT();
	DMA_UART5_RX_INIT();
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：UART5_IRQHandler
函数功能：UART5中断函数
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void UART5_IRQHandler(void)
{
	OSIntEnter();
	OS_ERR err;
	if (USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	{
		u8 clear = clear;
		clear = UART5->SR;
		clear = UART5->DR;
		DMA_Cmd(DMA1_Stream0, DISABLE);			  //关闭DMA,防止处理其间有数据
		USART_ClearFlag(UART5, USART_FLAG_IDLE); //清除IDLE标志
		uart5_rx.len= DMA_UART5_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Stream0);
		memcpy(uart5_rx.buf,DMA_UART5_RX_BUF,uart5_rx.len);
		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0 | DMA_FLAG_FEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_HTIF2); //清除DMA2_Steam5传输完成标志
		DMA_SetCurrDataCounter(DMA1_Stream0, DMA_UART5_RX_BUF_LEN);
		DMA_Cmd(DMA1_Stream0, ENABLE);
	}

	if (USART_GetFlagStatus(UART5, USART_IT_TXE) == RESET) //串口发送完成
	{
		USART_ITConfig(UART5, USART_IT_TC, DISABLE);
	}
	OSIntExit();
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA1_Stream7_IRQHandler
函数功能：DMA中断
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DMA1_Stream7_IRQHandler(void)
{
	OSIntEnter();
	//清除标志
	if (DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_TCIF7) != RESET) //等待DMA2_Steam7传输完成
	{
		DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7); //清除DMA2_Steam7传输完成标志
		DMA_Cmd(DMA1_Stream7, DISABLE);
		USART_ITConfig(UART5, USART_IT_TC, ENABLE); //打开串口发送完成中断
	}
	OSIntExit();
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA_UART5_Send
函数功能：串口发送数据
输入参数：data待发送数据  ， size 发送数据字节数
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DMA_UART5_Send(u8 *data, u16 size)
{
	memcpy(DMA_UART5_TX_BUF, data, size); //复制数据到DMA发送缓存区
	while (DMA_GetCmdStatus(DMA1_Stream7) != DISABLE)
		;										//确保DMA可以被设置
	DMA_SetCurrDataCounter(DMA1_Stream7, size); //设置数据传输长度
	DMA_Cmd(DMA1_Stream7, ENABLE);				//打开DMA数据流，开始发送
}
