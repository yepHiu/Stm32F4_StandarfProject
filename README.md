<!--
 * @Author: 奶茶不加冰
 * @Date: 2022-08-01 20:21:48
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-08-03 16:08:30
 * @FilePath: \2022-08f:\资料整理\STM32F4\stm32f407ve_prj\stm32飞行逻辑控制单元V0.5.5_回滚_中断配置完毕RE_V1.3_任务运行测试\README.md
 * @Description: 
-->
# STM32F4系列芯片开发标准工程
***
## 该工程概述  
* 芯片： stm32f407vet6  
* RTOS: UCOS-III
* 已经完成初始化的外设:  
    * TIM1
    * USART1~6(使用DMA进行发送，相关的DMA配置完毕) 
    * I2C协议(软件模拟)
    * SPI(硬件配置)
    * ADC(同样完成ADC相关DMA配置)
    * DAC

