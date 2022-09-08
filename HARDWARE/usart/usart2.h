/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-17 14:46:10
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-19 15:26:10
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\usart\usart2.h
 * @Description: 
 * 
 */
#ifndef _USART2_H_
#define _USART2_H_

#define DMA_USART2_TX_BUF_LEN 512
#define DMA_USART2_RX_BUF_LEN 512

#include "stm32f4xx.h"
#include "DataType.h"
#include "para.h"
#include "string.h"
#include "led.h"

extern _Data_Rx uart2_rx;

void DriveUSART2Init(uint32_t BaudRate);
void USART2_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
void DMA_USART2_Send(u8 *data, u16 size);

#endif // !_USART2_H_
