#ifndef __VCAN_SERVO_H__
#define __VCAN_SERVO_H__

#include "MK60_ftm.h"


#define MAX_ANGLE 700//最大打角   13/20
#define MIN_ANGLE 600//最小打角


#define SEVO_HEAD_U    0
#define SEVO_HEAD_D    1
#define SEVO_LEFT_U    2
#define SEVO_LEFT_D    3
#define SEVO_MOUNTS    4


typedef struct 
{
    uint32_t ServoSetPwm;
    uint32_t currentServoPwm;
    uint8  isServoMoveOk;
    uint8 Back;
    uint8 Mid;
    uint8 Fetch;
}_SevoCtrParam;

typedef enum 
{
    Servo_Up = 0,
    Servo_Mid,
    Servo_Down,
    EleIron_ON,
    EleIron_OFF,
    OK
}_SevoCtrState;

void Servo_StateCtr();
void SetServoDuty(uint8 sevoID,int16 duty);
void InitServo(void);
extern void TimeChangeServoPwm(void);

extern  uint8      userNow; 
extern _SevoCtrState ServoState[2];
extern _SevoCtrParam SevoCtlParam[2];

#endif