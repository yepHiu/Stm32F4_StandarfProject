/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-21 17:05:15
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-26 17:25:57
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ve_prj\stm32飞行逻辑控制单元V0.5\HARDWARE\key\key.c
 * @Description: 
 */
#include "key.h"

void InitKEY(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC| RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB ,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

