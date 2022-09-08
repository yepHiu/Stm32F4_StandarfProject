#include "includes.h"

uint8_t DMA_USART3_TX_BUF[512];
uint8_t DMA_USART3_RX_BUF[512];

uint16_t USART3_RX_LEN;

_Data_Rx uart3_rx;

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DriveUSART3GpioInit
函数功能：串口1GPIO初始化
输入参数：BaudRate波特率
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
static void DriveUSART3GpioInit(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	/****************************************初始化端口***********************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3时钟

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //将端口进行复用
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct); //初始化端口
										/****************************************初始化串口***********************************/
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_Init(USART3, &USART_InitStruct); //初始化串口

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		  // IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;		  //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //使能通道
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); //配置串口中断
	USART_Cmd(USART3, ENABLE);
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA_USART3_TX_INIT
函数功能：USART3+DMA发送初始化
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
static void DMA_USART3_TX_INIT(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA2时钟使能

	DMA_DeInit(DMA1_Stream3);
	while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE)
		; //等待DMA可配置

	/* 配置DMA2 Stream7，USART3发送 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;							//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;			// DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DMA_USART3_TX_BUF;			// DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = DMA_USART3_TX_BUF_LEN;				//数据传输量
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

	DMA_Init(DMA1_Stream3, &DMA_InitStructure);	   //初始化DMA Stream7
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE); // DMA2传输完成中断
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE); //使能USART3DMA发送

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;	  //串口1发送中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE)
	{
	}
	DMA_Cmd(DMA1_Stream3, DISABLE); //不使能
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA_USART3_RX_INIT
函数功能：USART3+DMA接收初始化
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
static void DMA_USART3_RX_INIT(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA2时钟使能

	DMA_DeInit(DMA1_Stream1);
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE)
		;																	//等待DMA可配置
																			/* 配置 DMA2 Stream5，USART3接收 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;							//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;			// DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)DMA_USART3_RX_BUF;			// DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;					//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = DMA_USART3_RX_BUF_LEN;				//数据传输量
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
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);							//初始化DMA Stream

	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); //使能USART3DMA接收

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;	  //串口1发送中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE)
	{
	}
	DMA_Cmd(DMA1_Stream1, ENABLE);
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DriveUSART3Init
函数功能：USART3+DMA初始化
输入参数：BaudRate
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DriveUSART3Init(uint32_t BaudRate)
{
	DriveUSART3GpioInit(BaudRate);
	DMA_USART3_TX_INIT();
	DMA_USART3_RX_INIT();
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：USART3_IRQHandler
函数功能：USART3中断函数
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void USART3_IRQHandler(void)
{
	OSIntEnter();
	OS_ERR err;
	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		u8 clear = clear;
		clear = USART3->SR;
		clear = USART3->DR;
		DMA_Cmd(DMA1_Stream1, DISABLE);			  //关闭DMA,防止处理其间有数据
		USART_ClearFlag(USART3, USART_FLAG_IDLE); //清除IDLE标志
		uart3_rx.len= DMA_USART3_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Stream1);
		memcpy(uart3_rx.buf,DMA_USART3_RX_BUF,uart3_rx.len);
		DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1); //清除DMA2_Steam5传输完成标志
		DMA_SetCurrDataCounter(DMA1_Stream1, DMA_USART3_RX_BUF_LEN);
		DMA_Cmd(DMA1_Stream1, ENABLE);
	}

	if (USART_GetFlagStatus(USART3, USART_IT_TXE) == RESET) //串口发送完成
	{
		USART_ITConfig(USART3, USART_IT_TC, DISABLE);
	}
	OSIntExit();
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA1_Stream3_IRQHandler
函数功能：DMA中断
输入参数：无
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DMA1_Stream3_IRQHandler(void)
{
	OSIntEnter();
	//清除标志
	if (DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) != RESET) //等待DMA2_Steam7传输完成
	{
		DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3); //清除DMA2_Steam7传输完成标志
		DMA_Cmd(DMA1_Stream3, DISABLE);
		USART_ITConfig(USART3, USART_IT_TC, ENABLE); //打开串口发送完成中断
	}
	OSIntExit();
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
函数名称：DMA_USART3_Send
函数功能：串口发送数据
输入参数：data待发送数据  ， size 发送数据字节数
返回参数：无
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void DMA_USART3_Send(u8 *data, u16 size)
{
	memcpy(DMA_USART3_TX_BUF, data, size); //复制数据到DMA发送缓存区
	while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE)
		;										//确保DMA可以被设置
	DMA_SetCurrDataCounter(DMA1_Stream3, size); //设置数据传输长度
	DMA_Cmd(DMA1_Stream3, ENABLE);				//打开DMA数据流，开始发送
}
