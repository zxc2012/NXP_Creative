#include "headfile.h"
//extern uint8 Data_State;/*主从机传送数据状态*/
uint8 remain_x;
uint8 remain_y;
void Connunication_init(void)   //双机通信
{
	uart_init (uart2, 115200);
	set_irq_priority(UART2_RX_TX_IRQn,0);	//中断优先级0						
	uart_rx_irq_en(uart2);
}

/*主机接收从机编码器数值函数，放在串口中断里面*/    
/*如果需要增加接收的数据，修改①②③④⑤⑥地方*/
void UART2_RX_ISR(void)              
{
	static uint8 Temp[8]={0};  //蓝牙数据变量												//①  若需要增加数据，改Temp数组大小
	static uint8 state = 0;/* 接收状态 */
	static uint8 _data_len = 6,_data_cnt = 0;                       //②  若需要增加数据，改_data_len的大小
	uint8 byteReceived;
	uart_getchar(uart2,&byteReceived);
	if(byteReceived == 0xfe && state==0)                              //标志位与从机对应
	{
		state = 1;
		Temp[0] = byteReceived;
	}			
	else if(byteReceived == 0xfa && state==1)													 //标志位与从机对应
	{
		state=2;
		Temp[1] = byteReceived;
	}
	else if(state == 2&&_data_len>0)
	{
		_data_len--;
		Temp[2+_data_cnt++] = byteReceived;
		if(_data_len==0)
			state = 3;
	}
	else if(state == 3)
	{
		if(byteReceived == 0xff)  																			 //标志位与从机对应
		{
			//处理函数
			MotorStatus_t.LB_Spd = ((int)(Temp[2]<<8)&0xff00)|Temp[3];         //③ 若需要增加数据，改处理函数的内容
			MotorStatus_t.RB_Spd = ((int)(Temp[4]<<8)&0xff00)|Temp[5];	
			remain_x = Temp[6];
			remain_y = Temp[7];
                        
			if(MotorStatus_t.LB_Spd >30000)     //编码器是数不可能超过30000
				MotorStatus_t.LB_Spd = MotorStatus_t.LB_Spd-65535;	
			if(MotorStatus_t.RB_Spd >30000)
				MotorStatus_t.RB_Spd = MotorStatus_t.RB_Spd-65535;
			
			state = 0;
			_data_len = 6;                                                 //④若需要增加数据，改_data_len的大小
			_data_cnt = 0;
                        
                        Speed_Value_LB+=MotorStatus_t.LB_Spd;
                        Speed_Value_RB+=MotorStatus_t.RB_Spd;
			return;
		}
		else    //若传输出错，则从头传输
		{
			_data_cnt = 0;
			_data_len = 6;																							  //⑤若需要增加数据，改_data_len的大小
			state = 0;
			return;
		}

	}
	else
	{
		state = 0;
		_data_len = 6;                                                  //⑥若需要增加数据，改_data_len的大小
		_data_cnt = 0;
	}
}


/*
*  发送数据给副核，执行对应任务
*  1：清空正交解码  2：发送一次速度
*/
void Communication_send(uint8 num)
{
    uint8 data_to_send[4];                             //若需增加数据，数组大小要修改
    data_to_send[0] = 0XFE;
	  data_to_send[1] = 0XFA;
		data_to_send[2] = num;     //若需增加数据，发送内容要修改	
                      data_to_send[3] = 0xff;	
   uart_putbuff(uart2, (uint8_t *)data_to_send, 4);    //若需增加数据，传输字节数要修改
}

