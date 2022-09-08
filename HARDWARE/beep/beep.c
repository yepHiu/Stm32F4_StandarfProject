/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-16 19:40:41
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-26 20:15:39
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ve_prj\stm32飞行逻辑控制单元V0.5\HARDWARE\beep\beep.c
 * @Description: 
 */
#include "beep.h"


void InitBEEP(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	BEEP_ON;
    delay_ms(100);
    BEEP_OFF;
}

