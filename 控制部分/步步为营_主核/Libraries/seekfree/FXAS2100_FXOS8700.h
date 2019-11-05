#ifndef FXAS2100_FXOS8700_H
#define FXAS2100_FXOS8700_H
#include "headfile.h"


#define IIC_FXAS2100_ADR          0x20
#define FXAS2100_CTRL_REG0        0x0D          ////        0x01   1000dps
#define FXAS2100_CTRL_REG1        0x13          ///         0x00  Stand By    0x02  Active  800HZ
#define FXAS2100_CTRL_REG2        0x14          ////        0x03
#define FXAS2100_ID               0x0C          /////       ID  0xD7
#define FXAS2100_XDATA            0x01

#define IIC_FXOS8700_ADR          0X1E
#define FXOS8700_ID               0x0D          /////ID  0xD7
#define FXOS8700_XYZ_DATA_CFG     0x0E          ////0x01  4G
#define FXOS8700_SYSMOD           0x0B
#define FX0S8700_CTRL_REG1        0x2A 
#define FXOS8700_ACC_XDATA        0x01
#define FXOS8700_MAG_XDATA        0x33

//typedef struct
//{
//  float GYROXdata;
//  float GYROYdata;
//  float GYROZdata;
//  float ACCXdata;
//  float ACCYdata;
//  float ACCZdata;
//  float MAGXdata;
//  float MAGYdata;
//  float MAGZdata;
//}sensorDatatypedef;
extern sensorDatatypedef FXAS2100_FXOS8700_data;

_Bool FXAS2100_FXOS8700_init(void);
_Bool FXAS2100_FXOS8700_DataRead(sensorDatatypedef *Q, uint8 type);
#endif
