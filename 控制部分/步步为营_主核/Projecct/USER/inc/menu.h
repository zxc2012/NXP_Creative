#ifndef _MENU_H_
#define _MENU_H_
#include "headfile.h"

#define max_tab 3
typedef  void  ap_func(void);

typedef struct
{
    uint8   *   strbuff;      //×Ö·û´®Í¼°¸

    ap_func *   pfunc;        //Ö´ÐÐº¯Êý

}menu_t;


extern uint8 MENU_flag;
extern uint8 speed_select;
#endif  //_MENU_H_