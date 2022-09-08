/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-17 14:46:10
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-19 14:20:01
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\usart\UART4.h
 * @Description: 
 * 
 */
#ifndef _USART4_H_
#define _USART4_H_

#define DMA_UART4_TX_BUF_LEN 512
#define DMA_UART4_RX_BUF_LEN 512

#include "stm32f4xx.h"
#include "DataType.h"
#include "para.h"
#include "string.h"
#include "led.h"

extern _Data_Rx uart4_rx;

void DriveUART4Init(uint32_t BaudRate);
void UART4_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
void DMA_UART4_Send(u8 *data, u16 size);

#endif // !_UART4_H_
