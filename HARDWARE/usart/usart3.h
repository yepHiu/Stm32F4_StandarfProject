/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-17 14:46:10
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-19 14:20:01
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\usart\USART3.h
 * @Description: 
 * 
 */
#ifndef _USART3_H_
#define _USART3_H_

#define DMA_USART3_TX_BUF_LEN 512
#define DMA_USART3_RX_BUF_LEN 512

#include "stm32f4xx.h"
#include "DataType.h"
#include "para.h"
#include "string.h"
#include "led.h"

extern _Data_Rx uart3_rx;

void DriveUSART3Init(uint32_t BaudRate);
void USART3_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
void DMA_USART3_Send(u8 *data, u16 size);

#endif // !_USART3_H_
