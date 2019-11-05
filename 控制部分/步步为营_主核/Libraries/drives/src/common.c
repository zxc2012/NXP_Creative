/*********************************************************************************************************************
update:2018/12/30
author:whut_ycy
 ********************************************************************************************************************/
#include "common.h"

uint32 core_clk_mhz;
uint32 bus_clk_mhz;

void get_clk(void)
{
#if defined(MK60DN)
    switch(MK60DN_CLOCK)
    {
        case 0:core_clk_mhz=100;
        break;
        
        case 1:core_clk_mhz=150;
        break;
        
        case 2:core_clk_mhz=180;
        break;
        default:core_clk_mhz=100;
        break;        
    }
#elif defined(MK60F15)
    switch(MK60FX_CLOCK)
    {
        case 0:core_clk_mhz=150;
        break;
        
        case 1:core_clk_mhz=180;
        break;
        
        case 2:core_clk_mhz=200;
        break;
        default:core_clk_mhz=150;
        break;        
    }
#endif
    bus_clk_mhz = core_clk_mhz / (((SIM->CLKDIV1 >> SIM_CLKDIV1_OUTDIV2_SHIFT) & 0x0f) + 1); 
}
unsigned long int Float2U32(float data)    ///////浮点转32位
{
  uint8 * point;
  point = (uint8*)(&data);
  return (((uint32)point[3] << 24) | ((uint32)point[2] << 16) | ((uint32)point[1] << 8) | ((uint32)point[0]));
}
float U322Float(uint64 * data)   /////32位转浮点
{
  return (*((float *)data));
}
