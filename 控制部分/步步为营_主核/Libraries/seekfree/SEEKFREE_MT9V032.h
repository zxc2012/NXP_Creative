/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_MT9V032.h
 * @brief      		总钻风(灰度摄像头)函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note
					MT9V032接线定义：
					------------------------------------
						模块管脚            单片机管脚
						SDA(51的RX)         C17
						SCL(51的TX)         C16
						场中断              C6
						行中断				未使用，因此不接
						像素中断            C18
						数据口              C8-C15
					------------------------------------

					默认分辨率是            188*120
					默认FPS                 50帧
 ********************************************************************************************************************/



#ifndef _MT9V032_h
#define _MT9V032_h

#include "headfile.h"


extern uint8 *imgbuff;
extern uint8 nrf_tx_buff[];
extern volatile int threshold;
/*移到了edge_extract.h中，在edge_extract.h中修改*/
//#define COL     128//图像宽度   范围1-752     K60采集不允许超过188
//#define ROW     64//图像高度	范围1-480
extern uint8 img[ROW][COL];
extern uint8 img1[];

//摄像头命令枚举
typedef enum
{
    INIT = 0,               //摄像头初始化命令
    AUTO_EXP,               //自动曝光命令
    EXP_TIME,               //曝光时间命令
    FPS,                    //摄像头帧率命令
    SET_COL,                //图像列命令
    SET_ROW,                //图像行命令
    LR_OFFSET,              //图像左右偏移命令
    UD_OFFSET,              //图像上下偏移命令
    GAIN,                   //图像偏移命令
    CONFIG_FINISH,          //非命令位，主要用来占位计数

    SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
    GET_STATUS,             //获取摄像头配置命令
    GET_VERSION,            //固件版本号命令

    SET_ADDR = 0XFE,        //寄存器地址命令
    SET_DATA                //寄存器数据命令
}CMD;



extern uint8 mt9v032_finish_flag;  //一场图像采集完成标志位
extern uint8 image[ROW][COL];      //内部使用


//引脚配置

#define MT9V032_COF_UART    uart1  //配置摄像头所使用到的串口


//--------------------------------------------------------------------------------------------------
//        如果修改了场中断引脚那么注意修改PORTC_IRQn、PORTC_ISFR。
//        比如使用的是PTA这组的引脚那么将这两个修改为PORTA_IRQn、PORTA_ISFR即可
//        且务必在对应的中断函数中调用VSYNC函数
//--------------------------------------------------------------------------------------------------
#define MT9V032_PCLK                A19                            	//摄像头像素时钟

#define MT9V032_VSYNC               C7                            	//摄像头场同步信号
#define MT9V032_VSYNC_CHANNEL       7                               //MT9V032_VSYNC对应引脚的引脚号 PTC6   -->     6
#define MT9V032_INTERRUPT_NUNBERS   PORTC_IRQn                      //中断编号
#define MT9V032_INTERRUPT_ISFR      PORTC->ISFR                     //中断标志位

//--------------------------------------------------------------------------------------------------
//        摄像头数据接口
//        C_IN_DATA(1)
//        第一个字母C代表着使用C口，可以选择A、B、C、D、E.分别对应A_IN_DATA，B_IN_DATA，C_IN_DATA，D_IN_DATA，E_IN_DATA
//        第二个括号里面的参数1代表着使用的是C8 - C15口,
//		  可以选择0、1、2、3，分别对应的是	该端口的X0-X7，X8-X15，X16-X23，X24-X31(这里x代表的是第一个字母，比如现在的就是C8-C15)
//--------------------------------------------------------------------------------------------------

#define MT9V032_DATAPORT 		B_IN_DATA(2)	                //DMA数据口

void   mt9v032_cof_uart_interrupt(void);
void   VSYNC(void);
void   camera_init(void);
void   seekfree_sendimg_032(void);
void   row_finished(void);
void   MT9V032_Get_Img(void);
void   img_recontract(uint8 *dst, uint8 *src, uint32 srclen);
void   img_extract(uint8 *dst, uint8 *src, uint32 srclen);
void   seekfree_sendimg(void *imgaddr, uint32_t imgsize);
void    Picture_display(void);
void   img_extract(uint8 *dst, uint8 *src, uint32 srclen);
void   img_recontract(uint8 *dst, uint8 *src, uint32 srclen);

#endif

