#include "headfile.h"




void Speed_ResolveXY(uint16 speed);

uint8 queen_set_flag = 8;;
float DiagonalDis;
float x_error;
float y_error;
float WALK_SPEED = 20;
float WALK_SPEED_SET = 50;

varBufferClass varBufferClass_t={0};    //变量缓冲
uint8 cal_n = 0;                      //判断走的步数
uint8 updown_step = 1;                //捡棋子步骤
uint8 updown_flag = 0;                //捡棋子标志
int WangToNum;                      //车移动去的位置
Spd_Action_E SpdAct=Car_straight;       //默认为加速状态
uint8 CarEnterOk = 0;

uint8 SetOkError = 2;
uint8 send_flag = 1;
uint8 ucn=0;
extern xy_t S_s[23];
extern char str[5];
extern uint8 speed_mode;

void Track_EightQueen()
{

    switch(SpdAct)
    {
        
        case Car_setpoint://定点阶段
        {
           SysPID[position].EnPid_Out = Yes;    //开位置环pid  
          /***状态切换****/
          if(SysPID[position].err_abs<=50)   //是否定点完毕
          {
                if(car_move_state==Car_serching)
                {
                  if(cal_n_serch==serch_step)
                  {
                      SpdAct = Car_stop;  
                      En_Update_PID = Yes;
                      break;               //切换状态，跳出当前case
                  }
                  TransferToXY_One(WangToNum,&now_position);//先更新现在的位置
                  TransferToNum(S_s[cal_n_serch++],&WangToNum);   //更新将要去的位置
                }
                else if(car_move_state==Car_chessing)
                {
                   if(cal_n == (8-optimal_n)*2)//是否到达终点
                   {
                      SpdAct = Car_stop;  
                      En_Update_PID = Yes;
                      break;               //切换状态，跳出当前case
                   }
                   TransferToXY_One(WangToNum,&now_position);//先更新现在的位置
//                   TransferToNum(S_s[++cal_n],&WangToNum);   //更新将要去的位置
                }
                
                SysPID[LH_Spe].set = 0;
                SysPID[LB_Spe].set = 0;
                SysPID[RH_Spe].set = 0;
                SysPID[RB_Spe].set = 0;
                SysPID[position].EnPid_Out = No;  //关位置环pid
              
                Pos_clear();       //清空编码器累计值
                SpdAct=Car_move_q;      //拾取或放下棋子
          }
        }break;
        
        case Car_move_q:          //拿起放下棋子
        {
          if(car_move_state==Car_serching)
          {
            ServoState[0]=Servo_Up;
            En_Update_PID = Yes;//开启PID，移动去下个位置
                
            Pos_clear();       //清空编码器累计值
                
            SpdAct = Car_straight; 
          }
          else if(car_move_state==Car_chessing)
          {
//            Judge_Queen_UpDown();  //暂时关掉
            updown_step =2;          //暂时添加
            if(updown_step == 2)   //是否完成
            {   
                ucn++;
                if(ucn>=100)
                {
                  ucn=0;
                updown_step = 1;
                En_Update_PID = Yes;//开启PID，移动去下个位置
                send_flag = 1;
                
                Pos_clear();       //清空编码器累计值
                
                SpdAct = Car_straight;  
                }
            }
          }
                    
        }break; 
        
        case Car_straight://直走阶段
        {
            
            SysPID[position].EnPid_Out = No;  //关位置环pid
            
            SendBeforeOrAfter();//发送树莓派使用前或后摄像头
            
            Speed_ResolveXY(WALK_SPEED);
            
            /***状态切换****/
            if(IsCarFindPoint()==Yes)//找到定点  
            {   
                SpdAct=Car_setpoint;
                WALK_SPEED = MIN_SPEED;
                WALK_SPEED_SET = WALK_SPEED;
                
            }
                          
        }break;  
        
        case Car_stop://完成
        {
            CtrRelay(RelayIdDown,Relay_No);
//            CtrRelay(RelayIdFrong,Relay_No);
//            CtrRelay(RelayIdLeft,Relay_No);

//            if(speed_mode!=4)
//                Out_Of_Board();         
        }break; 

    }
}

void Track_PlaceObstacle()    //放置障碍
{
    static uint8 cb_step = 0;
    static uint8 cb_cnt = 0;
    
    switch(SpdAct)
    {
        case Car_setpoint://定点阶段
        {
           SysPID[X_Dir].EnPid_Out = Yes;
           SysPID[Y_Dir].EnPid_Out = Yes;                    
          /***状态切换****/
          if(IsCarSetOK_Obstacle()==Yes)   //是否定点完毕
          {     
                StopAll();               //停下，
                En_Update_PID = No;      //关PID       
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
                TransferToNum(S_s[++cal_n],&WangToNum);   //更新将要去的位置
                SpdAct=Car_place_obstacle;        //拾取或放下棋子
          }
        }break;
        
        case Car_place_obstacle://放下挡板
        {	
            if(cb_step == 0 && ServoState[userNow] == OK)
            {
                cb_step = 1;
            }          
            else if(cb_step == 1 && ServoState[userNow] == OK)
            {
                cb_cnt++;
                if(cb_cnt>=50)
                {
                    ServoState[userNow] = EleIron_OFF;
                    if(cb_cnt>=70)
                    {
                        cb_cnt = 0;
                        cb_step = 2;    
                    }
                }
            }
            else if(cb_step == 2)  //userNow：0 前面的臂
            {                                                                    //         1 左边的臂
                ServoState[userNow] = Servo_Up; 

                if(cal_n == (8-optimal_n)*2+1)//是否到达终点
                {
                   ServoState[0] = Servo_Mid; 
                   ServoState[1] = Servo_Mid; 
                   SpdAct = Car_stop;   
                   En_Update_PID = Yes;
                   TransferToNum(S_s[--cal_n],&WangToNum);
                   break;               //切换状态，跳出当前case
                }
                En_Update_PID = Yes;
                send_flag = 1;
                SpdAct = Car_straight;   
                cb_step = 0;  
            } 
            else if(cb_step == 3 && (ServoState[userNow] == OK || ServoState[userNow] == Servo_Mid) && userNow == 0)
            {
                if(cal_n == (8-optimal_n)*2+1)//是否到达终点
                {
                   SpdAct = Car_stop;  
                   En_Update_PID = Yes;
                   TransferToNum(S_s[--cal_n],&WangToNum);
                   break;               //切换状态，跳出当前case
                }
                send_flag = 1;
                SpdAct = Car_straight;  
                En_Update_PID = Yes;
                cb_step = 0;
            }
                    
        }break; 
        
        case Car_straight://直走阶段
        {
            
            SendBeforeOrAfter();//发送树莓派使用前或后摄像头
           
//            Speed_ResolveXY(WALK_SPEED);
            
            /***状态切换****/
            if(IsCarFindPoint()==Yes)//找到定点
            {   
                SpdAct=Car_setpoint;
                WALK_SPEED_SET = MIN_SPEED;
            }
                          
        }break;   
        
        case Car_stop://完成
        {
            CtrRelay(RelayIdFrong,Relay_No);
            CtrRelay(RelayIdLeft,Relay_No);
            
            if(speed_mode!=4)
                Out_Of_Board();         
        }break; 

    }
}

void Speed_ResolveXY(uint16 speed)
{
    
    WALK_SPEED_SET = GetSpeedByDistance(SysPID[position].err);         
    
    X_Speed = (int16)((x_error * speed) / DiagonalDis);
    Y_Speed = (int16)((y_error * speed) / DiagonalDis);
}


void SendBeforeOrAfter(void)
{
 //   if(send_flag == 1)
 //   {
        if(WangToNum>=24 )
        {
            Walk_Ctrl.H_B_Flag = 0;  //前摄
  //          send_flag = 0;
        }
        else if(WangToNum< 24)  
        {
            Walk_Ctrl.H_B_Flag = 1;  //后摄
  //          send_flag = 0;
        }
 //   }
}    
 


uint16 max_speed_y,min_speed_y,max_speed_x,min_speed_x;


void Judge_Queen_UpDown(void)
{
    static uint8 down_cnt = 0;
    
    if(updown_flag == 0)     //捡棋子
    {
        
        if(updown_step == 1&&ServoState[0]!=Servo_Up&&ServoState[0] != OK)
        {
            ServoState[0]=Servo_Down;
        }
        
        if(ServoState[0] == OK)   //是否完成
        {                 
            updown_step = 2;
            updown_flag = 1;
        }
        
    }
    else                 //放棋子
    { 
        down_cnt++;
        ServoState[0]=EleIron_OFF;
        if(down_cnt>=1)
        {
            down_cnt=0;
            updown_step = 2;
            updown_flag = 0;
        }
        
    }
    
}



/*
*功能：是否找到转折点
*返回：YES：，NO，
*/
YesNo_t IsCarFindPoint()
{      
    static uint8 flag = 0;;
//    if((SysPID[X_Dir].err_abs<StartPid) && (SysPID[Y_Dir].err_abs <StartPid) && SysPID[Head_Angle].err_abs<10) //判断误差
    if((ABS(POS_set_value-POS_actual_value))/100< StartPid&& SysPID[Head_Angle].err_abs<10) //判断误差
    {	
        flag++;
        if(flag>3)
        {
            flag = 0;
            return Yes;
        }
        else
            return No;
    }    
    else 
    {
        flag = 0;
        return No;
    }
}

/*
*功能：是否定点完毕
*返回：YES：是，NO，不
*/
YesNo_t IsCarSetOK()
{
    static uint8 flag = 0;
    if(SysPID[X_Dir].err_abs<SetOkError &&SysPID[Y_Dir].err_abs <SetOkError && SysPID[Head_Angle].err_abs<SetOkError) //判断误差
    {	
        flag++;
        if(flag>=queen_set_flag)
        {
            flag = 0;
            return Yes;
        }
        else
            return No;
    }    
    else 
    {
        flag = 0;
        return No;
    }
}

YesNo_t IsCarSetOK_Obstacle()
{
    static uint8 flag = 0;;
    if(( (SysPID[X_Dir].err_abs<10 &&SysPID[Y_Dir].err_abs <2&&userNow==0) || 
          (SysPID[X_Dir].err_abs<2 &&SysPID[Y_Dir].err_abs <10&&userNow==1) ) && SysPID[Head_Angle].err_abs<2) //判断误差
    {	
        flag++;
        if(flag>8)
        {
            flag = 0;
            return Yes;
        }
        else
            return No;
    }    
    else 
    {
        flag = 0;
        return No;
    }
}



/**
 * @brief  变量缓冲函数，可从当前值逐步变化到目标值 每个周期变化一次 此函数需周期执行
 *           
 * \code
 *   varBufferClass SpeedBuffer= {.isOKFlag=0,.times=50};//初始化结构体
 *   SpeedBuffer.isOKFlag = 0;//标志位置0 开始步进  相当于函数模块复位 很重要
 *   variableBuffer(nowSpeed,setSpeed,&Speed,&SpeedBuffer);
 * \endcode
 * @param[in]  inValue     初始值
 *             setValue    目标值
 *             *variable   变量指针
 *             *varBuffer  结构体指针 
 * 
 * @retval     1   正在步进中
 *             2   步进完成  等待
 */

uint8 variableBuffer(float inValue,float setValue,float *variable,varBufferClass *varBuffer)
{
	if(varBuffer->isOKFlag==0)
	{
		varBuffer->delt = 0;
		varBuffer->count = 0;
		varBuffer->delt = (setValue - inValue)/varBuffer->times;	
		varBuffer->isOKFlag = 1;
	}
	if(varBuffer->isOKFlag==1)
	{
		*variable += varBuffer->delt;
		varBuffer->count++;
		if(varBuffer->count >= varBuffer->times) 
		{
				varBuffer->isOKFlag = 2;
		}
	}
	return varBuffer->isOKFlag;
}

void Out_Of_Board(void)
{
    uint16 num;
    xy_t end_q;
    static uint16 delay_cnt = 0;
    
    if(delay_cnt>300)
    {
        StopAll();
        En_Update_PID = No;
    }
    else
    {
        delay_cnt++;

    }
    
    end_q.y = (WangToNum-1)/8;
    end_q.x = (WangToNum-1)%8;

    num = WangToNum;


    if(end_q.x>=4)                         //若走X一定为右
    {
        if(end_q.y>=4)
        {
            if((7-end_q.y)<=(7-end_q.x))  //走Y 后
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)0;
                SysPID[Y_Dir].Out_Int16 = (int16)300;
                return;
            }
            else                         //走X 右
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)300;
                SysPID[Y_Dir].Out_Int16 = (int16)0;
                return;
            }
        }
        else
        {
            if((end_q.y-0)<=(7-end_q.x))  //走Y 前
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)0;
                SysPID[Y_Dir].Out_Int16 = (int16)-300;
                return;
            }
            else                         //走X 右
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)300;
                SysPID[Y_Dir].Out_Int16 = (int16)0;
                return;
            }
        }
    }
    else                                                //若走X一定为左
    {
        if(end_q.y>=4)
        {
            if((7-end_q.y)<=(end_q.x))  //走Y 后
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)0;
                SysPID[Y_Dir].Out_Int16 = (int16)300;
                return;
            }
            else                         //走X 左
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)-300;
                SysPID[Y_Dir].Out_Int16 = (int16)0;
                return;
            }
        }
        else
        {
            if((end_q.y-0)<=(end_q.x))  //走Y 前
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)0;
                SysPID[Y_Dir].Out_Int16 = (int16)-300;
                return;
            }
            else                         //走X 左
            {
                SysPID[X_Dir].EnPid_Out = No;
                SysPID[Y_Dir].EnPid_Out = No;
            
                SysPID[X_Dir].Out_Int16 = (int16)-300;
                SysPID[Y_Dir].Out_Int16 = (int16)0;
                return;
            }
        }    
    }

    
}