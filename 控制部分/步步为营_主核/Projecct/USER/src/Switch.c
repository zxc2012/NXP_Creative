#include "headfile.h"

uint8 SwitchMode = 0;

void Switch_Mode(void)
{
    
    gpio_init (A14, GPI,0);
    gpio_init (A15, GPI,0);
    gpio_init (A16, GPI,0);
    gpio_init (A17, GPI,0);
    
    port_init_NoAlt(A14, PULLUP);
    port_init_NoAlt(A15, PULLUP);
    port_init_NoAlt(A16, PULLUP);
    port_init_NoAlt(A17, PULLUP);
    
    SwitchMode = gpio_get (A14);
    SwitchMode |= gpio_get (A15)<<1;
    SwitchMode |= gpio_get (A16)<<2;
    SwitchMode |= gpio_get (A17)<<3;
}