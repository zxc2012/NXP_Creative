#include "headfile.h"


uint16 a_Speed=540;

int16 SpeedBuff[C_Point-StartPid];	//速度存储

/*
*根据距离返回速度设定值
*输入：距离
*输出：速度设定值
*/

int16 GetSpeedByDistance(int16 distance)
{
    static uint32_t m_spd_2=MIN_SPEED*MIN_SPEED;//最小速度平方
    int16 setSpeed=0;
	if(distance<0)		//绝对值
	   distance=-distance;
        distance/=100;
        
	if(distance>=C_Point )//分段函数
	{
		return MAX_SPEED; 
	}
	else if(distance<=StartPid)
	{
		return MIN_SPEED ;
	}
        
	return SpeedBuff[distance-StartPid];
	

}

/*
*计算速度随距离的变化值
*/
void InitSpeedWithDistance()
{

  uint16 i=0;
  float k=(float)(MAX_SPEED-MIN_SPEED)/(C_Point-StartPid);
  float b=MIN_SPEED-k*StartPid;
  int16 dis=0;
    for(i=0;i<(C_Point-StartPid);i++)
    {
            dis=i+StartPid;

    //             SpeedBuff[i]=-0.00807*dis*dis+3.426*dis-62.456;   //中速（最开始）
            SpeedBuff[i]=(int16)(k*dis+b);   //
    }

}

//
//void CtrSpeedPara(int16 distance,uint16 CurrentSpeed)
//{
//   if(distance<0)		//绝对值
//	distance=-distance;
//   C_Point=( CurrentSpeed*CurrentSpeed-MIN_SPEED*MIN_SPEED)/a_Speed+StartPid;
//   InitSpeedWithDistance();
//    
//}