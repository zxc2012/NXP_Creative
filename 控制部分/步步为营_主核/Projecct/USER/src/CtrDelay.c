#include "headfile.h"

#define DelayPort_Down D13 //继电器端口
#define DelayPort_FRONG D12 //继电器端口
#define DelayPort_Left D14 //继电器端口

Relay_t RelayID;

/*
*初始化控制继电器的端口
*/
void InitRelay()
{
    gpio_init (DelayPort_Down, GPO,0);    //初始化 管脚为输出
//    gpio_init (DelayPort_FRONG, GPO,0);    //初始化 管脚为输出
//    gpio_init (DelayPort_Left, GPO,0);    //初始化 管脚为输出
    
    CtrRelay(RelayIdDown,Relay_No);
//    CtrRelay(RelayIdFrong,Relay_No);
//    CtrRelay(RelayIdLeft,Relay_No);
}

/*
*ctrSign：1启动继电器
*		  0关闭继电器
*/
void CtrRelay(Relay_t relay_id,uint8 ctrSign)
{
    if(relay_id == RelayIdDown)
        gpio_set (DelayPort_Down, ctrSign);    //  管脚 输出 1
    else if(relay_id == RelayIdFrong)
        gpio_set (DelayPort_FRONG, ctrSign);  
    else if(relay_id == RelayIdLeft)
        gpio_set (DelayPort_Left, ctrSign);  
}

