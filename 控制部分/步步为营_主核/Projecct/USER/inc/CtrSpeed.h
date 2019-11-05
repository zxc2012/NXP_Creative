#ifndef _CTRSPEED_H_
#define _CTRSPEED_H_


#define C_Point   110	//转折点
#define MAX_SPEED 70	//最大速度
#define MIN_SPEED 30	//最小速度
#define StartPid  60  //开启PID的位置


int16 GetSpeedByDistance(int16 distance);
void InitSpeedWithDistance();

#endif