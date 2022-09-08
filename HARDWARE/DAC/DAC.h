/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-19 19:30:53
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-21 17:00:16
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\DAC\DAC.h
 * @Description: 
 */
#ifndef _DAC_H_
#define _DAC_H_

#include "stm32f4xx.h"
#include "sysconfig.h"

void InitDAC(void);
void DAC1_SetVol(uint16_t vol);



#endif // !_DAC_H_
