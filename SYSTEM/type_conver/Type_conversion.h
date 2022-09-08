/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-25 16:16:08
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-25 16:59:45
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\stm32飞行逻辑控制单元V0.5\SYSTEM\type_conver\Type_conversion.h
 * @Description: 
 */
#ifndef _TYPE_CONVERSIONS_H_
#define _TYPE_CONVERSIONS_H_

#include "includes.h"

void FloatToByte(float floatNum, unsigned char *byteArry);
void IntToByte(int intNum, unsigned char *byteArry);
void UintToByte(unsigned int uintNum, unsigned char *byteArry);
void FloatToUnsignedchar(unsigned char sequence, float Originalvalue, unsigned char *dataPC);
void arrycat(unsigned char *dst, unsigned char index, unsigned char *src, unsigned char len);

float Hex_To_Decimal(unsigned char *Byte, int num);
float Asc_to_f(volatile unsigned char *str);
float UnsignedcharToFloat(unsigned char *DataofPC, unsigned char sequence);

int Hex_To_Int(unsigned char *Byte, int num);

unsigned int Hex_To_Uint(unsigned char *Byte, int num);





//float UnsignedcharToFloat(uint16_t *DataofPC,unsigned char sequence);

#endif /* HARDWARE_TYPE_CONVERSIONS_H_ */
