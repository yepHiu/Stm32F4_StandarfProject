/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-23 19:25:19
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-23 19:27:17
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0 - UCOS III RE v2.0\SYSTEM\sys\explore_system.h
 * @Description: 
 */
#ifndef __EXPLORE_SYSTEM_H_
#define __EXPLORE_SYSTEM_H_

#include "stm32f4xx.h"

void WFI_SET(void);							// 执行WFI指令
void INTX_DISABLE(void);						// 关闭所有中断
void INTX_ENABLE(void);							// 开启所有中断
void MSR_MSP(u32 addr);							// 设置堆栈地址

#endif /* __EXPLORE_SYSTEM_H_ */
