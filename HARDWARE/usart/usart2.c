#include "includes.h"

uint8_t DMA_USART2_TX_BUF[512];
uint8_t DMA_USART2_RX_BUF[512];

uint16_t USART2_RX_LEN;

_Data_Rx uart2_rx;

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DriveUSART2GpioInit
函数功能：串口1GPIO初始化
输入参数：BaudRate波特率
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
static void DriveUSART2GpioInit(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	/****************************************初始化端口***********************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //将端口进行复用
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct); //初始化端口
										/****************************************初始化串口***********************************/
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_Init(USART2, &USART_InitStruct); //初始化串口

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		  // IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;		  //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //使能通道
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //配置串口中断
	USART_Cmd(USART2, ENABLE);
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA_USART2_TX_INIT
函数功能：USART2+DMA发送初始化
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
static void DMA_USART2_TX_INIT(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA2时钟使能

	DMA_DeInit(DMA1_Stream6);
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE)
		; //等待DMA可配置

	/* 配置DMA2 Stream7，USART2发送 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;							//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;			// DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DMA_USART2_TX_BUF;			// DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = DMA_USART2_TX_BUF_LEN;				//数据传输量
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

	DMA_Init(DMA1_Stream6, &DMA_InitStructure);	   //初始化DMA Stream7
	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE); // DMA2传输完成中断
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); //使能USART2DMA发送

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;	  //串口1发送中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 11;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE)
	{
	}
	DMA_Cmd(DMA1_Stream6, DISABLE); //不使能
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA_USART2_RX_INIT
函数功能：USART2+DMA接收初始化
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
static void DMA_USART2_RX_INIT(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA2时钟使能

	DMA_DeInit(DMA1_Stream5);
	while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE)
		;																	//等待DMA可配置
																			/* 配置 DMA2 Stream5，USART2接收 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;							//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;			// DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DMA_USART2_RX_BUF;			// DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;					//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = DMA_USART2_RX_BUF_LEN;				//数据传输量
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
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);							//初始化DMA Stream

	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); //使能USART2DMA接收

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;	  //串口1发送中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE)
	{
	}
	DMA_Cmd(DMA1_Stream5, ENABLE);
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DriveUSART2Init
函数功能：USART2+DMA初始化
输入参数：BaudRate
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DriveUSART2Init(uint32_t BaudRate)
{
	DriveUSART2GpioInit(BaudRate);
	DMA_USART2_TX_INIT();
	DMA_USART2_RX_INIT();
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：USART2_IRQHandler
函数功能：USART2中断函数
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void USART2_IRQHandler(void)
{
	OSIntEnter();
	OS_ERR err;
	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		u8 clear = clear;
		clear = USART2->SR;
		clear = USART2->DR;
		DMA_Cmd(DMA1_Stream5, DISABLE);			  //关闭DMA,防止处理其间有数据
		USART_ClearFlag(USART2, USART_FLAG_IDLE); //清除IDLE标志
		uart2_rx.len= DMA_USART2_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Stream5);
		memcpy(uart2_rx.buf,DMA_USART2_RX_BUF,uart2_rx.len);
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5); //清除DMA2_Steam5传输完成标志
		DMA_SetCurrDataCounter(DMA1_Stream5, DMA_USART2_RX_BUF_LEN);
		DMA_Cmd(DMA1_Stream5, ENABLE);
	}

	if (USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET) //串口发送完成
	{
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
	}
	OSIntExit();
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA1_Stream6_IRQHandler
函数功能：DMA中断
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DMA1_Stream6_IRQHandler(void)
{
	OSIntEnter();
	//清除标志
	if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET) //等待DMA2_Steam7传输完成
	{
		DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6); //清除DMA2_Steam7传输完成标志
		DMA_Cmd(DMA1_Stream6, DISABLE);
		USART_ITConfig(USART2, USART_IT_TC, ENABLE); //打开串口发送完成中断
	}
	OSIntExit();
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA_USART2_Send
函数功能：串口发送数据
输入参数：data待发送数据  ， size 发送数据字节数
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DMA_USART2_Send(u8 *data, u16 size)
{
	memcpy(DMA_USART2_TX_BUF, data, size); //复制数据到DMA发送缓存区
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE)
		;										//确保DMA可以被设置
	DMA_SetCurrDataCounter(DMA1_Stream6, size); //设置数据传输长度
	DMA_Cmd(DMA1_Stream6, ENABLE);				//打开DMA数据流，开始发送
}
