#ifndef _DUTY_H_
#define _DUTY_H_

typedef struct
{
	uint8_t check_flag;
	uint8_t err_flag;
	int16_t cnt_1ms;
	int16_t cnt_2ms;
	int16_t cnt_5ms;
	int16_t cnt_10ms;
	int16_t cnt_20ms;
	int16_t cnt_50ms;
	int16_t cnt_1000ms;
	int16_t time;
}loop_t;
void Loop_check();
void Duty_Loop() ;
void Car_EnterTo(void);
extern void car_serch (void);
extern car_serch_action car_move_state;

#endif