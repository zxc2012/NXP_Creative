#ifndef __Pos_cal_H_
#define __Pos_cal_H_

#include "headfile.h"
/*
* 变量声明区
*/
extern float forward_back,left_right;
extern float POS_actual_value;
extern float POS_set_value;

/*
* 函数声明区
*/
void Pos_cal(uint8 now_position_x,uint8 now_positin_y,uint8 aim_position_x,uint8 aim_positiom_y);
void Pos_get(void);
void Pos_clear (void);


#endif