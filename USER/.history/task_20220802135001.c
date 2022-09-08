#include "includes.h"

DroneRTInfo RT_Info;
SensorDataSend Send_Sensor;
fly_mode_e fly_mode;


void test_task(void *p_arg)
{
	OS_ERR err;
    p_arg = p_arg;
	//CPU_SR_ALLOC();
	while (1)
	{
		
		//DMA_USART2_Send("os test run\r\n",sizeof("os test run\r\n"));
		//OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
	
}

void CTR_task(void *p_arg)
{
	OS_ERR err;
    p_arg = p_arg;
	ctr_init();
	while (1)
	{
		ctr_process();
		//DMA_USART2_Send("os  ctr test run\r\n",sizeof("os  ctr test run\r\n"));
		//OSTimeDlyHMSM(0,0,0,610,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void SEND_task(void *p_arg)
{
	OS_ERR err;
    p_arg = p_arg;
	CPU_SR_ALLOC();
	while (1)
	{
		if(RT_Info.drone_start_flag==1)
		{
			OS_CRITICAL_EXIT();
			Send_Importance();
			OS_CRITICAL_EXIT();

			OSTimeDlyHMSM(0,0,0,4,OS_OPT_TIME_HMSM_STRICT,&err);

			OS_CRITICAL_ENTER();
			switch ((uint8_t)fly_mode)
			{
			case 0x00:
				Send_Headmode();
				break;
			case 0x01:
				Send_Pointmode();
				break;
			case 0x02:
				Send_Flowmode();
				break;
			case 0x03:
				Send_Linemode();
				break;
			default:
				break;
			}
			OS_CRITICAL_EXIT();
			OSTimeDlyHMSM(0,0,0,3,OS_OPT_TIME_HMSM_STRICT,&err);
			OS_CRITICAL_ENTER();
			Send_ctr();
			OS_CRITICAL_EXIT();
			OSTimeDlyHMSM(0,0,0,3,OS_OPT_TIME_HMSM_STRICT,&err);
		}
		else
		{
			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
		}
		//DMA_USART2_Send("os send test run\r\n",sizeof("os send test run\r\n"));						
		//OSTimeDlyHMSM(0,0,0,700,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void CMD_task(void *p_arg)
{
	OS_ERR err;
    p_arg = p_arg;
	RT_Info.drone_start_flag=0;
	while (1)
	{	
		Cmd_Process();
		//DMA_USART2_Send("os cmd test run\r\n",sizeof("os cmd test run\r\n"));
		//OSTimeDlyHMSM(0, 0, 0,900, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}



OS_SEM uart1_proc;
OS_SEM uart1_tofly;
_Data_Rx drone_msg = {0};
void UART1_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;

    OSSemCreate((OS_SEM *)&uart1_proc,
                (CPU_CHAR *)"uart1_proc",
                (OS_SEM_CTR)1,
                (OS_ERR *)&err);

    OSSemCreate((OS_SEM *)&uart1_tofly,
                (CPU_CHAR *)"uart1_tofly",
                (OS_SEM_CTR)1,
                (OS_ERR *)&err);

    while(1)
    {
        err = OS_ERR_NONE;
		OSSemPend (&uart1_proc,100,OS_OPT_PEND_BLOCKING,0,&err); //ti??????
		if(err == OS_ERR_TIMEOUT)
		{
			
		}
		else if(err == OS_ERR_NONE)
		{
			
		}

        memcpy(drone_msg.buf,uart1_rx.buf,sizeof(uart1_rx.buf));
		drone_msg.len = uart1_rx.len;

        for(unsigned int cnt = 0; cnt < drone_msg.len; cnt ++)
		{
			if(drone_msg.buf[cnt] == 0xff && drone_msg.buf[cnt+1] == 0xee && drone_msg.buf[cnt+15] == 0xcc) //????????
			{
				if(drone_msg.buf[cnt+2] == 0x01)
				{
					Cmd_Data.Cmd_Msg = (drone_msg.buf[cnt+4] << 8) | drone_msg.buf[cnt+3];
					Cmd_Data.beep_power = drone_msg.buf[cnt+5];
					
					Cmd_Data.flag_bits = drone_msg.buf[cnt+6];
					
					RT_Info.yaw = UnsignedcharToFloat(drone_msg.buf,cnt+7);
					RT_Info.height = UnsignedcharToFloat(drone_msg.buf,cnt+11);
				}
				else if(drone_msg.buf[cnt+2] ==0x02)
				{
					RT_Info.pitch = UnsignedcharToFloat(drone_msg.buf,cnt+7);
					RT_Info.roll = UnsignedcharToFloat(drone_msg.buf,cnt+11);
				}
				else if(drone_msg.buf[cnt+2] == 0x03)
				{
					RT_Info.flowx = UnsignedcharToFloat(drone_msg.buf,cnt+7);
					RT_Info.flowy = UnsignedcharToFloat(drone_msg.buf,cnt+11);
					
					RT_Info.flowx *= RT_INFO_FLOWX_BIAS;
					RT_Info.flowy *= RT_INFO_FLOWY_BIAS;
				}
				break;
			}
		}

    }



}

OS_SEM uart2_proc; 
_Data_Rx u_cam_data = {0};
void UART2_task(void *p_arg)//ucam
{
    OS_ERR err;
    p_arg = p_arg;

	OSSemCreate ((OS_SEM*)&uart2_proc,
							(CPU_CHAR*)"uart2_proc",
							(OS_SEM_CTR)1,
							(OS_ERR*)&err);

while(1)
	{
		
		OSSemPend (&uart2_proc,0,OS_OPT_PEND_BLOCKING,0,&err);
		memcpy(u_cam_data.buf,uart2_rx.buf,sizeof(uart2_rx.buf));
		u_cam_data.len = uart2_rx.len;
		u_cam_datadeal(u_cam_data);
	}

}


OS_SEM uart3_proc;
_Data_Rx h_cam_data = {0};
void UART3_task(void *p_arg)//hcam
{
    OS_ERR err;
    p_arg = p_arg;

	OSSemCreate ((OS_SEM*)&uart3_proc,
							(CPU_CHAR*)"uart3_proc",
							(OS_SEM_CTR)1,
							(OS_ERR*)&err);

    while(1)
	{
		OSSemPend (&uart3_proc,0,OS_OPT_PEND_BLOCKING,0,&err);
		memcpy(h_cam_data.buf,uart3_rx.buf,sizeof(uart3_rx.buf));
		h_cam_data.len = uart3_rx.len;
		h_cam_datadeal(h_cam_data);
	}
}

OS_SEM uart4_proc;
_Data_Rx Flow_data = {0};
void UART4_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;

	OSSemCreate ((OS_SEM*)&uart4_proc,
							(CPU_CHAR*)"uart4_proc",
							(OS_SEM_CTR)1,
							(OS_ERR*)&err);

    while(1)
	{
		OSSemPend (&uart4_proc,0,OS_OPT_PEND_BLOCKING,0,&err);
		memcpy(Flow_data.buf,uart4_rx.buf,sizeof(uart4_rx.buf));
		Flow_data.len = uart4_rx.len;
		Flow_datadeal(Flow_data);
	}
}

OS_SEM uart5_proc;
_Data_Rx Tof_data = {0};
void UART5_task(void *p_arg)
{

    OS_ERR err;
    p_arg = p_arg;
    OSSemCreate((OS_SEM *)&uart5_proc,
                (CPU_CHAR *)"uart5_proc",
                (OS_SEM_CTR)1,
                (OS_ERR *)&err);

    while (1)
    {
        OSSemPend(&uart5_proc, 300, OS_OPT_PEND_BLOCKING, 0, &err);
        if (err == OS_ERR_TIMEOUT)
            Send_Sensor.height_valid = 0x00;
        else if (err == OS_ERR_NONE)
        {
            memcpy(Tof_data.buf, uart5_rx.buf, sizeof(uart5_rx.buf));
            Tof_data.len = uart5_rx.len;
            TOF_DataDeal(Tof_data);
        }
    }
}
