/*
 * @file       Freecars上位机协议次层
 * @brief      whut2
 * @author     whut_ycy
 * @version    v2.0
 * @date       2019.1.14
 * @readme     添加了NRF传输协议,需要调用逐飞库
 */
#include "headfile.h"



/************以下是根据FreeCars2.0协议修正的，用户无需改变**************/
uint8 uSendBuf[UartDataNum*2]={0};
uint8 FreeCarsDataNum=UartDataNum*2;

double UartData[9] = {0};
SerialPortType SerialPortRx;
extern uint32 pulse;
extern int WangToNum;
/*
上位机传回参数后，用此函数赋值给需要的变量名 如Kp
*/
void UartDebug(void)
{
//                   SysPID[Head_Angle].p=UartData[0];
//                   SysPID[Head_Angle].d=UartData[1];
//                Y_Speed=UartData[2];
//                X_Speed=UartData[3];
//                PID_K[2]=UartData[2];

//                SysPID[position].p = UartData[0];		//方向P
//                SysPID[position].d = UartData[1];		//方向D
                CarEnterOk= UartData[2];
//                WangToNum=UartData[3];

}

void freecars_push(void)
{
//   push(0,middle_Error[0]);
//   push(1,Direction_Control_Out);
//   push(2,kp_out);
//   push(3,kd_out);
//   push(4,car_v);
//   push(5,Speed_Set);
//   push(0,YH_Crossroad);
//   push(1,you_zhixian);
//   push(2,break_row);
//   push(3,chengxu_flag);
//   push(4,third_point[0]);
//   push(5,third_point[1]);
   tofreecars();
}


/*
命令接收函数
CmdNum：0~255号命令
DATA  ：0~255个命令所对应的命令字
CmdNum    Data          对应快捷键
  1         6               F6
  1         7               F7
  1         8               F8
  1         9               F9
  1         10               F10
  1         11               F11
  1         12               F12
  2         100             Pause
  2         101             Home
  2         102             Pg up
  2         103             Pg dn
  2         104             end
*/


void UartCmd(uint8 CmdNum,uint8 Data)
{
  if(CmdNum == 1 && Data == 6)
  {
//       NVIC_DisableIRQ(PIT0_IRQn);
//            ftm_pwm_duty(ftm0,ftm_ch1,5000);              //双极性PWM互补输出
//            ftm_pwm_duty(ftm0,ftm_ch0,5000);
//
//            ftm_pwm_duty(ftm0,ftm_ch3,5000);
//            ftm_pwm_duty(ftm0,ftm_ch2,5000);
  }
  if(CmdNum == 1 && Data == 7)
  {
    NVIC_EnableIRQ(PIT0_IRQn);
  }
//  if(CmdNum == 1 && Data == 8)
//  {
//    push(1,27);
//  }
//  if(CmdNum == 1 && Data == 9)
//  {
//    push(1,27);
//  }
//  if(CmdNum == 1 && Data == 10)
//  {
//    push(1,27);
//  }
//  if(CmdNum == 1 && Data == 11)
//  {
//    push(1,27);
//  }
}

/*
轮询法发送一帧数据
消耗时间与数据长度有关
不可以放在中断里面周期性调用
消耗时间计算看帮助文档
*/
void tofreecars(void)
{
  uint8 i,sum=0;
  //使用轮询的方式发送数据，当数据未发送，程序停在此处直到发送完成
  uart_putchar(UARTPort,251);
  uart_putchar(UARTPort,109);
  uart_putchar(UARTPort,37);
  sum+=(251);      //全部数据加入校验
  sum+=(109);
  sum+=(37);
  for(i=0;i<FreeCarsDataNum;i++)
  {
    uart_putchar(UARTPort,uSendBuf[i]);
    sum+=uSendBuf[i];         //全部数据加入校验
  }
  uart_putchar(UARTPort,sum);
}
/*******nrf发送函数******/
//void tonrf(void)
//{
//  uint8 i,sum=0;
//  //使用轮询的方式发送数据，当数据未发送，程序停在此处直到发送完成
////  UART_Put_Char(FreeCarsUARTPort,251);
////  UART_Put_Char(FreeCarsUARTPort,109);
////  UART_Put_Char(FreeCarsUARTPort,37);
//  NRF_TX_Buff[0] = 251;
//  NRF_TX_Buff[1] = 109;
//  NRF_TX_Buff[2] = 37;
//  sum+=(251);      //全部数据加入校验
//  sum+=(109);
//  sum+=(37);
//  for(i=0;i<FreeCarsDataNum;i++)
//  {
//  //  UART_Put_Char(FreeCarsUARTPort,uSendBuf[i]);
//    NRF_TX_Buff[i+3]=uSendBuf[i];
//    sum+=uSendBuf[i];         //全部数据加入校验
//  }
// // UART_Put_Char(FreeCarsUARTPort,sum);
//  NRF_Send_Packet(NRF_TX_Buff);
//  systick_delay_ms(2);
//}
//发送一幅图像到Freecars上位机上   80*60
void freecars_img(uint8 imgData[],uint16 img_H,uint16 img_W)
{
  int16 i,j;
  uint16 offset;
  uart_putchar(UARTPort,0xFF);//FF,FE,FF,FE四个数表示一幅图像的开始
  uart_putchar(UARTPort,0xFE);
  uart_putchar(UARTPort,0xFF);
  uart_putchar(UARTPort,0xFE);

//  for(i = 0;i < img_H;i++)
//  {
//    offset = i*img_W;
//    for(j = 0;j < CAMERA_W;j++)

  for(i = img_H - 1;i >=0;i--)
  {
    offset = i*img_W;
    for(j = img_W - 1;j >= 0;j--)
    {
      uint8 d = imgData[offset + j];
      if(d > 0xFD) d = 0xFD;            //避开校验位
      uart_putchar(UARTPort,d);
    }
    uart_putchar(UARTPort,0xFE);//FE,FE 2个数表示换行
    uart_putchar(UARTPort,0xFE);
  }
  j=0;
}
/*
向某个通道填充数据
adr：通道
date：数据-32768~32767(2^8-1)
*/
void push(unsigned char adr,uint16 data)
{
  uSendBuf[adr*2]=data/256;
  uSendBuf[adr*2+1]=data%256;
}
//#if define longqiu
//void UART4_IRQHandler(void)
//#if define zhufei
void UART4_RX_TX_IRQHandler(void)

{
  int i,b,d1;
  uint32 d;

  //SerialPortRx.Data=uart_getchar(UARTPort);
  uart_getchar(UARTPort,&SerialPortRx.Data);


  if( SerialPortRx.Stack < UartRxBufferLen )
  {
    SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;

    if(   SerialPortRx.Stack >= UartRxDataLen  //UartRxDataLen个数为一帧
       && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen]  ==0xff //校验字头
         && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+1]==0x55
           && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+2]==0xaa
             && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen+3]==0x10 )
    {   //double data 9个通道数据校验
      SerialPortRx.Check = 0;
      b = SerialPortRx.Stack - UartRxDataLen; //起始位
      for(i=b; i<SerialPortRx.Stack-1; i++)  //除校验位外的位进行校验
      {
        SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
      }

      if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1] )
      {   //校验成功，进行数据解算
        for(i = 0; i<9; i++)
        {
          d = SerialPortRx.Buffer[b+i*4+4]*0x1000000L + SerialPortRx.Buffer[b+i*4+5]*0x10000L + SerialPortRx.Buffer[b+i*4+6]*0x100L + SerialPortRx.Buffer[b+i*4+7];
          if(d>0x7FFFFFFF)
          {
            d1 = 0x7FFFFFFF - d;
          }
          else
          {
            d1 = d;
          }
          UartData[i]=d1;
          UartData[i]/=65536.0;
        }
        UartDebug();  //转去处理，把受到的数据付给变量
      }
      SerialPortRx.Stack = 0;
    }
    else if(   SerialPortRx.Stack >= UartRxCmdLen //UartRxDataLen个数为一帧
            && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen]  ==0xff
              && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+1]==0x55
                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+2]==0xaa
                  && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen+3]==0x77 )//cmd
    {
      SerialPortRx.Check = 0;
      b = SerialPortRx.Stack - UartRxCmdLen; //起始位
      for(i=b; i<SerialPortRx.Stack-1; i++)  //除校验位外的位进行校验
      {
        SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
      }
      if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack-1] )
      {   //校验成功
        UartCmd(UartCmdNum,UartCmdData);//处理接收到的命令，付给MCU命令变量
      }
      SerialPortRx.Stack = 0;
    }
  }
  else
  {
    SerialPortRx.Stack = 0;
  }
}

void freecars_init(void)
{
  uart_init(UARTPort,BAUD*2);//打开所用串口
  uart_rx_irq_en(UARTPort);//使能接收中断
  set_irq_priority(UART4_RX_TX_IRQn,0);
  //set_vector_handler(FreeCarsVECTORn,Freecars_IRQHandler);//设置中断函数
}
/*!
 *  @brief      山外多功能调试助手上位机，摄像头显示函数
 *  @param      imgaddr    图像起始地址
 *  @param      imgsize    图像占用空间的大小
 *  @since      v5.0
*  Sample usage:
             具体用法参考这帖子:
            【山外摄像头】鹰眼上位机例程和微焦效果 - 智能车资料区
             http://vcan123.com/forum.php?mod=viewthread&tid=6242&ctid=27
 */
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    uart_putbuff(UARTPort, cmdf, sizeof(cmdf));    //先发送命令

    uart_putbuff(UARTPort, (uint8 *)imgaddr, imgsize); //再发送图像

    uart_putbuff(UARTPort, cmdr, sizeof(cmdr));    //先发送命令
}


/////发送图像数据
////不限制发送的是一位图还是八位图，图像格式的设置在上位机端设置
void SendPicData(UARTn uartn, uint8 * data, uint16 size)
{   uint8 SendHead[4] = {0xFF, 0xFA, 0xFD, 0xF6};
  uint8 psize[2] = {0};
  if(size >= 256)
    psize[0] = size / 256;
  psize[1] = size - 256 * (uint16)psize[0];

  uart_putbuff(uartn, SendHead, 4);
  uart_putchar(uartn, 250);
  uart_putbuff(uartn, psize, sizeof(psize));
  uart_putbuff(uartn, data, size);
  uart_putchar(uartn, 0x0A);  ////最后一位校验
}
