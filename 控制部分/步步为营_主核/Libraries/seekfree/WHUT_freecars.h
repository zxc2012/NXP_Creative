/*
 * @file       Freecars上位机协议次层
 * @brief      2017nxp
 * @author     liwei
 * @version    v0.1
 * @date       2016.5.10
 */
#ifndef __Freecars_H_
#define __Freecars_H_

#define UARTPort uart4
#define BAUD 115200
#define UartDataNum      17	//FreeCars上位机 串口数据显示框 接收通道数，按照上位机设置改变

#define UartRxBufferLen  100
#define UartRxDataLen    41           //FreeCars上位机发送浮点数据MCU接收，不要改
#define UartRxCmdLen     7	      //FreeCars上位机接收命令数据长度，不要改

#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]//命令号
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]//命令数据


//尽管数据的长度只要求数组长度为26=22+3+1；但是为了与发送字符串取得统一
//取数据的长度与字符串的长度相等，这样在发送数据是会多发送了一些
//无效位，但是影响不大的
#include "common.h"
#include "MK60_uart.h"
typedef struct
{
  int Stack;
  uint8 Data;
  uint8 PreData;
  uint8 Buffer[UartRxBufferLen];
  uint8 Enable;
  uint8 Check;
}SerialPortType;

extern double UartData[9];


extern void UartDebug(void);
extern void UartCmd(uint8 CmdNum,uint8 Data);
extern void freecars_init(void);
extern void tofreecars(void);
extern void push(uint8 chanel,uint16 data);
extern void freecars_img(uint8 imgData[],uint16 img_H,uint16 img_W);
extern void freecars_push(void);

#endif
