#ifndef _CTR_DELAY_
#define _CTR_DELAY_

#define Relay_Yes      1
#define Relay_No       0

typedef enum{
    RelayIdDown = 0,
    RelayIdFrong,
    RelayIdLeft,
}Relay_t;

void InitRelay(void);
void CtrRelay(Relay_t relay_id,uint8 ctrSign);

extern Relay_t RelayID;
#endif 