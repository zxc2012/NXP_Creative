#include "headfile.h"

uint8       userNow = 0;      //当前使用的臂
_SevoCtrParam SevoCtlParam[2];
_SevoCtrState ServoState[2] = {OK,OK};  //ServoState[0] 捡棋子舵机状态  ServoState[1] 放障碍舵机状态
uint8 ChangeServoValue[2]={10,20,};

//uint8 startCtrServo=0;
/**初始化舵机PWM**/
void InitServo(void)
{
    SevoCtlParam[0].Back = 725;   
    SevoCtlParam[0].Mid = 690;
    SevoCtlParam[0].Fetch = 665;
    
    SevoCtlParam[1].Back = 725;   
    SevoCtlParam[1].Mid = 690;
    SevoCtlParam[1].Fetch = 665;
    
//    SevoCtlParam[2].Back = 21;     
//    SevoCtlParam[2].Mid = 40;
//    SevoCtlParam[2].Fetch = 60;
//     
//    SevoCtlParam[3].Back = 67;    
//    SevoCtlParam[3].Mid = 47;
//    SevoCtlParam[3].Fetch = 26;

    ftm_pwm_init(ftm3,ftm_ch0,300,SevoCtlParam[0].Back);
    ftm_pwm_init(ftm3,ftm_ch1,300,SevoCtlParam[1].Back);
//    ftm_pwm_init(ftm3,ftm_ch2,300,SevoCtlParam[2].Back);
//    ftm_pwm_init(ftm3,ftm_ch3,300,SevoCtlParam[3].Back);
    
    SevoCtlParam[0].ServoSetPwm=SevoCtlParam[0].Back;
    SevoCtlParam[1].ServoSetPwm=SevoCtlParam[1].Back;
//    SevoCtlParam[2].ServoSetPwm=SevoCtlParam[2].Back;
//    SevoCtlParam[3].ServoSetPwm=SevoCtlParam[3].Back;
    
    SevoCtlParam[0].currentServoPwm=SevoCtlParam[0].Back;
    SevoCtlParam[1].currentServoPwm=SevoCtlParam[1].Back;
//    SevoCtlParam[2].currentServoPwm=SevoCtlParam[2].Back;
//    SevoCtlParam[3].currentServoPwm=SevoCtlParam[3].Back;
    
    SevoCtlParam[0].isServoMoveOk=1;
    SevoCtlParam[1].isServoMoveOk=1;
//    SevoCtlParam[2].isServoMoveOk=1;
//    SevoCtlParam[3].isServoMoveOk=1;

}

/**设置舵机占空比**/
void SetServoDuty(uint8 sevoID,int16 duty)
{  
  if(duty>MAX_ANGLE)//限幅
	duty=MAX_ANGLE;
  else if(duty<MIN_ANGLE)
	duty=MIN_ANGLE;
 
  if(duty!=SevoCtlParam[sevoID].ServoSetPwm)
  { 
      SevoCtlParam[sevoID].ServoSetPwm = duty;
      SevoCtlParam[sevoID].isServoMoveOk=0;
  }
}   


void TimeChangeServoSetPwm()
{
    uint8 i=0,j=0;
     
    for(j=0;j<2;j++)
    {
        //j = i+(userNow<<1);
        if(SevoCtlParam[j].ServoSetPwm==SevoCtlParam[j].currentServoPwm)
        {
            SevoCtlParam[j].isServoMoveOk = 1;
           // return;
        }
        if(SevoCtlParam[j].ServoSetPwm>SevoCtlParam[j].currentServoPwm)
            SevoCtlParam[j].currentServoPwm += ChangeServoValue[j];
        else if(SevoCtlParam[j].ServoSetPwm<SevoCtlParam[j].currentServoPwm)
            SevoCtlParam[j].currentServoPwm -= ChangeServoValue[j];
        
        if(ABS(SevoCtlParam[j].ServoSetPwm-SevoCtlParam[j].currentServoPwm)<=ChangeServoValue[j]) //误差小则强行赋值
            SevoCtlParam[j].currentServoPwm = SevoCtlParam[j].ServoSetPwm;
        
        if(SevoCtlParam[j].currentServoPwm>MAX_ANGLE)//限幅
            SevoCtlParam[j].currentServoPwm=MAX_ANGLE;
        else if(SevoCtlParam[j].currentServoPwm<MIN_ANGLE)
            SevoCtlParam[j].currentServoPwm=MIN_ANGLE;              
      
        ftm_pwm_duty(ftm3, (ftm_ch0+j), SevoCtlParam[(ftm_ch0+j)].currentServoPwm); 
    }
//    for(i=0;i<2;i++)
//    {
//        j = i+(userNow<<1);
//        if(SevoCtlParam[j].ServoSetPwm==SevoCtlParam[j].currentServoPwm)
//        {
//            SevoCtlParam[j].isServoMoveOk = 1;
//           // return;
//        }
//        if(SevoCtlParam[j].ServoSetPwm>SevoCtlParam[j].currentServoPwm)
//            SevoCtlParam[j].currentServoPwm += ChangeServoValue[j];
//        else if(SevoCtlParam[j].ServoSetPwm<SevoCtlParam[j].currentServoPwm)
//            SevoCtlParam[j].currentServoPwm -= ChangeServoValue[j];
//        
//        if(ABS(SevoCtlParam[j].ServoSetPwm-SevoCtlParam[j].currentServoPwm)<=ChangeServoValue[j]) //误差小则强行赋值
//            SevoCtlParam[j].currentServoPwm = SevoCtlParam[j].ServoSetPwm;
//        
//        if(SevoCtlParam[j].currentServoPwm>MAX_ANGLE)//限幅
//            SevoCtlParam[j].currentServoPwm=MAX_ANGLE;
//        else if(SevoCtlParam[j].currentServoPwm<MIN_ANGLE)
//            SevoCtlParam[j].currentServoPwm=MIN_ANGLE;              
//      
//        ftm_pwm_duty(FTM3, (ftm_ch0+j), SevoCtlParam[(ftm_ch0+j)].currentServoPwm); 
//    }
}

extern Spd_Action_E SpdAct; //默认为加速状态

void Servo_StateCtr()
{
    static uint16 up_cnt=0;
        switch(ServoState[0])
        {
            case Servo_Down:
                {
                    SetServoDuty(0,SevoCtlParam[0].Fetch);
                    if(SevoCtlParam[0].isServoMoveOk==1)
                    {
                        CtrRelay(RelayIdDown,Relay_Yes);
                        up_cnt++;
                        if(up_cnt>=15)
                        {
                            ServoState[0] = Servo_Up;
                            up_cnt = 0;
                        }
//                        if((SpdAct == Car_stop))    //退场
//                            ServoState[i] = OK;                     
                    }
                    break;
                }
            case Servo_Up:
                {
                    SetServoDuty(0,SevoCtlParam[0].Back);
                    if(SevoCtlParam[0].isServoMoveOk==1)
                    {
//                        CtrRelay(RelayIdFrong,Relay_No);
//                        CtrRelay(RelayIdLeft,Relay_No);
                        ServoState[0] = OK;
                    }
                    break;
                }
            case EleIron_OFF:
                {
                    CtrRelay(RelayIdDown,Relay_No);
                    ServoState[0] = OK;
                    break;
                }
           case EleIron_ON:
               {
                    CtrRelay(RelayIdDown,Relay_Yes);
                    ServoState[0] = OK;
                    break;
               }
           case OK:break;
        }
        
        
        switch(ServoState[1])
        {
            case Servo_Down:
                {
                    SetServoDuty(1,SevoCtlParam[1].Back);
                    if(SevoCtlParam[1].isServoMoveOk==1)
                    {
                        CtrRelay(RelayIdFrong,Relay_Yes);
                        //if(i == 0)
                        //    ServoState[i] = Servo_Mid;
                        //else 
                        up_cnt++;
                        if(up_cnt>=15)
                        {
                            ServoState[1] = Servo_Down;
                            up_cnt = 0;
                        }
//                        if((SpdAct == Car_stop))    //退场
//                            ServoState[i] = OK;                     
                    }
                    break;
                }
            case Servo_Mid:
                {
                    SetServoDuty(1,SevoCtlParam[1].Mid);
                    if(SevoCtlParam[1].isServoMoveOk==1)
                    {
                        ServoState[1] = OK;
                    }
                    
                    break;
                }
            case Servo_Up:
                {
                    SetServoDuty(1,SevoCtlParam[1].Fetch);
                    if(SevoCtlParam[1].isServoMoveOk==1)
                    {
//                        CtrRelay(RelayIdFrong,Relay_No);
//                        CtrRelay(RelayIdLeft,Relay_No);
                        ServoState[1] = OK;
                    }
                    break;
                }
            case EleIron_OFF:
                {
                    CtrRelay(RelayIdFrong,Relay_No);
                    ServoState[1] = OK;
                    break;
                }
           case EleIron_ON:
               {
                    CtrRelay(RelayIdFrong,Relay_Yes);
                    ServoState[1] = OK;
                    break;
               }
           case OK:break;
        }
    
 
}

