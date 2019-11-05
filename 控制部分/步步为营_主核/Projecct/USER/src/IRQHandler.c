
#include "headfile.h"
extern int WangToNum;
void PIT0_IRQHandler(void)
{

    static uint16 cnt_1ms=0;//1ms周期
    static uint16 cnt_2ms=0;//2ms周期
    static uint16 cnt_3ms=0;//3ms周期
    static uint16 cnt_5ms=0;//5ms周期
    static uint16 cnt_10ms=0;//10ms周期
    static uint16 cnt_15ms=0;//15ms周期
    static uint16 cnt_50ms=0;//30ms周期
    static uint16 cnt_20ms=0;//20ms周期
    
    static uint16 cnt_30ms=0;//30ms周期
    
    static uint16 cnt_xxms=0;//20ms周期
    static uint16 cnt_1000ms=0;//1000ms周期
    
    if(++cnt_1ms>=1)
    {
        cnt_1ms=0;
//        Loop_check();
//        CountTimeBase();//计时功能调用
    }

    if(++cnt_2ms>=2)//2ms周期
    {     
        cnt_2ms=0;//清标志    
    }
    if(++cnt_3ms>=3)//3ms周期
    {
      MOTOR_get();  //获取速度
      Angle_get();  //获取角度
      cnt_3ms=0;//清标志
    }
    if(++cnt_5ms>=5)//5ms周期
    {   
        SysCtrl_Angle_PID(Angle_to_Ctro,SystemAttitude.Yaw,SysPID[Head_Angle].EnPid_Out);  //角度环
        SysCtrl_Speed_PID_Update();//速度计算及PID控制  //speed N.2
        Car_Move();//速度PID输出赋值  //speed N.4

        cnt_5ms=0;//清标志   
    }

    if(++cnt_10ms>=10)
    {
        if(CarEnterOk == 1)  //车进场之后
        {
            position_pid(now_position,WangToNum);  //speed N.1
            if(SwitchMode == 1)
            {
                Track_PlaceObstacle();  //放障碍
                Servo_StateCtr();
            }
            else
            {
                Track_EightQueen();  //八皇后
//                Servo_StateCtr();
            }
        }
    
        cnt_10ms=0;	
    }

    if(++cnt_15ms>=15)
    {
        
        cnt_15ms=0;
    }

    if(++cnt_20ms>=20)
    {
       if(CarEnterOk == 1)
       {
         if(car_move_state==Car_serching)
           WALK_SPEED=30;
         else if(car_move_state==Car_chessing)
           TimeChangeFreq();
       }
            
        cnt_20ms=0; 
    }

    if(++cnt_50ms>=30)
    {
//        TimeChangeServoSetPwm();//舵机匀速动作
        cnt_50ms=0;
    }

    if(++cnt_xxms>=100)
    {

        cnt_xxms=0;   
            
    }
        
    if(++cnt_1000ms>=1000)
    {       	 
        cnt_1000ms=0;
    }
    
    
      PIT_FlAG_CLR(pit0);



}


/*
*定时器20ms匀速改变频率
*/
#define ChangePwmValue	(16)
void TimeChangeFreq(void)
{
	if(WALK_SPEED>WALK_SPEED_SET)//直接减速
	  WALK_SPEED = WALK_SPEED_SET;
	else if(WALK_SPEED<WALK_SPEED_SET)//均匀加速
	  WALK_SPEED += ChangePwmValue;
		
}



