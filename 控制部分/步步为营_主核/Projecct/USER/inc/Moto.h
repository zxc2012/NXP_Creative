#ifndef __MOTO_H_
#define __MOTO_H_

extern void Moto_Init(void);
extern void Moto_Set_Pwm(Wheel_E wheel_e, int PWM_IN);
extern void Car_Move(void);
void StopAll(void);


#endif
