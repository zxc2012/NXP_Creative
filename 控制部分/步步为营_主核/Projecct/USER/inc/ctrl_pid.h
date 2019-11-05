#ifndef __ctrl_pid_H__
#define __ctrl_pid_H__

#include "headfile.h"
#include "Find_path.h"

#define SAMPLE_CACHE_MAX  10
typedef enum
{
   X_Dir = 0,
   Y_Dir,
   position,
   Head_Angle,
   LH_Spe,
   RH_Spe,
   LB_Spe,
   RB_Spe,
   Linear_Pos,
   Dof_Num,
}Dof_t;

typedef struct
{
    YesNo_t EnPid_Out;
    float Out_Float;
    int16 Out_Int16;
    float Out_Min;
    float Out_Max;
    
    float p;
    float i;
    float d;
    float sample;//ֵ采样值
    float sample_last;//上次采样值
    float sample_last_last;//上上次采样值
    float sample_cache[SAMPLE_CACHE_MAX];//采样存储数组
    uint16 sample_cache_cnt;//采样数组计数
    uint16 sample_cache_max;//采样数组最大值
    float set;
    float last_set; 
    float err;
    float err_abs;
    float err_last;
    float err_last_last;
    float err_p;
    float err_i;
    float err_d;
    float err_last_d;
    float err_i_abs_max;//积分最大值
    float i_act_err_abs_max;//积分分离
    
    float out;
    float out_p;
    float out_i;
    float out_i_min;
    float out_i_max;
    float out_d;
}Dof_PID_t;

typedef struct
{
    float sample;
    YesNo_t valid;
}PID_Sample_t;

extern YesNo_t En_Update_PID;
extern Dof_PID_t SysPID[Dof_Num];
extern int16 Y_Speed,X_Speed;
extern int Speed_Value_LH;   
extern int Speed_Value_RH;    
extern int Speed_Value_RB;     
extern int Speed_Value_LB; 
extern float Angle_to_Ctro;
void PID_Parameter_Init(void);
void PID_Update_Sample(Dof_PID_t* pPID, float Sample_NewRaw, YesNo_t Sample_Valid);//采样
void PID_Update_Error(Dof_PID_t* pPID, YesNo_t Sample_Valid);//误差
void Dof_PID_Update_Calculate(Dof_PID_t* pPID,YesNo_t En);//计算
float PID_Update_Out(Dof_PID_t* pPID);//输出

extern void SysCtrl_Speed_PID_Update(void);
extern void UpdataPidIcerment(Dof_PID_t* pPID,float Sample_NewRaw);
extern void SysCtrl_Update_PID(void);
extern void position_pid (xy_t now,int num);
void SysCtrl_Speed_PID(int16 Speedx,int16 Speedy,int16 Ang_Speed,YesNo_t En_Turn);
extern void SysCtrl_Angle_PID(uint8 aim_angle,float actual_angle,YesNo_t En_Turn); 

#endif  // __PID_H__

