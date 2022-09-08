/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-17 14:45:59
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-18 21:34:53
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\tim\tim.h
 * @Description: 
 */
#ifndef _TIM_H_
#define _TIM_H_

#include "stm32f4xx.h"
#include "DataType.h"

#define BASIC_TIM_1                 TIM7
#define BASIC_TIM_1_CLK             RCC_APB1Periph_TIM7
#define BASIC_TIM_2                 TIM7
#define BASIC_TIM_2_CLK             RCC_APB1Periph_TIM7

#define BASIC_TIM_1_IRQn            TIM7_IRQn
#define BASIC_TIM_1_IRQHandler      TIM7_IRQHandler
#define BASIC_TIM_2_IRQn            TIM7_IRQn
#define BASIC_TIM_2_IRQHandler      TIM7_IRQHandler


void InitTIM(void);
void BASIC_TIM_1_IRQnHandler(void);
void BASIC_TIM_2_Handler(void);

u8  Get2msFlag(void);
void  Clr2msFlag(void);
u8  Get1SecFlag(void);
void  Clr1SecFlag(void);

#endif // !_TIM_H_
