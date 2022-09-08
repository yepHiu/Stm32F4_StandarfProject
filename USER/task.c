#include "includes.h"

void test_task(void *p_arg)
{
	OS_ERR err;
    p_arg = p_arg;
	//CPU_SR_ALLOC();
	while (1)
	{
		
		DMA_USART2_Send("os test run\r\n",sizeof("os test run\r\n"));
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
	
}