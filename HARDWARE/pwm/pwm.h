/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-18 10:32:46
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-20 15:01:55
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\pwm\pwm.h
 * @Description: 
 */
#ifndef _PWM_H_
#define _PWM_H_

#include "stm32f4xx.h"
#include "sysconfig.h"

void InitPWM(uint32_t arr,uint32_t psc);
void SetPWM(TIM_TypeDef *TIMx, uint32_t CCRx,uint16_t pwm);



#endif // !_PWM_H_
