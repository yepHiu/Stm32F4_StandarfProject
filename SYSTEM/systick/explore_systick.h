/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-23 19:10:56
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-23 19:26:25
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0 - UCOS III RE v2.0\SYSTEM\systick\explore_systick.h
 * @Description: 
 */
#ifndef __EXPLORE_SYSTICK_H_
#define __EXPLORE_SYSTICK_H_

#include "stm32f4xx.h"
#include "explore_system.h"

void Systick_Init(u8 SYSCLK);						// 初始化延时函数
void delay_us(u32 nus);							// 延时nus微秒
void delay_ms(u16 nms);							// 延时nms毫秒
void delay_xms(u16 nms);						// 延时nms毫秒
void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q);

#endif /* __EXPLORE_SYSTICK_H_ */
