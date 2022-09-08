/*
 * @Author: 奶茶不加冰
 * @Date: 2022-07-25 13:03:01
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-08-06 16:51:49
 * @FilePath: \stm32飞行逻辑控制单元V0.5.5_回滚_中断配置完毕RE_V1.3_任务运行测试 - 副本\DRONE\flow\flow.c
 * @Description:
 */
#include "flow.h"

void Flow_datadeal(_Data_Rx rx)
{
    static int16_t tmpX,  tmpY = 0;

    uint16_t FlowTime = 0;
    //优象光流
    for (unsigned int cnt = 0; cnt < rx.len; cnt++)
    {
        if (rx.buf[cnt] == 0xfe && rx.buf[cnt + 1] == 0x0A && rx.buf[cnt + 13] == 0x55)
        {
            Send_Sensor.flow_eff = rx.buf[cnt + 10];
            if (rx.buf[cnt + 10] == 0xf5)
            {

                tmpX = ((rx.buf[cnt + 3] << 8) + rx.buf[cnt + 2]);
                tmpY = ((rx.buf[cnt + 5] << 8) + rx.buf[cnt + 4]);
                FlowTime = ((uint16_t)(rx.buf[cnt + 7] << 8) + rx.buf[cnt + 6]);
                
                Send_Sensor.tmpx = tmpX;
                Send_Sensor.tmpy = tmpY;
                Send_Sensor.flytime_tmp = FlowTime;
                Send_Sensor.flow_valid = 0xaa;

                if (ctr.src_flow_ban == 1)
                {
                    Send_Sensor.flow_valid = 0x00;
                }
            }
            else
            {
                Send_Sensor.tmpx = 0;
                Send_Sensor.tmpy = 0;
                Send_Sensor.flytime_tmp = 20800;
                Send_Sensor.flow_valid = 0x00;
            }
            break;
        }
        else
        {
            Send_Sensor.tmpx = 0;
            Send_Sensor.tmpy = 0;
            Send_Sensor.flytime_tmp = 20800;
            Send_Sensor.flow_valid = 0x00;
        }
    }
}
