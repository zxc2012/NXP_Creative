#ifndef __CTRL_BASIC_H__
#define __CTRL_BASIC_H__

#include "common.h"



typedef enum
{
    Failure = 0,
    Success,
}OptSta_t;

#define FLOAT_0                     ((float)(0.0001))
#define FLOAT_EQUAL(x, cmp)         ((((x)-(cmp)) > -FLOAT_0) && (((x)-(cmp)) < FLOAT_0))

extern uint32 SysDelay_ms_Cnt;
extern uint32 Sys_ms;

extern float Average_Float(float* pDataBuf, uint16 num);
extern void Fill_Mem(uint8* pAdd, uint16 Len, uint8 FillWith);
extern void SysDelay_ms(uint32 ms);
extern float Abs(float num);
extern void Constraint(float* pFloatNum, float Min, float Max);
extern void Constraint_By_Abs(float* pFloatNum, float AbsMax);
extern float DataFilter(float* pDataBuf, uint16 Num);
extern void Data_Quick_Sort(float* pDataBuf, uint16 Num);

#endif	// __CTRL_BASIC_H__

