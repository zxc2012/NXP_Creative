#include "headfile.h"

Dof_PID_t SysPID[Dof_Num] = {0};//PID数组初始化
static void Dof_PID_Init_Similar_Parameter(Dof_PID_t* pDof_PID, float Out_Min, float Out_Max);
YesNo_t En_Update_PID = No;
int16 Y_Speed,X_Speed;
float Angle_to_Ctro=0;

int Speed_Value_LH = 0;      //左前电机PWM
int Speed_Value_RH = 0;      //右前电机PWM
int Speed_Value_RB = 0;      //右后电机PWM
int Speed_Value_LB = 0;      //左后电机PWM
/*
*PID采样并对采样数据进行滤波
*Sample_NewRaw：本次采样值
*Sample_Valid：本次采样有效性，默认Yes
*/
void PID_Update_Sample(Dof_PID_t* pPID, float Sample_NewRaw, YesNo_t Sample_Valid)
{    
    uint16 i;
   
    //if(pPID->sample - pPID->sample_last > )
    
    for(i=SAMPLE_CACHE_MAX-1; i>0; i--)//采样数组更新
        pPID->sample_cache[i] = pPID->sample_cache[i-1];
    
    pPID->sample_cache[0] = Sample_NewRaw;//更新最新的采样
    
    if(Yes == Sample_Valid)
    {
        pPID->sample_cache_cnt++;
        if(pPID->sample_cache_cnt > pPID->sample_cache_max)
            pPID->sample_cache_cnt = pPID->sample_cache_max;
       
        pPID->sample = DataFilter(pPID->sample_cache, pPID->sample_cache_cnt);//数据滤波
    }    
    else
    {
        pPID->sample_cache_cnt = 0;
    }
}

/*
*计算误差
*/
void  PID_Update_Error (Dof_PID_t* pPID, YesNo_t Sample_Valid)
{
  pPID->err_last = pPID->err;//保存上次误差
    if(Yes == Sample_Valid)
    {
        pPID->err = pPID->set - pPID->sample;//计算误差
        
        if(pPID->err > 0)//误差绝对值
            pPID->err_abs = pPID->err;
        else
            pPID->err_abs = -pPID->err;
        
        pPID->err_p = pPID->err;//比例P
        
        if(Abs(pPID->err) < pPID->i_act_err_abs_max)//计算积分I
        {
            pPID->err_i += pPID->err;//积分累加
            Constraint_By_Abs(&pPID->err_i, pPID->err_i_abs_max);//积分限幅
        }
        else
            pPID->err_i = 0;//积分清0
        

//        pPID->err_d =  pPID->err_last_d*0.7 + pPID->err_d*0.3;//低通滤波
        pPID->err_d = pPID->err - pPID->err_last;//计算误差
//		if(ABS(pPID->err_d)>30)        //微分D最大值限制
//			pPID->err_d=0;
    }
    else
    {
        pPID->err = 0;
        pPID->err_p = 0;
        pPID->err_i = 0;
        pPID->err_d = 0;
    }
}

/*
*  pid 输出限幅
*/
void Dof_PID_Update_Calculate(Dof_PID_t* pPID,YesNo_t En)
{ 
    if(En == Yes)
    {
        pPID->Out_Float = PID_Update_Out(pPID);

        if(pPID->Out_Float < pPID->Out_Min)//输出限幅最小值
            pPID->Out_Float = pPID->Out_Min;
        else if(pPID->Out_Float > pPID->Out_Max)//输出限幅最大值
            pPID->Out_Float = pPID->Out_Max;
        
          pPID->Out_Int16 = (int16)pPID->Out_Float;//输出化为整型
    
//        return pPID->Out_Int16;
    }
}

/*
*  pid 输出
*/
float PID_Update_Out(Dof_PID_t* pPID)
{
    pPID->out_p = pPID->p * pPID->err_p;//输出P
    pPID->out_i = pPID->i * pPID->err_i;//输出I
    pPID->out_d = pPID->d * pPID->err_d;//输出D
    
    if(pPID->out_i < pPID->out_i_min)//输出积分最小值限幅
        pPID->out_i = pPID->out_i_min;
    else if(pPID->out_i > pPID->out_i_max)//输出积分最大值限幅
        pPID->out_i = pPID->out_i_max;
    if(pPID->out_d>=2000)
    {
        pPID->out_d = 0;
    }
    pPID->out = pPID->out_p + pPID->out_i + pPID->out_d;//输出累加
    return pPID->out; 
}

extern uint16 max_speed_y,min_speed_y,max_speed_x,min_speed_x;
/*
* 速度参数初始化
*/
void PID_Parameter_Init(void)
{
    Dof_PID_Init_Similar_Parameter(&SysPID[Head_Angle], -100, 100);
    Dof_PID_Init_Similar_Parameter(&SysPID[position], -30, 30);
    Dof_PID_Init_Similar_Parameter(&SysPID[LH_Spe], -9800, 9800);//最大输出，最小输出
    Dof_PID_Init_Similar_Parameter(&SysPID[RH_Spe], -9800, 9800);
    Dof_PID_Init_Similar_Parameter(&SysPID[LB_Spe], -9800, 9800);//最大输出，最小输出
    Dof_PID_Init_Similar_Parameter(&SysPID[RB_Spe], -9800, 9800);
    
    SysPID[X_Dir].EnPid_Out = No;   
    SysPID[X_Dir].p = 2.5; 
    SysPID[X_Dir].i = 0;
    SysPID[X_Dir].d = 0;
    SysPID[X_Dir].sample_cache_max = 3;
    SysPID[X_Dir].set = 0;
    SysPID[X_Dir].err_i_abs_max = 50;
    SysPID[X_Dir].i_act_err_abs_max = 10;
    SysPID[X_Dir].Out_Int16=0;
    
    SysPID[Y_Dir].EnPid_Out = No;    
    SysPID[Y_Dir].p = 2.5;
    SysPID[Y_Dir].i = 0;
    SysPID[Y_Dir].d = 0;
    SysPID[Y_Dir].sample_cache_max = 3;
    SysPID[Y_Dir].set = 0;
    SysPID[Y_Dir].err_i_abs_max = 50;
    SysPID[Y_Dir].i_act_err_abs_max = 10;
    SysPID[Y_Dir].Out_Int16=0;
    
    SysPID[position].EnPid_Out = No;    
    SysPID[position].p = 0.015;
    SysPID[position].i = 0;
    SysPID[position].d = 0.018;
    SysPID[position].sample_cache_max = 3;
    SysPID[position].set = 0;
    SysPID[position].err_i_abs_max = 50;
    SysPID[position].i_act_err_abs_max = 10;
    
    SysPID[Head_Angle].EnPid_Out = No;
    SysPID[Head_Angle].p = 1.4;
    SysPID[Head_Angle].i = 0;
    SysPID[Head_Angle].d = 1;
    SysPID[Head_Angle].sample_cache_max = 2;
    SysPID[Head_Angle].set = 0;
    SysPID[Head_Angle].err_i_abs_max = 50;
    SysPID[Head_Angle].i_act_err_abs_max = 20;
    
    SysPID[LH_Spe].EnPid_Out = Yes;   
    SysPID[LH_Spe].p =20; 
    SysPID[LH_Spe].i = 5.5;
    SysPID[LH_Spe].d = 0;
    SysPID[LH_Spe].sample_cache_max = 3;
    SysPID[LH_Spe].set = 0;
    SysPID[LH_Spe].EnPid_Out = Yes;
    SysPID[LH_Spe].err_i_abs_max = 10000;
    SysPID[LH_Spe].i_act_err_abs_max = 10000;
    
    SysPID[RH_Spe].EnPid_Out = Yes;   
    SysPID[RH_Spe].p = 20; 
    SysPID[RH_Spe].i = 5.5;
    SysPID[RH_Spe].d = 0;
    SysPID[RH_Spe].sample_cache_max = 3;
    SysPID[RH_Spe].set = 0;
    SysPID[RH_Spe].EnPid_Out = Yes;
    SysPID[RH_Spe].err_i_abs_max = 10000;
    SysPID[RH_Spe].i_act_err_abs_max = 10000;
    
    SysPID[LB_Spe].EnPid_Out = Yes;   
    SysPID[LB_Spe].p = 20; 
    SysPID[LB_Spe].i = 5.5;
    SysPID[LB_Spe].d = 0;
    SysPID[LB_Spe].sample_cache_max = 3;
    SysPID[LB_Spe].set = 0;
    SysPID[LB_Spe].EnPid_Out = Yes;
    SysPID[LB_Spe].err_i_abs_max = 10000;
    SysPID[LB_Spe].i_act_err_abs_max = 10000;
    
    SysPID[RB_Spe].EnPid_Out = Yes;   
    SysPID[RB_Spe].p = 20; 
    SysPID[RB_Spe].i = 5.5;
    SysPID[RB_Spe].d = 0;
    SysPID[RB_Spe].sample_cache_max = 3;
    SysPID[RB_Spe].set = 0;
    SysPID[RB_Spe].EnPid_Out = Yes;
    SysPID[RB_Spe].err_i_abs_max = 10000;
    SysPID[RB_Spe].i_act_err_abs_max = 10000;
   
//    Walk_Ctrl.head_angle = SysPID[Head_Angle].set;
   
    
}

void test_mode(void)
{
    
}

/*
* 速度输出限幅
*/
static void Dof_PID_Init_Similar_Parameter(Dof_PID_t* pPID, float Out_Min, float Out_Max)
{
    pPID->Out_Float = 0;
    pPID->Out_Int16 = 0;
    pPID->Out_Min = Out_Min;
    pPID->Out_Max = Out_Max;

    Fill_Mem((uint8*)pPID->sample_cache, sizeof(float)*SAMPLE_CACHE_MAX, 0);
    
    pPID->sample_cache_cnt = 0;
    pPID->err = 0;
    pPID->err_p = 0;
    pPID->err_i = 0;
    pPID->err_d = 0;
    pPID->err_last = 0;
    pPID->out = 0; 
    pPID->out_p = 0;
    pPID->out_i = 0;
    pPID->out_d = 0;
    pPID->out_i_min = pPID->Out_Min * 1;
    pPID->out_i_max = pPID->Out_Max * 1;
    
}

int16 x_temp,y_temp,h_temp;
extern int WangToNum;
uint8 temp1;
uint32_t temp2;
void Sys_Move_Ctrl_PID(void)
{
    static uint8 ANGLE_PID_EN = 0;
    float DD=0.0f;
    
    uint8 nn=0;
    uint8 mm=0;
    uint16 zy_sum=0;
    uint16 sx_sum=0;
    uint8 zy=64;
    uint8 sx=50;
    uint8 i=0;
    if(x_error>0)
      zy=80;
    else if(x_error<0)
      zy=50;
    for(i=0;i<3;i++)
    {
      if(heng_flag[zy+i]==1)
      {
        nn++;
        zy_sum+=heng_edge[zy+i];
      }
      if(midline_flag[sx+i]==1)
      {
        mm++;
        sx_sum+=midline[sx+i];
      }
    }
    SysPID[X_Dir].set=64;  
    SysPID[Y_Dir].set=50;
    if(mm!=0)
      SysPID[X_Dir].sample=sx_sum/mm;
    else
      SysPID[X_Dir].sample=SysPID[X_Dir].set;
    if(nn!=0)
      SysPID[Y_Dir].sample=zy_sum/nn;
    else 
      SysPID[Y_Dir].sample=SysPID[Y_Dir].set=50;
        
//        PID_Update_Sample(&SysPID[position], POS_actual_value, (YesNo_t)Yes);  //采样值滤波  
        
        PID_Update_Error(&SysPID[X_Dir], (YesNo_t)Yes);  //计算误差
        PID_Update_Error(&SysPID[Y_Dir], (YesNo_t)Yes); 
             
        Dof_PID_Update_Calculate(&SysPID[X_Dir],SysPID[X_Dir].EnPid_Out);  //pid 输出并限幅    
        Dof_PID_Update_Calculate(&SysPID[Y_Dir],SysPID[Y_Dir].EnPid_Out);   
               
}

void position_pid (xy_t now,int num)
{
    xy_t aim;
    
    TransferToXY_One(WangToNum,&aim);
    x_error=-(aim.x-now.x);
    y_error=(aim.y-now.y);
    DiagonalDis = sqrt(x_error*x_error + y_error*y_error);
    
    Pos_cal(now.x,now.y,aim.x,aim.y);
    Pos_get();
    if(y_error<0)//下
    {
      SysPID[position].set=-POS_set_value;
      SysPID[position].sample=-POS_actual_value;
    }
    else if(y_error==0&&x_error<0)//正左
    {
      SysPID[position].set=-POS_set_value;
      SysPID[position].sample=-POS_actual_value;
    }
    else 
    {
      SysPID[position].set=POS_set_value;  
      SysPID[position].sample=POS_actual_value;
    }
    
    PID_Update_Error(&SysPID[position], (YesNo_t)Yes);  //计算误差
    Dof_PID_Update_Calculate(&SysPID[position],SysPID[position].EnPid_Out);  //pid 输出并限幅
    if(SysPID[position].EnPid_Out==Yes)
    {
    X_Speed = (int16)((x_error * SysPID[position].Out_Int16) / DiagonalDis);
    Y_Speed = (int16)((y_error * SysPID[position].Out_Int16) / DiagonalDis);
    }
}



void SysCtrl_Speed_PID(int16 Speedx,int16 Speedy,int16 Ang_Speed,YesNo_t En_Turn)     //speed N.2
{
    int16 Wheel_Speed[4]={0},X_Wheel_Speed[4] = {0},Y_Wheel_Speed[4]= {0},A_Wheel_Speed[4] = {0};
    int8 i;
    
//    if(y_error!=0&&x_error==0)
//    {}
    
    Y_Wheel_Speed[LH] =  Speedy ;
    Y_Wheel_Speed[RH] =  Speedy ;   
    Y_Wheel_Speed[LB] =  Speedy ;   
    Y_Wheel_Speed[RB] =  Speedy ;       

    X_Wheel_Speed[LH] =  Speedx ;
    X_Wheel_Speed[RH] =  Speedx ;   
    X_Wheel_Speed[LB] =  Speedx ;   
    X_Wheel_Speed[RB] =  Speedx ;   
    
    if(En_Turn == Yes)
    {
        A_Wheel_Speed[LH] =  Ang_Speed ;
        A_Wheel_Speed[RH] = -Ang_Speed ;   
        A_Wheel_Speed[LB] =  Ang_Speed ;   
        A_Wheel_Speed[RB] = -Ang_Speed ;               
    }  
     
    for(i = 0;i < 4;i++)
       Wheel_Speed[i] = X_Wheel_Speed[i] + Y_Wheel_Speed[i] + A_Wheel_Speed[i];  
    
    SysPID[LH_Spe].set = Wheel_Speed[LH];  //目标速度
    SysPID[RH_Spe].set = Wheel_Speed[RH];
    SysPID[LB_Spe].set = Wheel_Speed[LB];
    SysPID[RB_Spe].set = Wheel_Speed[RB];
        
    UpdataPidIcerment(&SysPID[LH_Spe],MotorStatus_t.LH_Spd);  //speed N.3
    UpdataPidIcerment(&SysPID[RH_Spe],MotorStatus_t.RH_Spd);
    UpdataPidIcerment(&SysPID[LB_Spe],MotorStatus_t.LB_Spd);
    UpdataPidIcerment(&SysPID[RB_Spe],MotorStatus_t.RB_Spd);
     
}

void SysCtrl_Angle_PID(uint8 aim_angle,float actual_angle,YesNo_t En_Turn)     
{
        if(En_Turn==Yes)
        {
           PID_Update_Sample(&SysPID[Head_Angle], actual_angle , (YesNo_t)Yes); 
          
           SysPID[Head_Angle].set=aim_angle;
//           SysPID[Head_Angle].sample=actual_angle;
  
           PID_Update_Error(&SysPID[Head_Angle], (YesNo_t)Yes);          
           
           Dof_PID_Update_Calculate(&SysPID[Head_Angle],SysPID[Head_Angle].EnPid_Out); 
        }          
        else 
           SysPID[Head_Angle].Out_Int16 = 0;
     
}

/*!
 *  @brief    增量式PID
 *  @since      v1.0
 */
void UpdataPidIcerment(Dof_PID_t* pPID,float Sample_NewRaw)  //speed N.3
{
  /**更新采样值并滤波**/
//  PID_Update_Sample(pPID, Sample_NewRaw, (YesNo_t)Yes);
  
  /*****计算误差****/
  pPID->err_last_last=pPID->err_last;//更新上上次误差
  pPID->err_last = pPID->err;//更新上次误差
  pPID->err = pPID->set - Sample_NewRaw;//更新本次误差
  
  if(pPID->err > 0)//误差绝对值
      pPID->err_abs = pPID->err;
  else
      pPID->err_abs = -pPID->err; 
  
     
  pPID->err_p = pPID->err-pPID->err_last;//本次误差减去上次误差  
  pPID->err_i = pPID->err;//误差I，不累加
  pPID->err_d = pPID->err-2*pPID->err_last+pPID->err_last_last;  
  

  /*****计算结果*****/
  pPID->out_p = pPID->p * pPID->err_p;//输出P
  pPID->out_i = pPID->i * pPID->err_i;
  pPID->out_d = pPID->d * pPID->err_d;

  if(pPID->out_i < pPID->out_i_min)//积分限幅
      pPID->out_i = pPID->out_i_min;
  else if(pPID->out_i > pPID->out_i_max)
      pPID->out_i = pPID->out_i_max;

  pPID->out = pPID->out_p + pPID->out_i + pPID->out_d;
  pPID->Out_Int16+= (int16)pPID->out;   
  if(pPID->Out_Int16 < pPID->Out_Min)//输出限幅
      pPID->Out_Int16 = (int16)pPID->Out_Min;
  else if(pPID->Out_Int16 > pPID->Out_Max)
      pPID->Out_Int16 = (int16)pPID->Out_Max;   
    
}

void SysCtrl_Speed_PID_Update(void)
{
    if(En_Update_PID == Yes)
        SysCtrl_Speed_PID(X_Speed,Y_Speed,SysPID[Head_Angle].Out_Int16,En_Update_PID);
}





