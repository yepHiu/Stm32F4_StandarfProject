/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-16 17:15:02
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-08-01 12:39:08
 * @FilePath: \USER\main.c
 * @Description:
 */
#include "includes.h"


#define START_TASK_PRIO 3
#define START_STK_SIZE 1024
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

#define TEST_TASK_PRIO 3
#define TEST_STK_SIZE 1024
OS_TCB TEST_TaskTCB;
CPU_STK TEST_TASK_STK[TEST_STK_SIZE];
void test_task(void *p_arg);

#define SEND_TASK_PRIO 4	
#define SEND_STK_SIZE 512
OS_TCB SENDTaskTCB;							
CPU_STK SEND_TASK_STK[SEND_STK_SIZE];					
void SEND_task(void *p_arg);

#define CTR_TASK_PRIO	5
#define CTR_STK_SIZE	1024
OS_TCB CTR_TaskTCB;
CPU_STK CTR_TASK_STK[CTR_STK_SIZE];
void CTR_task(void *p_arg);


#define UART1_TASK_PRIO 6
#define UART1_STK_SIZE 1024
OS_TCB UART1_TaskTCB;
CPU_STK UART1_TASK_STK[UART1_STK_SIZE];
void UART1_task(void *p_arg);

#define UART2_TASK_PRIO 7
#define UART2_STK_SIZE 1024
OS_TCB UART2_TaskTCB;
CPU_STK UART2_TASK_STK[UART2_STK_SIZE];
void UART2_task(void *p_arg);

#define UART3_TASK_PRIO 8
#define UART3_STK_SIZE 1024
OS_TCB UART3_TaskTCB;
CPU_STK UART3_TASK_STK[UART3_STK_SIZE];
void UART3_task(void *p_arg);

#define UART4_TASK_PRIO 9
#define UART4_STK_SIZE 1024
OS_TCB UART4_TaskTCB;
CPU_STK UART4_TASK_STK[UART4_STK_SIZE];
void UART4_task(void *p_arg);

#define UART5_TASK_PRIO 10
#define UART5_STK_SIZE 1024
OS_TCB UART5_TaskTCB;
CPU_STK UART5_TASK_STK[UART5_STK_SIZE];
void UART5_task(void *p_arg);

#define CMD_TASK_PRIO	16
#define CMD_STK_SIZE	512
OS_TCB CMD_TaskTCB;
CPU_STK CMD_TASK_STK[CMD_STK_SIZE];
void CMD_task(void *p_arg);

static void InitSoftWare(void);
static void InitHardWare(void);

static void InitSoftWare(void)
{
  LED_FLOW();
  ALLLED_OFF;
}

static void InitHardWare(void)
{
  InitLED();
  InitTIM();
  DriveUsart1Init(115200);
  DriveUSART2Init(115200);
  DriveUSART3Init(115200);
  DriveUART4Init(115200);
  DriveUART5Init(115200);
  InitPWM(999, 84);
  InitKEY();
  InitBEEP();
}

int main(void)
{
  OS_ERR err;
  CPU_SR_ALLOC();

  HSE_SetSysClock(8, 336, 2, 7);
  Systick_Init(168); // 时钟初始化

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  HSE_SetSysClock(8, 336, 2, 7);

  InitHardWare();
  InitSoftWare();

  OSInit(&err);
  OS_CRITICAL_ENTER();
  OSTaskCreate((OS_TCB *)&StartTaskTCB,           //任务控制块
               (CPU_CHAR *)"start task",          //任务名字
               (OS_TASK_PTR)start_task,           //任务函数
               (void *)0,                         //传递给任务函数的参数
               (OS_PRIO)START_TASK_PRIO,          //任务优先级
               (CPU_STK *)&START_TASK_STK[0],     //任务堆栈基地址
               (CPU_STK_SIZE)START_STK_SIZE / 10, //任务堆栈深度限位
               (CPU_STK_SIZE)START_STK_SIZE,      //任务堆栈大小
               (OS_MSG_QTY)0,                     //任务内部消息队列能够接收//的最大消息数目,为0时禁止//接收消息
               (OS_TICK)0,                        //当使能时间片轮转时的时间//片长度，为0时为默认长度。
               (void *)0,                         //用户补充的存储区
               (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
               (OS_ERR *)&err); //存放该函数错误时的返回值

  OS_CRITICAL_EXIT(); //退出临界区
  OSStart(&err);

  while (1)
  {
    
    
  }
}

void start_task(void *p_arg)
{
  OS_ERR err;
  CPU_SR_ALLOC();
  p_arg = p_arg;

  CPU_Init();

#if OS_CFG_STAT_TASK_EN > 0u
  OSStatTaskCPUUsageInit(&err);
#endif // OS_CFG_STAT_TASK_EN>0u

#ifdef CPU_CFG_INT_DIS_MEAS_EN //如果使能了测量中断关闭时间
  CPU_IntDisMeasMaxCurReset();
#endif

#if OS_CFG_SCHED_ROUND_ROBIN_EN //当使用时间片轮转的时候//使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
  OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif

  OS_CRITICAL_ENTER(); //进入临界区

  OSTaskCreate(
      (OS_TCB *)&TEST_TaskTCB,      //任务控制块
      (CPU_CHAR *)"test_task",      //任务名字
      (OS_TASK_PTR)test_task,       //任务函数
      (void *)0,                    //传递给任务的参数
      (OS_PRIO)TEST_TASK_PRIO,      //任务优先级别
      (CPU_STK *)&TEST_TASK_STK[0], //栈基地址
      (CPU_STK_SIZE)TEST_STK_SIZE / 10,
      (CPU_STK_SIZE)TEST_STK_SIZE,
      (OS_MSG_QTY)0,
      (OS_TICK)0,
      (void *)0,
      (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
      (OS_ERR *)&err);

  OSTaskCreate(
      (OS_TCB *)&SENDTaskTCB, // 创建SEND任务
      (CPU_CHAR *)"SEND_task",
      (OS_TASK_PTR)SEND_task,
      (void *)0,
      (OS_PRIO)SEND_TASK_PRIO,
      (CPU_STK *)&SEND_TASK_STK[0],
      (CPU_STK_SIZE)SEND_STK_SIZE / 10,
      (CPU_STK_SIZE)SEND_STK_SIZE,
      (OS_MSG_QTY)0,
      (OS_TICK)0,
      (void *)0,
      (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
      (OS_ERR *)&err);

  OSTaskCreate(
      (OS_TCB *)&CTR_TaskTCB, //
      (CPU_CHAR *)"CTR_task",
      (OS_TASK_PTR)CTR_task,
      (void *)0,
      (OS_PRIO)CTR_TASK_PRIO,
      (CPU_STK *)&CTR_TASK_STK[0],
      (CPU_STK_SIZE)CTR_STK_SIZE / 10,
      (CPU_STK_SIZE)CTR_STK_SIZE,
      (OS_MSG_QTY)0,
      (OS_TICK)0,
      (void *)0,
      (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
      (OS_ERR *)&err);

  OSTaskCreate(
      (OS_TCB *)&UART2_TaskTCB,      //任务控制块
      (CPU_CHAR *)"UART2_task",      //任务名字
      (OS_TASK_PTR)UART2_task,       //任务函数
      (void *)0,                    //传递给任务的参数
      (OS_PRIO)UART2_TASK_PRIO,      //任务优先级别
      (CPU_STK *)&UART2_TASK_STK[0], //栈基地址
      (CPU_STK_SIZE)UART2_STK_SIZE / 10,
      (CPU_STK_SIZE)UART2_STK_SIZE,
      (OS_MSG_QTY)0,
      (OS_TICK)0,
      (void *)0,
      (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
      (OS_ERR *)&err);
  
  OSTaskCreate(
      (OS_TCB *)&UART3_TaskTCB,      //任务控制块
      (CPU_CHAR *)"UART3_task",      //任务名字
      (OS_TASK_PTR)UART3_task,       //任务函数
      (void *)0,                    //传递给任务的参数
      (OS_PRIO)UART3_TASK_PRIO,      //任务优先级别
      (CPU_STK *)&UART3_TASK_STK[0], //栈基地址
      (CPU_STK_SIZE)UART3_STK_SIZE / 10,
      (CPU_STK_SIZE)UART3_STK_SIZE,
      (OS_MSG_QTY)0,
      (OS_TICK)0,
      (void *)0,
      (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
      (OS_ERR *)&err);

  OSTaskCreate(
      (OS_TCB *)&UART4_TaskTCB,      //任务控制块
      (CPU_CHAR *)"UART4_task",      //任务名字
      (OS_TASK_PTR)UART4_task,       //任务函数
      (void *)0,                    //传递给任务的参数
      (OS_PRIO)UART4_TASK_PRIO,      //任务优先级别
      (CPU_STK *)&UART4_TASK_STK[0], //栈基地址
      (CPU_STK_SIZE)UART4_STK_SIZE / 10,
      (CPU_STK_SIZE)UART4_STK_SIZE,
      (OS_MSG_QTY)0,
      (OS_TICK)0,
      (void *)0,
      (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
      (OS_ERR *)&err);

  OSTaskCreate(
      (OS_TCB *)&UART5_TaskTCB,      //任务控制块
      (CPU_CHAR *)"UART5_task",      //任务名字
      (OS_TASK_PTR)UART5_task,       //任务函数
      (void *)0,                    //传递给任务的参数
      (OS_PRIO)UART5_TASK_PRIO,      //任务优先级别
      (CPU_STK *)&UART5_TASK_STK[0], //栈基地址
      (CPU_STK_SIZE)UART5_STK_SIZE / 10,
      (CPU_STK_SIZE)UART5_STK_SIZE,
      (OS_MSG_QTY)0,
      (OS_TICK)0,
      (void *)0,
      (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
      (OS_ERR *)&err);

  OSTaskCreate(
      (OS_TCB *)&CMD_TaskTCB,
      (CPU_CHAR *)"CMD_task",
      (OS_TASK_PTR)CMD_task,
      (void *)0,
      (OS_PRIO)CMD_TASK_PRIO,
      (CPU_STK *)&CMD_TASK_STK[0],
      (CPU_STK_SIZE)CMD_STK_SIZE / 10,
      (CPU_STK_SIZE)CMD_STK_SIZE,
      (OS_MSG_QTY)0,
      (OS_TICK)0,
      (void *)0,
      (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
      (OS_ERR *)&err);

  OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err); //挂起开始任务
  OS_CRITICAL_EXIT();                            //进入临界区
}
