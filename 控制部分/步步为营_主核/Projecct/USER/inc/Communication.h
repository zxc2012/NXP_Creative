#ifndef _communication_h
#define _communication_h
#include "headfile.h"

void Connunication_init(void);
void UART2_RX_ISR(void) ;
extern void Communication_send(uint8 num);

//void Connunication2_init(void);
//void UART0_RX_ISR(void) ;
#endif

