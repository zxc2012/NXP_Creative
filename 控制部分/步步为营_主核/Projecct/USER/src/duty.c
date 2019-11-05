#include "headfile.h"

extern void Num_To_PidSetValue(uint8_t num,float *pid_set_x,float *pid_set_y);
extern int input_final[3];
extern uint8 speed_mode;
loop_t loop;
car_serch_action car_move_state;
void Duty_2ms()
{

}

void Duty_5ms()
{
}

void Duty_10ms()
{
}

void Duty_20ms()
{

//    key_IRQHandler();//按键检测
//    LCDMenu_Refresh();//OLED刷新
}

void Duty_50ms()
{
 
}

void Duty_1000ms()
{ 
//  powerVal=GetPowerAdc();
}
void Loop_check()  //TIME INTTERRUPT
{
	//loop.time++; //uint16_t
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;
	loop.cnt_1000ms++;

}

void Duty_Loop()   					//最短任务周期为1ms，总的代码执行时间需要小于1ms。
{
	if(1)
	{		
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//周期2ms的任务 
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//周期5ms的任务 
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//周期10ms的任务
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//周期20ms的任务
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//周期50ms的任务
		}
		if( loop.cnt_1000ms >= 1000 )
		{
			loop.cnt_1000ms = 0;
			Duty_1000ms();					//周期50ms的任务
		}
	}
}





//void Read_Yaw(void)
//{
//    MPU6050_Data_Prepare();
//    if((mpu6050.Gyro.z>=0.5 || mpu6050.Gyro.z<-0.5 )&& mpu6050.Gyro_CALIBRATE == 1)
//        Yaww += (mpu6050.Gyro.z*0.05);
//}


extern uint8_t first_flag ;
extern xy_t S_s[23];
extern uint8_t test_form ,test_to ;
void test_select()
{
    uint8_t i = 0;
    uint8_t sta_i = 0;
    int temp[30];
    
    for(i=0;i<8;i++)
         input_q[i] = 100;
    if(test_to == 8)
    {
       temp[0] = 1;temp[1] = 57;temp[2] = 1;temp[3] = 2;temp[4] = 58;temp[5] = 2;
       temp[6] = 3;temp[7] = 59;temp[8] = 3;temp[9] = 4;temp[10] = 60;temp[11] = 4;
       temp[12] = 5;temp[13] = 61;temp[14] = 5;temp[15] = 6;temp[16] = 62;temp[17] = 6;
       temp[18] = 7;temp[19] = 63;temp[20] = 7;temp[21] = 8;temp[22] = 64;temp[23] = 8;

           for(i=((test_form-1)*3);i<24;i++)
                ConversionToXYone(temp[i],&S_s[i-((test_form-1)*3)]);
           
           optimal_n = -4;//8-12
           Walk_Ctrl.start_point = test_form;

    }
    if(test_to == 57)
    {
       temp[0] = 1;temp[1] = 8;temp[2] = 1;temp[3] = 9;temp[4] = 16;temp[5] = 9;
       temp[6] = 17;temp[7] = 24;temp[8] = 17;temp[9] = 25;temp[10] = 32;temp[11] = 25;
       temp[12] = 33;temp[13] = 40;temp[14] = 33;temp[15] = 41;temp[16] = 48;temp[17] = 41;
       temp[18] = 49;temp[19] = 56;temp[20] = 49;temp[21] = 57;temp[22] = 64;temp[23] = 57;
       
       sta_i = (test_form/8)*3;
       for(i=sta_i;i<24;i++)
            ConversionToXYone(temp[i],&S_s[i-sta_i]);
           
       optimal_n = -4;//8-12
       Walk_Ctrl.start_point = test_form;
    }
}
extern int WangToNum;
void Select_MoveDir(void)
{
    static uint16_t delay_cnt=0;
    if(WangToNum<=8)
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = 0;
        SysPID[Y_Dir].Out_Int16 = (int16_t)100;
        En_Update_PID = Yes;
         
    }
    else if(WangToNum>=57)
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = (int16_t)0;
        SysPID[Y_Dir].Out_Int16 = -(int16_t)100;
        En_Update_PID = Yes;
      //  systick_delay_ms(900); 
    }
    else if(WangToNum==16||WangToNum==24||WangToNum==32||
            WangToNum==40||WangToNum==48||(WangToNum==56&& SwitchMode>1) )
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = -(int16_t)100 * 1;
        SysPID[Y_Dir].Out_Int16 = (int16_t)0;
        En_Update_PID = Yes;
      //  systick_delay_ms(900); 
    }
    else if(WangToNum==9||WangToNum==17||WangToNum==25||
            WangToNum==33||WangToNum==41|| (WangToNum==49&& SwitchMode>1) )
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = (int16_t)100 * 1;
        SysPID[Y_Dir].Out_Int16 = (int16_t)0;
        En_Update_PID = Yes;
     //   systick_delay_ms(900); 
    }
    else if(SwitchMode<=1 && (WangToNum==49||WangToNum==50||WangToNum==51||
             WangToNum==52||WangToNum==53||WangToNum==54||WangToNum==55||WangToNum==56))
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = (int16_t)0;
        SysPID[Y_Dir].Out_Int16 = -(int16_t)100;
        En_Update_PID = Yes;
    }
    else if(SwitchMode<=1 && (WangToNum==15||WangToNum==23||WangToNum==31||
             WangToNum==39||WangToNum==47) )
    {
        SysPID[X_Dir].EnPid_Out = No;
        SysPID[Y_Dir].EnPid_Out = No;
        
        SysPID[X_Dir].Out_Int16 = -(int16_t)100 * 1;
        SysPID[Y_Dir].Out_Int16 = (int16_t)0;
        En_Update_PID = Yes;
    }

    while(1)
    {
        delay_cnt++;
        if(delay_cnt>=120)
            break;
        if(WangToNum<=8)
        {
            if((Walk_Ctrl.H_B_Flag==1&&Walk_Ctrl.FoundNumber==(WangToNum+8))&&delay_cnt>=60)
            {
//                Walk_Ctrl.pos = NumPos_To_CarPos(1,(WangToNum+8),Walk_Ctrl.num_pos);     //更新old_xy  
                break;
            }
            else
                systick_delay_ms(10);              
        }
        else if(WangToNum>=57)
        {
            if((Walk_Ctrl.H_B_Flag==0&&Walk_Ctrl.FoundNumber==(WangToNum)) &&delay_cnt<=5)
            {
//                Walk_Ctrl.pos = NumPos_To_CarPos(0,(WangToNum),Walk_Ctrl.num_pos);     //更新old_xy  
                break;
            }
            else
                systick_delay_ms(10);
        }
        else if( (WangToNum>=49&&WangToNum<=55) && SwitchMode <=1)
        {
            if((Walk_Ctrl.H_B_Flag==0&&Walk_Ctrl.FoundNumber==(WangToNum)) &&delay_cnt<=5 )
            {
//                Walk_Ctrl.pos = NumPos_To_CarPos(0,(WangToNum),Walk_Ctrl.num_pos);     //更新old_xy  
                break;
            }
            else
                systick_delay_ms(10);
        }
        else
        {
            if((Walk_Ctrl.H_B_Flag==0&&Walk_Ctrl.FoundNumber==(WangToNum-8)) || (Walk_Ctrl.H_B_Flag==1&&Walk_Ctrl.FoundNumber==(WangToNum+8)))
            {
                if(Walk_Ctrl.CameraId == 0)
                {
//                    Walk_Ctrl.pos = NumPos_To_CarPos(0,(WangToNum-8),Walk_Ctrl.num_pos);     //更新old_xy  
                }
                else
                {
//                    Walk_Ctrl.pos = NumPos_To_CarPos(1,(WangToNum+8),Walk_Ctrl.num_pos);  
                }
                break;
            }
            else
                systick_delay_ms(10); 
        }
    }
     
    first_flag = 1;         //打开错误保护  
    
    SysPID[X_Dir].EnPid_Out = No;
    SysPID[Y_Dir].EnPid_Out = No;
    
}
uint8_t min_n;
extern uint8_t stop_flag;
extern xy_t start_car[28];
extern xy_t start_car_set;

void Cau_EnterPos()
{
    uint8_t j;
    uint16_t min_d = 10000;
    xy_t start_pos;

    if(SwitchMode==15)
    {
        for(j=0;j<28;j++)                  //计算最佳入场位置
        {
            Find_path(input_q,start_car[j]);  //start_car 车辆可以进场的28个位置  input_q 棋子的八个位置
            if(S_Dis<min_d)
            {
                min_d = S_Dis;
                min_n = j;
            }
        }
//        if(speed_mode == 3)                 //模式3为认为设定入场位置
//        {
//            start_car[min_n].x = start_car_set.x;
//            start_car[min_n].y = start_car_set.y;
//            S_Dis = 10000;
//        }
        Find_path(input_q,start_car[min_n]);
    
    }
//    while(En_Update_PID == No)          //开启则车启动
//    {
////        Duty_Loop();
//          MOTOR_get();
//          Communication_send(2);
////        Switch_Mode();              //选择模式
////        sendDataToScope();
//        ALLEncoder_clear();//清零正交解码值
//    }
    En_Update_PID = No;
    
    
    if(speed_mode != 4)                 //算出起始位置数字
      TransferToNum(start_car[min_n],&WangToNum);  
    now_position.x=9;
    now_position.y=5;
    SendBeforeOrAfter();
  //  first_flag = 1;         //打开错误保护    
}



void Enter_FirstPos(void)
{

    Cau_EnterPos();     //计算入场坐标，路线

//    Select_MoveDir();   //车入场

}


extern uint8_t CarEnterOk;
void car_serch (void)
{
  En_Update_PID = No;
  car_move_state=Car_serching;
  now_position.x=-1;
  now_position.y=2;
  TransferToNum(S_s[cal_n_serch++],&WangToNum);
  CarEnterOk = 1;
  En_Update_PID=Yes;
  while(1)
  {}
}
void Car_EnterTo(void)
{
//    if(SwitchMode==15)
//    {
//        while(En_Update_PID == No)   //输入参数
//        {
//            Duty_Loop(); 
//            MotorStatus_t.LH_Spd = GetDmaConut(LH);
//            MotorStatus_t.RH_Spd = GetDmaConut(RH);
//            MotorStatus_t.LB_Spd = GetDmaConut(LB);
//            MotorStatus_t.RB_Spd = GetDmaConut(RB);
//            Switch_Mode();              //选择模式
//            sendDataToScope();
//            AllDmaValueClear();//清零DMA值
//        }
//    }
    
    En_Update_PID = No;
    
    Enter_FirstPos();          //进入第一个坐标
    En_Update_PID = No;
    
    TransferToXY_One(WangToNum,&now_position);//先更新现在的位置
    TransferToNum(S_s[++cal_n],&WangToNum);
    
    if(SwitchMode == 1)
        ServoState[userNow] = Servo_Up;
    //ServoState[1] = Servo_Up;

    CarEnterOk = 1; 
    En_Update_PID = Yes;       //
}