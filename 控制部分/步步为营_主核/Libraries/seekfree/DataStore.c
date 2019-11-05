#include "headfile.h"
#include "Attitude_Calculation.h"
#include "DataStore.h"
#include "isr.h"

FLASH_WRITE_TYPE   FLASH_DATA[FLASHNUM];

_Bool My_FlashWrite(uint16 Boot)        /////写入
{
    FLASH_ERASE(BOOTSECTOR - Boot);
    for(int n = 0; n < FLASHNUM; n++)
    {
       FLASH_WRITE((BOOTSECTOR - Boot), (FLASH_ALIGN_ADDR * n), FLASH_DATA[n]);
       FLASH_DELAY(4000);
    }
    return 1;
}

_Bool My_FlashRead(uint16 Boot)          ////读出
{
    for(int n = 0; n < FLASHNUM; n++)
    {
       FLASH_DATA[n] = FLASH_READ((BOOTSECTOR - Boot), (FLASH_ALIGN_ADDR * n), FLASH_WRITE_TYPE);
       FLASH_DELAY(4000);
    }
    return 1;
}


extern AttitudeDatatypedef    GyroOffset;
extern float i;
void DataSave(void)                       ///////数据保存
{
  FLASH_DATA[0] = Float2U32(GyroOffset.Xdata);
  FLASH_DATA[1] = Float2U32(GyroOffset.Ydata);
  FLASH_DATA[2] = Float2U32(GyroOffset.Zdata);
  //FLASH_DATA[3] = Float2U32(i);
//  FLASH_DATA[3] = ad_coeff[0];
//  FLASH_DATA[4] = ad_coeff[1];
//  FLASH_DATA[5] = ad_coeff[2];
//  FLASH_DATA[6] = ad_coeff[3];
  My_FlashWrite(0);
  return;
}

void DataLoad(void)                       ///////数据载入
{
  My_FlashRead(0);
  GyroOffset.Xdata = U322Float(&FLASH_DATA[0]);
  GyroOffset.Ydata = U322Float(&FLASH_DATA[1]);
  GyroOffset.Zdata = U322Float(&FLASH_DATA[2]);
  //i=U322Float(&FLASH_DATA[3]);
//  ad_coeff[0]=(uint16)(&FLASH_DATA[3]);
//  ad_coeff[1]=(uint16)(&FLASH_DATA[4]);
//  ad_coeff[2]=(uint16)(&FLASH_DATA[5]);
//  ad_coeff[3]=(uint16)(&FLASH_DATA[6]);
}


void Data_init(void)
{
  FLASH_init();
  systick_delay_ms(100);
  DataLoad();
}
