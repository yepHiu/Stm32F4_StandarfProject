/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-19 19:30:50
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-21 17:03:38
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\ADC\ADC.c
 * @Description: 
 */
#include "adc.h"

__IO uint16_t ADC_ConvertedValue[ADC_ChannelNum];
float ADC_ConvertedValueLocal[ADC_ChannelNum];

void InitADC(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;      
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);

    DMA_DeInit(DMA2_Stream0);
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
		; //等待DMA可配置
	/* 配置DMA2 Stream7，USART1发送 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;							//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;			// DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;			// DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = ADC_ChannelNum;				//数据传输量
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

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);	   //初始化DMA Stream0
    DMA_Cmd(DMA2_Stream0,ENABLE);

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                     // 独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; // 两个采样阶段之间的延迟 5 个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;      // DMA 失
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                  // 预分频 4 分频。//ADCCLK=PCLK2/4=84/4=21Mhz,ADC 时钟最好不要超过 36Mhz
    ADC_CommonInit(&ADC_CommonInitStructure);                                    // 初始化
    
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                       // 12 位模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                // 非扫描模式//多通道需要开启
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                          // 关闭连续转换//多通道需要开启
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;             // 1 个转换在规则序列中
    ADC_Init(ADC1, &ADC_InitStructure);                    // ADC 初始化

    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_144Cycles);

    ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
    ADC_DMACmd(ADC1,ENABLE);
    ADC_Cmd(ADC1,ENABLE);

    ADC_SoftwareStartConv(ADC1);

}

void ADC_Conver(void)
{
    uint8_t cnt;
    for(cnt=0;cnt<ADC_ChannelNum;cnt++)
    {
        ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*(float)3.3;
    }
}

void DMA2_Stream0_IRQHandler(void)
{
    if (DMA_GetFlagStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET)  
	{
		
		DMA_ClearFlag(DMA2_Stream0, DMA_IT_TCIF0); 
	}
}
