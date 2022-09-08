/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-19 19:30:53
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-21 17:13:27
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\DAC\DAC.c
 * @Description: 
 */
#include "DAC.h"

void InitDAC(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;
#if _DAC1_ENABLE==1
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能PA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);   //使能DAC时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;   // 模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; // 下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);         // ①初始化 GPIO

    DAC_InitType.DAC_Trigger = DAC_Trigger_None;//不使用触发功能 TEN1 = 0 
    DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None; // 不使用波形发生
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
    DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//输出缓存关闭
    DAC_Init(DAC_Channel_1, &DAC_InitType);//初始化DAC通道1 
    DAC_Cmd(DAC_Channel_1, ENABLE); //使能DAC通道1
    DAC_SetChannel1Data(DAC_Align_12b_R,0);

#endif //// _DAC1_ENABLE


}

void DAC1_SetVol(uint16_t vol)
{
    double temp=vol;
    temp/=1000;
    temp=temp*4096/3.3;
    DAC_SetChannel1Data(DAC_Align_12b_R,temp);

}
