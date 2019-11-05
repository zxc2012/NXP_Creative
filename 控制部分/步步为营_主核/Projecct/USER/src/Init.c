#include "headfile.h"

uint8 flag1=0;
uint8 start_flag=0;

void all_init()
{
   camera_init();
//   IIC_init();         //模拟IIC端口初始化
//   InitMPU6050();      //初始化MPU6050
//   GPIO_init();          //按键 遥控初始化
//   initOLED();
//   FLASH_Init();       //FLASH初始化
//   Moto_Init();          //电机初始化
//   Encoder_Init();       //编码器初始化
//   Connunication_init(); //双核通讯初始化
   freecars_init();      //上位机初始化

}


void GPIO_init()
{
//
//   //贴片按键初始化
//   gpio_init (G5,GPI,0);            //K0
//   gpio_init (G6,GPI,0);            //K1
//   gpio_init (G7,GPI,0);            //K2

//      key_init(KEY_MAX);            //五向开关初始化
//
    gpio_init (B8, GPI,0);        //遥控
    gpio_init (B9, GPI,0);
    gpio_init (B10, GPI,0);
    gpio_init (B11, GPI,0);

//   gpio_init(A9,GPO,0);    //蜂鸣器


}



//---------------中断初始化------------------//
void ISR_init(void)
{
  
    pit_init_ms(pit0,1);                            //初始化PIT0，定时时间为： xms
    set_irq_priority(PIT0_IRQn,2);
    enable_irq(PIT0_IRQn);                            //使能PIT0中断
  

}



