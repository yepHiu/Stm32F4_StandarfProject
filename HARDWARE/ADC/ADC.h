/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-19 19:30:50
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-21 17:03:14
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\ADC\ADC.h
 * @Description: 
 */
#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f4xx.h"
#include "sysconfig.h"

#define ADC_ChannelNum 1

extern __IO uint16_t ADC_ConvertedValue[ADC_ChannelNum];
extern float ADC_ConvertedValueLocal[ADC_ChannelNum];


void InitADC(void);
void ADC_Conver(void);




#endif // !_ADC_H_
