/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-17 14:45:48
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-20 18:52:25
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\HARDWARE\spi\spi.h
 * @Description: 
 */
#ifndef _SPI_H_
#define _SPI_H_

#include "sysconfig.h"
#include "stm32f4xx.h"

#define SPI1_CS_LOW()   {GPIOA->BSRRH=GPIO_Pin_4;}
#define SPI1_CS_HIGH()  {GPIOA->BSRRL=GPIO_Pin_4;}


void InitSPI(void);
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SPI1_Read_SendByte(uint8_t byte);

#endif // !_SPI_H_
