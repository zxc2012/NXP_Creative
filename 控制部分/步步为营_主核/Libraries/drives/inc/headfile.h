/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		headfile
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN and MK60FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/



#ifndef _headfile_h
#define _headfile_h


#include "MK60_port_cfg.h"
#include "common.h"
#include "misc.h"
#include "stdio.h"
#include "edge_extract.h"
#include "image_binaryzation.h"
#include "math.h"
#include "WHUT_freecars.h"
#include "Init.h"
#include "menu.h"
#include "lcd.h"
#include "VCAN_key.h"
#include "Moto.h"
#include "Encoder.h"                                    //编码器
#include "Attitude_Calculation.h"                       //姿态解算
#include "BMX055.h"                                     //陀螺仪1
   
#include "FXAS2100_FXOS8700.h"                          //陀螺仪2

#include "DataStore.h"                                  //Flash存储
#include "Communication.h"                              //双机通信
#include "ctrl_pid.h"
#include "Pos_cal.h"
#include "VCAN_Track.h"
#include "Find_path.h"
#include "ALL_solution.h"
#include "Switch.h"                                     //模式
#include "VCAN_Servo.h"                                 //舵机
#include "CtrDelay.h"                                   //继电器
#include "CtrSpeed.h"
#include "duty.h"

//--------函数库--------
#include "MK60_rtc.h"
#include "MK60_gpio.h"
#include "MK60_systick.h"
#include "MK60_port.h"
#include "MK60_uart.h"
#include "MK60_pit.h"
#include "MK60_lptmr.h"
#include "MK60_ftm.h"
#include "MK60_adc.h"
#include "MK60_dac.h"
#include "MK60_flash.h"
#include "MK60_spi.h"
#include "MK60_i2c.h"
#include "MK60_wdog.h"
#include "MK60_dma.h"
#include "MK60_cmt.h"
#include "MK60_sdhc.h"

#include "isr.h"
#include "IRQHandler.h"

//fatfs文件系统
#include "ff.h"

//--------逐飞科技产品例程库--------
#include "SEEKFREE_MT9V032.h"
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_TSL1401.h"
#include "SEEKFREE_7725.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_NRF24L01.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_MPU6050.h"

extern uint8 test_flag;
extern uint8 serch_step;
extern uint8 cal_n_serch;
#endif
