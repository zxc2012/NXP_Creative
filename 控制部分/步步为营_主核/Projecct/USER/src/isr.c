/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr.c
 * @brief      		中断函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/



#include "isr.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PROTA中断执行函数
//  @return     void
//  @since      v1.0
//  Sample usage:               当A口启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void PORTA_IRQHandler(void)
{
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
	PORTA->ISFR = 0xffffffff;
	//使用我们编写的宏定义清除发生中断的引脚
	//PORTA_FLAG_CLR(A1);

}


void PORTC_IRQHandler(void)
{
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
	PORTC->ISFR = 0xffffffff;
	//使用我们编写的宏定义清除发生中断的引脚
	//PORTC_FLAG_CLR(C1);
    VSYNC();
}


void DMA0_IRQHandler(void)
{
	DMA_IRQ_CLEAN(DMA_CH0);
    row_finished();

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART1中断执行函数
//  @return     void
//  @since      v1.0
//  Sample usage:               当UART3启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void UART1_RX_TX_IRQHandler(void)
{
    if(UART1->S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
    {
        //用户需要处理接收数据
        mt9v032_cof_uart_interrupt();
    }
    if(UART1->S1 & UART_S1_TDRE_MASK )                                    //发送数据寄存器空
    {
        //用户需要处理发送数据

    }
}

void UART2_RX_TX_IRQHandler(void)
{
    if(UART2->S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
    {
			UART2_RX_ISR();   
    }
    if(UART2->S1 & UART_S1_TDRE_MASK )                                    //发送数据寄存器空
    {
 

    }
}


/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了DMA0中断，然后就到下面去找哪个是DMA0的中断函数名称，找到后写一个该名称的函数即可
void DMA0_IRQHandler(void)
{
    ;
}
记得进入中断后清除标志位


DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
DMA4_IRQHandler
DMA5_IRQHandler
DMA6_IRQHandler
DMA7_IRQHandler
DMA8_IRQHandler
DMA9_IRQHandler
DMA10_IRQHandler
DMA11_IRQHandler
DMA12_IRQHandler
DMA13_IRQHandler
DMA14_IRQHandler
DMA15_IRQHandler
DMA_Error_IRQHandler
MCM_IRQHandler
FTFL_IRQHandler
Read_Collision_IRQHandler
LVD_LVW_IRQHandler
LLW_IRQHandler
Watchdog_IRQHandler
RNG_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
CAN0_ORed_Message_buffer_IRQHandler
CAN0_Bus_Off_IRQHandler
CAN0_Error_IRQHandler
CAN0_Tx_Warning_IRQHandler
CAN0_Rx_Warning_IRQHandler
CAN0_Wake_Up_IRQHandler
I2S0_Tx_IRQHandler
I2S0_Rx_IRQHandler
CAN1_ORed_Message_buffer_IRQHandler
CAN1_Bus_Off_IRQHandler
CAN1_Error_IRQHandler
CAN1_Tx_Warning_IRQHandler
CAN1_Rx_Warning_IRQHandler
CAN1_Wake_Up_IRQHandler
Reserved59_IRQHandler
UART0_LON_IRQHandler
UART0_RX_TX_IRQHandler
UART0_ERR_IRQHandler
UART1_RX_TX_IRQHandler
UART1_ERR_IRQHandler
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
PDB0_IRQHandler
USB0_IRQHandler
USBDCD_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler
ENET_Receive_IRQHandler
ENET_Error_IRQHandler
Reserved95_IRQHandler
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler
LPTimer_IRQHandler
Reserved102_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
Reserved108_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler
*/



