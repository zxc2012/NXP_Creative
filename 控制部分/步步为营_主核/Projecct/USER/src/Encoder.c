/*---------------------------------------------------------------------------------------
                                     电机编码器底层
功能：编码器初始化以及计数读取（未测试）
作者：WK
时间：2018.4.16
说明：本程序读取编码器计数
----------------------------------------------------------------------------------------*/
#include "headfile.h"
Motor_St MotorStatus_t={0};//车轮状态

/*------------------------
     电机编码器初始化
------------------------*/
void Encoder_Init(void)
{
	ftm_quad_init(ftm1);	//
	ftm_quad_init(ftm2);	//

}




/*------------------------------------------------------------------------------
 采集电机速度
------------------------------------------------------------------------------*/
void MOTOR_get(void)
{
	MotorStatus_t.LH_Spd=ftm_quad_get(ftm1);	
	ftm_quad_clean(ftm1);

	MotorStatus_t.RH_Spd=-ftm_quad_get(ftm2);
	ftm_quad_clean(ftm2);
        
        Speed_Value_LH+=MotorStatus_t.LH_Spd;
        Speed_Value_RH+=MotorStatus_t.RH_Spd;

}

void ALLEncoder_clear (void)
{
  ftm_quad_clean(ftm1);
  ftm_quad_clean(ftm2);
  Communication_send(1);
}

