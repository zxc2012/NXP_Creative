/*---------------------------------------------------------------------------------------
                                     电机编码器底层
功能：编码器初始化以及计数读取（未测试）
作者：WK
时间：2018.4.16
说明：本程序读取编码器计数
----------------------------------------------------------------------------------------*/
#include "headfile.h"

float forward_back,left_right;
float POS_actual_value;
float POS_set_value;
float dis_k1;
float dis_k2;

void Pos_cal(uint8 now_position_x,uint8 now_positin_y,uint8 aim_position_x,uint8 aim_positiom_y)
{                                                     //    7,6     5       4,3   2,1
  if(ABS(aim_positiom_y-now_positin_y)==7||ABS(aim_positiom_y-now_positin_y)==6)
    dis_k1=7300;
  else if(ABS(aim_positiom_y-now_positin_y)==5)
    dis_k1=7380;
  else if(ABS(aim_positiom_y-now_positin_y)==4||ABS(aim_positiom_y-now_positin_y)==3)
    dis_k1=7450;
  else if(ABS(aim_positiom_y-now_positin_y)==2||ABS(aim_positiom_y-now_positin_y)==1||ABS(aim_positiom_y-now_positin_y)==0)
    dis_k1=7550;
  
  if(ABS(aim_position_x-now_position_x)==7||ABS(aim_position_x-now_position_x)==6)
    dis_k2=7950;
  else if(ABS(aim_position_x-now_position_x)==5)
    dis_k2=8100;
  else if(ABS(aim_position_x-now_position_x)==4||ABS(aim_position_x-now_position_x)==3)
    dis_k2=8190;
  else if(ABS(aim_position_x-now_position_x)==2||ABS(aim_position_x-now_position_x)==1||ABS(aim_position_x-now_position_x)==0)
    dis_k2=8500;
  forward_back=dis_k1*(aim_positiom_y-now_positin_y);   //   7300    7380   7450   7550
  left_right=-dis_k2*(aim_position_x-now_position_x);    //  7950    8100   8190   8500
  
  if(forward_back>0&&left_right>0)  //右上
    POS_set_value=forward_back+left_right;  //车身目标位移
  else if(forward_back<0&&left_right>0)  //右下
    POS_set_value=forward_back-left_right;  //车身目标位移
  else if(forward_back>0&&left_right<0)  //左上
    POS_set_value=forward_back-left_right;  //车身目标位移
  else if(forward_back<0&&left_right<0)  //左下
    POS_set_value=forward_back+left_right;  //车身目标位移
  else if(forward_back==0)  //正左右
    POS_set_value=forward_back+left_right;  //车身目标位移
  else if(left_right==0)  //正前后
    POS_set_value=forward_back+left_right;  //车身目标位移
}

void Pos_get(void)
{
  
  if(forward_back>0&&left_right>0)  //右上
    POS_actual_value=(Speed_Value_LH+Speed_Value_RB)/2;  //车身实际位移
  else if(forward_back<0&&left_right>0)  //右下
    POS_actual_value=(Speed_Value_RH+Speed_Value_LB)/2;  //车身实际位移
  else if(forward_back>0&&left_right<0)  //左上
    POS_actual_value=(Speed_Value_RH+Speed_Value_LB)/2;  //车身实际位移
  else if(forward_back<0&&left_right<0)  //左下
    POS_actual_value=(Speed_Value_LH+Speed_Value_RB)/2;  //车身实际位移
  else if(forward_back==0)  //正左右
    POS_actual_value=(Speed_Value_LH-Speed_Value_RH+Speed_Value_RB-Speed_Value_LB)/4;  //车身实际位移
  else if(left_right==0)  //正前后
    POS_actual_value=(Speed_Value_LH+Speed_Value_RH+Speed_Value_RB+Speed_Value_LB)/4;  //车身实际位移
}

void Pos_clear (void)
{
  Speed_Value_LH=0;
  Speed_Value_RH=0;
  Speed_Value_LB=0;
  Speed_Value_RB=0;
}

