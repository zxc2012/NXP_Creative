/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_FUN.h
 * @brief      		扩展功能函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/



#ifndef _SEEKFREE_FUN_h
#define _SEEKFREE_FUN_h

#include "headfile.h"


int   myabs(int dat);
void  my_delay(long t);
float limit(float x, uint16 y);
int16 limit_ab(int16 x, uint16 a, uint16 b);
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define XIANFU(x,lb,ub)   ((x)>=(ub)?(ub):(x<=(lb)?(lb):(x))) //限幅，闭区间











#endif
