/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-17 14:45:59
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-19 14:01:35
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\tim\tim.c
 * @Description: 
 */
#include "tim.h"



static void BASIC_TIM1_Config(u16 arr,u16 psc);

static void BASIC_TIM1_Config(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_TimeBaseStructure.TIM_Period=arr;
    TIM_TimeBaseStructure.TIM_Prescaler=psc;

    TIM_TimeBaseInit(TIM7,&TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM7,TIM_FLAG_Update);
    TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM7,DISABLE);
}

void InitTIM(void)
{
    BASIC_TIM1_Config(999,83);//72MHz/(71+1)=1MHz, 1ms一次中断
}

void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)
    {
        TIM_ClearITPendingBit(TIM7,TIM_FLAG_Update);
    }
}
