#ifndef _VCAN_TRACK_H_
#define _VCAN_TRACK_H_


//#define YU_VALUE  (Y_DIS_PER_PIX*4+Y_DIS_PER_PIX/2)
//#define YU_VALUE  (Y_DIS_PER_PIX*4+Y_DIS_PER_PIX/2)


//各种控制变量设置的缓冲
typedef struct
{
  uint8 isOKFlag;
  double delt;
  uint16 times;
  uint16 count;
}varBufferClass;

typedef enum
{
  Car_straight = 0,
  Car_stop,
  Car_setpoint,
  Car_move_q,
  Car_place_obstacle
}Spd_Action_E;

typedef enum
{
  Car_serching = 0,
  Car_chessing
}car_serch_action;

typedef enum{
	Left_E=0,
	Right_E
}DIR_E;

extern float WALK_SPEED;
extern float WALK_SPEED_SET;

void Track_PlaceObstacle(void);
void Track_EightQueen(void);
YesNo_t IsCarFindPoint(void);
YesNo_t IsCarFindNext();
YesNo_t IsCarSetOK(void);
void Calculate_Once(void);
void C_Replace_queen(void);
void Replace_queen(void);
uint16 GetRecordValueX(DIR_E dir);
uint8 variableBuffer(float inValue,float setValue,float *variable,varBufferClass *varBuffer);
float MyFuzzy(void);
void SendBeforeOrAfter(void);

void Judge_Queen_UpDown(void);
void Judge_Obstacle_Down(void);
YesNo_t IsCarSetOK_Obstacle();
void SpeedUpdate();
void Out_Of_Board(void);

extern Spd_Action_E SpdAct;
extern uint8 cal_n;
extern uint8 step;
extern varBufferClass varBufferClass_t;
extern uint8 CarEnterOk;
extern float DiagonalDis;
extern float x_error;
extern float y_error;

#endif