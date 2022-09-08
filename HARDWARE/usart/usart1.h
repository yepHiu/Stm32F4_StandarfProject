/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-17 14:46:10
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-19 14:20:01
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\usart\usart1.h
 * @Description: 
 * 
 */
#ifndef _USART1_H_
#define _USART1_H_

#define DMA_USART1_TX_BUF_LEN 512
#define DMA_USART1_RX_BUF_LEN 512

#include "stm32f4xx.h"
#include "DataType.h"
#include "para.h"
#include "string.h"
#include "led.h"

extern _Data_Rx uart1_rx;

void DriveUsart1Init(uint32_t BaudRate);
void USART1_IRQHandler(void);
void DMA2_Stream7_IRQHandler(void);
void DMA_USART1_Send(u8 *data, u16 size);

#endif // !_USART1_H_
