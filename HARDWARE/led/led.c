/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-16 19:40:41
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-26 16:56:48
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ve_prj\stm32飞行逻辑控制单元V0.5\HARDWARE\led\led.c
 * @Description: 
 */
#include "led.h"
#include "includes.h"


void InitLED(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;							
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;													
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;												
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;										
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;													
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_1;																			
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE,&GPIO_InitStructure);


    GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_SetBits(GPIOC, GPIO_Pin_2 | GPIO_Pin_3|GPIO_Pin_1);
	GPIO_SetBits(GPIOE,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}

void LED_FLOW(void)
{
    unsigned char i = 0;
    for (i = 0; i < 10; i++)
    {
        switch (i)
        {
        case 0:
            LED2_ON;
            break;

        case 1:
            LED3_ON;
            break;

        case 2:
            LED4_ON;
            break;

        case 3:
            LED5_ON;
            break;

        case 4:
            LED6_ON;
            break;

        case 5:
            LED1_ON;
            break;

        case 6:
            LED9_ON;
            break;

        case 7:
            LED10_ON;
            break;

        case 8:
            LED11_ON;
            break;

        case 9:
            LED12_ON;
            break;
        }
        delay_ms(200);
    }
}
