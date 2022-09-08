/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-17 14:44:47
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-23 19:20:25
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0 - UCOS III RE v2.0\HARDWARE\i2c\i2c.h
 * @Description: 
 */
#ifndef _I2C_H_
#define _I2C_H_

#include "stm32f4xx.h"
#include "BITBAND.h"
#include "explore_systick.h"

#define SDA_IN()    {GPIOB->MODER&=~(3 <<(9*2));GPIOB->MODER|=0<<9*2;}
#define SDA_OUT()   {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}

#define IIC_SCL PBout(8)
#define IIC_SDA PBout(9)
#define READ_SDA PBin(9)

void InitI2C(void);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(uint8_t ack);


#endif // !_I2C_H_
