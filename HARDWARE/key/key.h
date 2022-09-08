/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-21 17:05:15
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-31 20:25:52
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ve_prj\stm32飞行逻辑控制单元V0.5_回滚\HARDWARE\key\key.h
 * @Description: 
 */
#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f4xx.h"
#include "sysconfig.h"
#include "BITBAND.h"

#define KEY4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0?1:0
#define KEY3 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==0?1:0
#define KEY2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0?1:0
#define KEY1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0?1:0
#define KEY5 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0?1:0



void InitKEY(void);

#endif // !_KEY_H_
