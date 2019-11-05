/********************************************
逐飞科技 总钻风-摄像头  历程
Designed by Fly Sir
软件版本:V1.1
最后更新:2016年5月3日
相关信息参考下列地址：
淘宝店：https://seekfree.taobao.com/
------------------------------------
软件版本： IAR 7.2 or MDK 5.17
目标核心： MK60DN512VLL10
============================================
MT9V032接线定义：
------------------------------------
	模块管脚            单片机管脚
    SDA(51的RX)         C17
    SCL(51的TX)         C16
	场中断              C6
    行中断				未使用，因此不接
    像素中断            C18
    数据口              C8-C15

    串口
    波特率 115200
    数据位 8
    校验位 无
    停止位 1位
    流控   无
	串口连接注意事项：切勿使用蓝牙等无线串口连接
    RX                  D3
    TX                  D2
============================================

分辨率是                188*120
摄像头参数设置可以到    SEEKFREE-->h_file-->SEEKFREE_MT9V032.h

总钻风-摄像头测试步骤：
1.下载程序到开发板
2.插上串口线或者USB转TTL
3.接好MT9V032模块接线
4.通电在TFT液晶上即可观看
*********************************************/
#include "headfile.h"
extern int WangToNum;
uint8 test_flag=0;
uint8 speed_mode = 0;
xy_t start_car_set={0,0};
xy_t start_car[28] = {  {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},
                        {7,0},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7},
                        {1,0},{2,0},{3,0},{4,0},{5,0},{6,0},
                        {1,7},{2,7},{3,7},{4,7},{5,7},{6,7},
};

xy_t S_s[23] = {24,40,34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8 serch_step=3;
uint8 cal_n_serch=0;
extern int16 Y_Speed,X_Speed;
int main(void)
{
    get_clk();            //配置时钟
    camera_init();        //摄像头初始化
    Data_init();
    IIC_init();           //IIC初始化
    while(!FXAS2100_FXOS8700_init());//陀螺仪初始化
    freecars_init();      //上位机初始化
    Connunication_init(); //双机通信初始化
    Encoder_Init();       //编码器初始化
    Moto_Init();          //电机初始化
    InitServo();          //舵机初始化
    InitRelay();          //继电器初始化
    GPIO_init();
    initOLED();           //oled初始化
    
    //参数初始化
//    trigonometric_function_int();    //三角函数初始化
    InitSpeedWithDistance();
    PID_Parameter_Init();            //速度环、位置环参数更新
    SwitchMode=15;
    
    ISR_init();           //时间中断初始化
//    car_serch();
//    Car_EnterTo();
      now_position.x=0;
      now_position.y=0;
    for(;;)
    {
      car_move_state=Car_chessing;
//      ServoState[0]=Servo_Up;

      WangToNum=10;
//      X_Speed=0;
//      Y_Speed=30;
      En_Update_PID=Yes; 
      ftm_pwm_duty(ftm3, ftm_ch0, 725);
//      CarEnterOk =1;
      
//      SysPID[position].EnPid_Out=No;    //关位置环
//      En_Update_PID=No;
//      Angle_to_Ctro=0;
      if(gpio_get(B8))
      {
        for(;;)
        {
          En_Update_PID=No;
          if(gpio_get(B9)) break;
        }
      }
//      Moto_Set_Pwm(LH,-3000);
//      Moto_Set_Pwm(RH,-3000);
//      Moto_Set_Pwm(LB,-3000);
//      Moto_Set_Pwm(RB,-3000);
      push(0,WALK_SPEED);
      push(1,WALK_SPEED_SET);
      push(2,X_Speed);
//      push(3,SysPID[position].err);
//      push(4,SysPID[position].set);
//      push(5,SysPID[position].sample);
      tofreecars();

		if(mt9v032_finish_flag)
        {

            mt9v032_finish_flag = 0;
//            Hough_Transfom(65);
//            edge_extract();
//            img_binary();                                //二值化处理函数
//            Get_Road();                                   //边沿提取
//            freecars_img(*img,ROW,COL);
//            seekfree_sendimg_032();                    //发送至逐飞上位机
//            uart_putchar(uart4,0x00);

       }

    }
}
