/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * All rights reserved.
 * 中南大学黄竞辉
 *
 * @file       		FXAS2100_FXOS8700
 * @author     		Alex
 * @version    		v1.0
 * @Software 		MDK
 * @date       		2017-11-9
 ********************************************************************************************************************/

#include "FXAS2100_FXOS8700.h"

_Bool FXAS2100_FXOS8700_init(void)
{
  /************************************/
  /*陀螺仪配置*/
  /************************************/
  uint8 ErrCount = 0;
  while(IIC_Read_Reg(IIC_FXAS2100_ADR, FXAS2100_ID) != 0xD7)   /////确认芯片ID
  {
    ErrCount++;
    if(ErrCount > 5)
      return 0;
  }
  if(IIC_Write_Reg(IIC_FXAS2100_ADR, FXAS2100_CTRL_REG1, 0x00) == 0)return 0;  ///stand by
  systick_delay_ms(10);
  if(IIC_Write_Reg(IIC_FXAS2100_ADR, FXAS2100_CTRL_REG0, 0x01) == 0)return 0;   ///1000dps
  systick_delay_ms(10);
  if(IIC_Write_Reg(IIC_FXAS2100_ADR, FXAS2100_CTRL_REG2, 0x03) == 0)return 0;  
  systick_delay_ms(10);
  if(IIC_Write_Reg(IIC_FXAS2100_ADR, FXAS2100_CTRL_REG1, 0x02) == 0)return 0;   ////800hz
  systick_delay_ms(10);

  
  /************************************/
  /*加速度配置*/
  /************************************/
  ErrCount = 0;
  while(IIC_Read_Reg(IIC_FXOS8700_ADR, FXOS8700_ID) != 0xC7)   /////确认芯片ID
  {
    ErrCount++;
    if(ErrCount > 5)
      return 0;
  }
  if(IIC_Write_Reg(IIC_FXOS8700_ADR, FXOS8700_SYSMOD, 0x00) == 0)return 0;          ///standby
  systick_delay_ms(10);
  if(IIC_Write_Reg(IIC_FXOS8700_ADR, FXOS8700_XYZ_DATA_CFG, 0x01) == 0)return 0;    ///4G
  systick_delay_ms(10);
  if(IIC_Write_Reg(IIC_FXOS8700_ADR, FX0S8700_CTRL_REG1, 0x05) == 0)return 0;       ///
  systick_delay_ms(10);
  
  /************************************/
  /*磁力计配置*/
  /************************************/
  return 1;
}


_Bool FXAS2100_FXOS8700_DataRead(sensorDatatypedef *Q, uint8 type)
{
  /************************************/
  /*陀螺仪数据读取*/
  /************************************/	
  uint8 datatemp[6] = {0};
  if(IIC_Read_Buff(IIC_FXAS2100_ADR, FXAS2100_XDATA, datatemp, 6) == 0)return 0;
  Q->GYROXdata = (float)((int16)((datatemp[0] << 8) | datatemp[1]));
  Q->GYROYdata = (float)((int16)((datatemp[2] << 8) | datatemp[3]));
  Q->GYROZdata = (float)((int16)((datatemp[4] << 8) | datatemp[5]));
  /************************************/
  /*加速度数据读取*/
  /************************************/
  if(IIC_Read_Buff(IIC_FXOS8700_ADR, FXOS8700_ACC_XDATA, datatemp, 6) == 0)return 0;
  Q->ACCXdata = (float)((int16)((datatemp[0] << 8) | datatemp[1]) >> 2);
  Q->ACCYdata = (float)((int16)((datatemp[2] << 8) | datatemp[3]) >> 2);
  Q->ACCZdata = (float)((int16)((datatemp[4] << 8) | datatemp[5]) >> 2);
  if(type)
  {
    if(IIC_Read_Buff(IIC_FXOS8700_ADR, FXOS8700_MAG_XDATA, datatemp, 6) == 0)return 0;
    Q->MAGXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]) >> 3);
    Q->MAGYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]) >> 3);
    Q->MAGZdata = (float)((int16)((datatemp[5] << 8) | datatemp[4]) >> 1);
  }
//  if(IIC_Read_Buff(IIC_FXOS8700_ADR, FXOS8700_ACC_XDATA, datatemp, 12) == 0)return 0;
//  Q->ACCXdata = (float)((int16)((datatemp[0] << 8) | datatemp[1]) >> 2);
//  Q->ACCYdata = (float)((int16)((datatemp[2] << 8) | datatemp[3]) >> 2);
//  Q->ACCZdata = (float)((int16)((datatemp[4] << 8) | datatemp[5]) >> 2);
//    Q->MAGXdata = (float)((int16)(datatemp[6] << 8) | datatemp[7]);
//    Q->MAGYdata = (float)((int16)(datatemp[8] << 8) | datatemp[9]);
//    Q->MAGZdata = (float)((int16)(datatemp[10] << 8) | datatemp[11]);
  

  return 1;
}

