/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-25 12:40:58
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-26 19:33:18
 * @FilePath: \USER\task.h
 * @Description: 
 */
/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-25 12:40:58
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-07-25 12:44:53
 * @FilePath: \USERf:\资料整理\STM32F4\stm32f407ze_Prj\stm32飞行逻辑控制单元V0.5\USER\task.h
 * @Description: 
 */
#ifndef _TASK_H_
#define _TASK_H_

#include "stm32f4xx.h"
#include "includes.h"

extern DroneRTInfo RT_Info;
extern SensorDataSend Send_Sensor;
extern fly_mode_e fly_mode;
extern OS_SEM uart1_proc;
extern OS_SEM uart2_proc;
extern OS_SEM uart3_proc;
extern OS_SEM uart4_proc;
extern OS_SEM uart5_proc;
extern OS_SEM uart6_proc;
extern OS_SEM uart_tofly;

extern OS_SEM FLOW_proc;
extern OS_SEM TOF_proc;


void LED0_task(void *p_arg);
void LED1_task(void *p_arg);


#endif // !_TASK_H_
