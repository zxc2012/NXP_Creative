#include "ctrl_basic.h"

uint32 SysDelay_ms_Cnt = 0;
uint32 Sys_ms = 0;

float Average_Float(float* pDataBuf, uint16 num)
{
    double Sum;
    uint16 i;
    float Average;
    
    Sum = 0.0;
    for(i=0; i<num; i++)
        Sum += pDataBuf[i];
    
    Average = Sum / num;
    
    return Average;
}

void Fill_Mem(uint8* pAdd, uint16 Len, uint8 FillWith)
{
    uint16 i;
    
    for(i=0; i< Len; i++)
    {
        pAdd[i] = FillWith;
    }
}

void SysDelay_ms(uint32 ms)
{
    SysDelay_ms_Cnt = 0;
    
    while(SysDelay_ms_Cnt < ms)
        ;
}

float Abs(float num)
{
    if(num >= 0)
        return num;
    else
        return -num;
}

void Constraint(float* pFloatNum, float Min, float Max)
{
    if(Min <= Max)
    {
        if(*pFloatNum < Min)
            *pFloatNum = Min;
        else if(*pFloatNum > Max)
            *pFloatNum = Max;
    }
}

void Constraint_By_Abs(float* pFloatNum, float AbsMax)
{
    Constraint(pFloatNum, -AbsMax, AbsMax);
}

float DataFilter(float* pDataBuf, uint16 Num)
{
    //static float DataCache[20];
    static double Sum;
    uint16 i;
    
    if(1 == Num)
    {
        return pDataBuf[0];
    }
    else if(2 == Num)
    {
        Sum = (pDataBuf[0] + pDataBuf[1]) / 2;
        return (float)Sum;
    }
    else if(Num >= 3)
    {
        for(i=0; i<Num; i++)
        {   //DataCache[i] = pDataBuf[i];
        
//        Data_Quick_Sort(DataCache, Num);
        Sum = 0.00;
//        for(i=1; i<=Num-2; i++)
            Sum += pDataBuf[i];
        }
        return Sum/Num;
    }
    
    return 0;
}

void Data_Quick_Sort(float* pDatas, uint16 Num)
{
    uint16 CntL, CntR;
    float Compare;
    
    if(Num < 2)
        return;
	
    CntL = 0;
    CntR = Num - 1;
	
	Compare = pDatas[CntL];  		// first item saved, and be used for comparer
   
    while(CntL < CntR)
    {
        while(CntL < CntR)
        {
            if(pDatas[CntR] < Compare)
            {
                pDatas[CntL] = pDatas[CntR];
                break;
            }
            CntR--;
        }
        
        while(CntL < CntR)
        {
            if(pDatas[CntL] > Compare)
            {
                pDatas[CntR] = pDatas[CntL];
                break;
            }
            CntL++;
        }
    }
    
    pDatas[CntL++] = Compare;                       // rember to cover the last saved element with Comparer
    Data_Quick_Sort(&pDatas[0], CntL);              // pDatas[0]        ~   pDatas[CntL-1]
    Data_Quick_Sort(&pDatas[CntL], Num-(CntL));     // pDatas[CntL] ~   pDatas[Num-1]
}

