#include "explore_systick.h"
#include "includes.h"							// 如果要使用uC/OS，则必须包含此头文件

static u8 fac_us = 0;							// us延时倍乘数
static u16 fac_ms = 0;							// ms延时倍乘数，在OS下，代表每个节拍的ms数

/**
 * 当delay_us/delay_ms需要支持OS的时候需要三个与OS相关的宏定义和函数来支持
 * 首先是3个宏定义：
 *  delay_osrunning：用于表示OS当前是否正在运行，以决定是否可以使用相关函数
 *  delay_ostickspersec：用于表示OS设定的时钟节拍，delay_init将根据这个参数来初始哈systick
 *  delay_osintnesting：用于表示OS中断嵌套级别，因为中断里面不可以调度，delay_ms使用该参数来决定如何运行
 * 然后是3个函数：
 *  delay_osschedlock：用于锁定OS任务调度，禁止调度
 *  delay_osschedunlock：用于解锁OS任务调度，重新开启调度
 *  delay_ostimedly：用于OS延时，可以引起任务调度
 */

#define delay_osrunning	OSRunning					// OS是否运行标记：0 不运行，1 在运行
#define delay_ostickspersec OSCfg_TickRate_Hz				// OS时钟节拍，即每秒调度次数
#define delay_osintnesting OSIntNestingCtr				// 中断嵌套级别，即中断嵌套次数

/**
 * @Description us级延时时，关闭任务调度(防止打断us级延迟)
 */
void delay_osschedlock(void)
{
	OS_ERR err;
	OSSchedLock(&err);						// uC/OS-III的方式，禁止调度，防止打断us延时
}

/**
 * @Description us级延时时，恢复任务调度
 */
void delay_osschedunlock(void)
{
	OS_ERR err;
	OSSchedUnlock(&err);						// uC/OS-III的方式，恢复调度
}

/**
 * @Description 调用OS自带的延时函数延时
 * @param ticks 延时的节拍数
 */
void delay_ostimedly(u32 ticks)
{
	OS_ERR err;
	OSTimeDly(ticks, OS_OPT_TIME_PERIODIC, &err);			// uC/OS-III延时采用周期模式
}

/**
 * @Description systick中断服务函数，使用OS时用到
 */
void OS_CPU_SysTick_Handler(void)
{
	if(delay_osrunning==1)						// OS开始跑了，才执行正常的调度处理
	{
		OSIntEnter();						// 进入中断
		OSTimeTick();						// 调用uC/OS的时钟服务程序
		OSIntExit();       	 				// 触发任务切换软中断
	}
}

/**
 * @Description 初始化延时函数
 * @param system_clock 系统时钟频率(单位为Mhz)
 * @note 当使用uC/OS的时候，此函数会初始化uC/OS的时钟节拍，Systick定时器的时钟固定为AHB时钟的1/8
 * @return null 无返回值
 */
void Systick_Init(u8 system_clock)
{
	u32 reload;							// 表示一个时钟节拍时间滴答定时器的计数值
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);		// SYSTICK使用外部时钟源的8分频
	fac_us = system_clock/8;					// 定时器1us滴答定时器的计数值
	reload = system_clock/8;					// 中间过程
	reload *= 1000000/delay_ostickspersec;				// 根据delay_ostickspersec设定溢出时间
	fac_ms = 1000/delay_ostickspersec;				// 代表OS可以延时的最少单位
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;			// 开启SYSTICK中断
	SysTick->LOAD = reload;						// 每1/OS_TICKS_PER_SEC秒中断一次
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;			// 开启SYSTICK  
}

/**
 * @Description 延时nus微秒
 * @param nus 延时时间 nus<=204522252(最大值即2^32/fac_us@fac_us=21)
 * @return null 无返回值
 */
void delay_us(u32 nus)
{
	u32 ticks;
	u32 told, tnow, tcnt=0;
	u32 reload = SysTick->LOAD;					// LOAD的值
	ticks = nus*fac_us;						// 需要的节拍数
	delay_osschedlock();						// 阻止OS调度，防止打断us延时
	told = SysTick->VAL;						// 刚进入时的计数器值
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)
				tcnt += told-tnow;			// 这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt += reload-tnow+told;
			told = tnow;
			if(tcnt>=ticks)
				break;					// 时间超过/等于要延迟的时间，则退出.
		}
	}
	delay_osschedunlock();						// 恢复OS调度
}

/**
 * @Description 延时nms毫秒
 * @param nus 延时时间 nms<=65535
 * @return null 无返回值
 */
void delay_ms(u16 nms)
{
	if(delay_osrunning&&delay_osintnesting==0)			// 如果OS已经在跑了，并且不是在中断里面(中断里面不能任务调度)
	{
		if(nms>=fac_ms)						// 延时的时间大于OS的最少时间周期
		{
   			delay_ostimedly(nms/fac_ms);			// OS延时
		}
		nms %= fac_ms;						// OS已经无法提供这么小的延时了，采用普通方式延时
	}
	delay_us((u32)(nms*1000));					// 普通方式延时
}


/*超频设置*/
/*
 * 使用HSE时，设置系统时钟的步骤
 * 1、开启HSE ，并等待 HSE 稳定
 * 2、设置 AHB、APB2、APB1的预分频因子
 * 3、设置PLL的时钟来源
 *    设置VCO输入时钟 分频因子        m
 *    设置VCO输出时钟 倍频因子        n
 *    设置PLLCLK时钟分频因子          p
 *    设置OTG FS,SDIO,RNG时钟分频因子 q
 * 4、开启PLL，并等待PLL稳定
 * 5、把PLLCK切换为系统时钟SYSCLK
 * 6、读取时钟切换状态位，确保PLLCLK被选为系统时钟
 */

/*
 * m: VCO输入时钟 分频因子，取值2~63
 * n: VCO输出时钟 倍频因子，取值192~432
 * p: PLLCLK时钟分频因子  ，取值2，4，6，8
 * q: OTG FS,SDIO,RNG时钟分频因子，取值4~15
 * 函数调用举例，使用HSE设置时钟
 * SYSCLK=HCLK=168M,PCLK2=HCLK/2=84M,PCLK1=HCLK/4=42M
 * HSE_SetSysClock(25, 336, 2, 7);
 * HSE作为时钟来源，经过PLL倍频作为系统时钟，这是通常的做法
 
 * 系统时钟超频到216M爽一下
 * HSE_SetSysClock(25, 432, 2, 9);
 */
void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)	
{
  __IO uint32_t HSEStartUpStatus = 0;
  
  // 使能HSE，开启外部晶振，野火F407使用 HSE=25M
  RCC_HSEConfig(RCC_HSE_ON);
	
  // 等待HSE启动稳定
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    // 调压器电压输出级别配置为1，以便在器件为最大频率
		// 工作时使性能和功耗实现平衡
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;
		
		// HCLK = SYSCLK / 1
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		// PCLK2 = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		// PCLK1 = HCLK / 4
		RCC_PCLK1Config(RCC_HCLK_Div4);
		
    // 如果要超频就得在这里下手啦
		// 设置PLL来源时钟，设置VCO分频因子m，设置VCO倍频因子n，
		//  设置系统时钟分频因子p，设置OTG FS,SDIO,RNG分频因子q
		RCC_PLLConfig(RCC_PLLSource_HSE, m, n, p, q);
		
		// 使能PLL
		RCC_PLLCmd(ENABLE);
  
	  // 等待 PLL稳定
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }   

/*-----------------------------------------------------*/
    //开启 OVER-RIDE模式，以能达到更高频率(F429才有该功能)
//    PWR->CR |= PWR_CR_ODEN;
//    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
//    {
//    }
//    PWR->CR |= PWR_CR_ODSWEN;
//    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
//    {
//    }      
    // 配置FLASH预取指,指令缓存,数据缓存和等待状态
    FLASH->ACR = FLASH_ACR_PRFTEN 
		            | FLASH_ACR_ICEN 
		            | FLASH_ACR_DCEN 
		            | FLASH_ACR_LATENCY_5WS;
/*-----------------------------------------------------*/
		
		// 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // 读取时钟切换状态位，确保PLLCLK被选为系统时钟
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { // HSE启动出错处理

    while (1)
    {
    }
  }
}
