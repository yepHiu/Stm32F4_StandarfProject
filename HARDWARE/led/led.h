/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-25 20:48:59
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-26 16:54:28
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ve_prj\stm32飞行逻辑控制单元V0.5\HARDWARE\led\led.h
 * @Description:
 */
#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"
#include "BITBAND.h"

#define LED4_ON GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define LED4_OFF GPIO_SetBits(GPIOC, GPIO_Pin_1)

#define LED2_ON GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define LED3_ON GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define LED5_ON GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define LED6_ON GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define LED1_ON GPIO_ResetBits(GPIOE, GPIO_Pin_2)
#define LED9_ON GPIO_ResetBits(GPIOE, GPIO_Pin_3)
#define LED10_ON GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define LED11_ON GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define LED12_ON GPIO_ResetBits(GPIOE, GPIO_Pin_6)

#define ALLLED_ON \
    LED6_ON;      \
    LED2_ON;      \
    LED3_ON;      \
    LED4_ON;      \
    LED5_ON;      \
    LED1_ON;      \
    LED9_ON;      \
    LED10_ON;     \
    LED11_ON;     \
    LED12_ON

#define LED2_OFF GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define LED3_OFF GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define LED5_OFF GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define LED6_OFF GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define LED1_OFF GPIO_SetBits(GPIOE, GPIO_Pin_2)
#define LED9_OFF GPIO_SetBits(GPIOE, GPIO_Pin_3)
#define LED10_OFF GPIO_SetBits(GPIOE, GPIO_Pin_4)
#define LED11_OFF GPIO_SetBits(GPIOE, GPIO_Pin_5)
#define LED12_OFF GPIO_SetBits(GPIOE, GPIO_Pin_6)
#define ALLLED_OFF \
    LED6_OFF;      \
    LED2_OFF;      \
    LED3_OFF;      \
    LED4_OFF;      \
    LED5_OFF;      \
    LED1_OFF;      \
    LED9_OFF;      \
    LED10_OFF;     \
    LED11_OFF;     \
    LED12_OFF

void InitLED(void);
void LED_FLOW(void);

#endif // !_LED_H_
