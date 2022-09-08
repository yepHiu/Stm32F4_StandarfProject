/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-17 16:09:06
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-17 16:16:12
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\Template工程模板_V2.0\SYSTEM\schedule\schedule.c
 * @Description: 
 */
#include "schedule.h"

void  Proc2msTask(void)
{
    if (Get2msFlag())
    {
        
        Clr2msFlag();
    }
}


void  Proc1SecTask(void)
{
    if (Get1SecFlag())
    {
        
        Clr1SecFlag();
    }
}
