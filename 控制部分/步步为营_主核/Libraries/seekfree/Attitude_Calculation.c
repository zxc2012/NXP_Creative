/*********************************************************************************************************************
 *
 * @file       		Attitude_Calculation.c
 * @author     		Alex
 * @version    		v1.0
 * @Software 		IAR 8.1
 * @date       		2016-11-9
 ********************************************************************************************************************/

#include "headfile.h"
#include "Attitude_Calculation.h"

AttitudeDatatypedef         Acc;
AttitudeDatatypedef         Gyro;
u8 sensor=0;
#if (sensor==1)//BMX055
#define  XA    (-Acc.Xdata)
#define  YA     (Acc.Zdata)  
#define  ZA    (-Acc.Ydata)

#define  XG    (Gyro.Xdata)
#define  YG   (-Gyro.Zdata)
#define  ZG   (Gyro.Ydata)

#elif (sensor==0)//FXAS2100_FXOS8700
#define  XA    (-Acc.Xdata)
#define  YA     (Acc.Zdata)  
#define  ZA    (-Acc.Ydata)

#define  XG    (-Gyro.Xdata)
#define  YG   (-Gyro.Zdata)
#define  ZG   (-Gyro.Ydata)
#endif
pid_s wy_pid,angle_pid,wx_pid,v_pid,dir_pid;
#define  ATTITUDE_COMPENSATE_LIMIT   ((float)PI*PERIODS / 180  )


QuaternionTypedef    Quaternion;
EulerAngleTypedef    EulerAngle;
QuaternionTypedef    AxisAngle;
EulerAngleTypedef    EulerAngleRate;

QuaternionTypedef    MeaQuaternion;
EulerAngleTypedef    MeaEulerAngle;
QuaternionTypedef    MeaAxisAngle;

QuaternionTypedef    ErrQuaternion;
EulerAngleTypedef    ErrEulerAngle;
QuaternionTypedef    ErrAxisAngle;
coef_tag other_coef;
float angle_coef[4]={0,0,0,0};
float v_coef[4]={0,0,0,0};
float wx_coef[4]={0,0,0,0};
float wy_coef[4]={0,0,0,0};
float dir_coef[4]={0,0,0,0};
float XN, XE, XD;
float YN, YE, YD;
float ZN, ZE, ZD;


vuint8                 IsGyroOffsetReset =1;     /////如果需要进行陀螺仪零飘矫正则将改变量置为   1
sensorDatatypedef      sensor_data;
EulerAngleTypedef      SystemAttitude;            /////姿态角
EulerAngleTypedef      SystemAttitudeRate;        /////姿态角速度
AttitudeDatatypedef    GyroOffset;
float a_angle=0;                      //加速度计算出车身俯仰角
float car_angle=0;
float SysYaw_last;
int32 car_diroutcore=0;               

void Quaternion_Normalize(QuaternionTypedef *Qu)       //四元数归一化
{
	float Normal = 0;
	Normal = sqrtf(Qu->W * Qu->W + Qu->X * Qu->X + Qu->Y * Qu->Y + Qu->Z * Qu->Z);
	if (isnan(Normal) || Normal <= 0)
	{
		Qu->W = 1;
		Qu->X = 0;
		Qu->Y = 0;
		Qu->Z = 0;
	}
	else
	{
		Qu->W /= Normal;
		Qu->X /= Normal;
		Qu->Y /= Normal;
		Qu->Z /= Normal;
	}
}


// 四元数求逆, p=q^-1=q*/(|q|^2), 原始四元数需为单位四元数
void Quaternion_Invert(QuaternionTypedef *p, const QuaternionTypedef *q)
{
	p->W = q->W;
	p->X = -q->X;
	p->Y = -q->Y;
	p->Z = -q->Z;
}


// 四元数乘法, result=pq
void Quaternion_Multi(QuaternionTypedef *result, const QuaternionTypedef *p, const QuaternionTypedef *q)
{
	result->W = p->W * q->W - p->X * q->X - p->Y * q->Y - p->Z * q->Z;
	result->X = p->W * q->X + p->X * q->W - p->Y * q->Z + p->Z * q->Y;
	result->Y = p->W * q->Y + p->X * q->Z + p->Y * q->W - p->Z * q->X;
	result->Z = p->W * q->Z - p->X * q->Y + p->Y * q->X + p->Z * q->W;
}



void Quaternion_ToEulerAngle(const QuaternionTypedef *q, EulerAngleTypedef *e)  //四元数转欧拉角
{
	e->Roll = atan2f(2 * (q->W * q->X + q->Y * q->Z), 1 - 2 * (q->X * q->X + q->Y * q->Y));
	float k = 2 * (q->W * q->Y - q->Z * q->X);
	if (k > 1) k = 1;
	else if (k < -1) k = -1;
	e->Pitch = asinf(k);
	e->Yaw = atan2f(2 * (q->W * q->Z + q->X * q->Y), 1 - 2 * (q->Y * q->Y + q->Z * q->Z));
}


void Quaternion_FromEulerAngle(QuaternionTypedef *q, const EulerAngleTypedef *e) //欧拉角转四元数
{
	float cosx = cosf(e->Roll / 2);
	float sinx = sinf(e->Roll / 2);
	float cosy = cosf(e->Pitch / 2);
	float siny = sinf(e->Pitch / 2);
	float cosz = cosf(e->Yaw / 2);
	float sinz = sinf(e->Yaw / 2);
	q->W = cosx * cosy * cosz + sinx * siny * sinz;
	q->X = sinx * cosy * cosz - cosx * siny * sinz;
	q->Y = cosx * siny * cosz + sinx * cosy * sinz;
	q->Z = cosx * cosy * sinz - sinx * siny * cosz;
}

void Quaternion_ToAxisAngle(const QuaternionTypedef *q, QuaternionTypedef *a) //四元数转轴角
{
	a->W = 0;
	a->X = q->X;
	a->Y = q->Y;
	a->Z = q->Z;
	Quaternion_Normalize(a);
	a->W = acosf(q->W) * 2;
}

// 轴角转换为四元数
void Quaternion_FromAxisAngle(QuaternionTypedef *q, const QuaternionTypedef *a)
{
	float c = cosf(a->W / 2);
	float s = sinf(a->W / 2);
	q->W = 0;
	q->X = a->X;
	q->Y = a->Y;
	q->Z = a->Z;
	Quaternion_Normalize(q);
	q->W = c;
	q->X *= s;
	q->Y *= s;
	q->Z *= s;
}



//角速度周期转化四元数增量
void Quaternion_FromGyro(QuaternionTypedef *q, float wx, float wy, float wz, float dt)
{
	q->W = 1;
	q->X = wx * dt / 2;
	q->Y = wy * dt / 2;
	q->Z = wz * dt / 2;
	Quaternion_Normalize(q);
}


// 使用角速度更新四元数(一阶龙哥库塔法Runge-Kunta, 等价于使用角度增量)
void Quaternion_UpdateFromGyro(QuaternionTypedef *q, float x, float y, float z, float dt)
{
	float dW = 0.5 * (-q->X * x - q->Y * y - q->Z * z) * dt;
	float dX = 0.5 * (q->W * x + q->Y * z - q->Z * y) * dt;
	float dY = 0.5 * (q->W * y - q->X * z + q->Z * x) * dt;
	float dZ = 0.5 * (q->W * z + q->X * y - q->Y * x) * dt;
	q->W += dW;
	q->X += dX;
	q->Y += dY;
	q->Z += dZ;
	Quaternion_Normalize(q);
}



void QuaternionFromAcc(QuaternionTypedef *Qu, float ax, float ay, float az, float mx, float my, float mz)//采集加速度计获得四元数
{
	float Normal = 0;
	XD = ax;
	YD = ay;
	ZD = az;      //取重力方向
	Normal = sqrtf(XD * XD + YD * YD + ZD * ZD);
	XD /= Normal;
	YD /= Normal;
	ZD /= Normal;                //归一化


	XN = -mx; YN = -my; ZN = -mz;              //取磁力反方向为北
	Normal = XD * XN + YD * YN + ZD * ZN;      //北
	XN -= Normal*XD;
	YN -= Normal*YD;
	ZN -= Normal*ZD;                       //正交化
	Normal = sqrtf(XN * XN + YN * YN + ZN * ZN);
	XN /= Normal;
	YN /= Normal;
	ZN /= Normal;                       //归一化

	XE = YD*ZN - YN*ZD;                     //东//正交化
	YE = ZD*XN - ZN*XD;
	ZE = XD*YN - XN*YD;
	Normal = sqrtf(XE*XE + YE*YE + ZE*ZE);
	XE /= Normal;
	XE /= Normal;
	XE /= Normal;                   //归一化

	Qu->W = 0.5 * sqrtf(XN + YE + ZD + 1);   //旋转矩阵转换四元数
	Qu->X = (YD - ZE) / (4 * Qu->W);
	Qu->Y = (ZN - XD) / (4 * Qu->W);
	Qu->Z = (XE - YN) / (4 * Qu->W);
	Quaternion_Normalize(Qu);
	return;
}


void Quaternion_init()
{
	if (XA != 0 || YA != 0 || ZA != 0)
	{
		QuaternionFromAcc(&Quaternion, XA, YA, ZA, -1, 0, 0);
	}
	else
	{
		Quaternion.W = 1;
		Quaternion.X = 0;
		Quaternion.Y = 0;
		Quaternion.Z = 0;
	}
	Quaternion_ToEulerAngle(&Quaternion, &EulerAngle);
	Quaternion_ToAxisAngle(&Quaternion, &AxisAngle);
	EulerAngleRate.Pitch = 0;
	EulerAngleRate.Roll = 0;
	EulerAngleRate.Yaw = 0;
}


void Attitude_UpdateAcc(void)//深度融合更新
{
	QuaternionTypedef    EstQuaternion;
	EulerAngleTypedef    EstEulerAngle;
	QuaternionTypedef    DivQuaternion;
	QuaternionTypedef    ComAxisangle;
	QuaternionTypedef    Compensate;
	QuaternionTypedef    Last;


	QuaternionFromAcc(&MeaQuaternion, 0, YA, ZA, -1, 0, 0);
	Quaternion_ToEulerAngle(&MeaQuaternion, &MeaEulerAngle);
	Quaternion_ToAxisAngle(&MeaQuaternion, &MeaAxisAngle); //计算当前加速度计姿态

	EstEulerAngle.Roll = EulerAngle.Roll;
	EstEulerAngle.Pitch = EulerAngle.Pitch;
	EstEulerAngle.Yaw = 0;

	Quaternion_FromEulerAngle(&EstQuaternion, &EstEulerAngle);  //估计欧拉角转四元数

	   //计算估计与测得四元数偏差
	Quaternion_Invert(&DivQuaternion, &EstQuaternion);
	Quaternion_Multi(&ErrQuaternion, &DivQuaternion, &MeaQuaternion);
	Quaternion_Normalize(&ErrQuaternion);
	Quaternion_ToEulerAngle(&ErrQuaternion, &ErrEulerAngle);
	Quaternion_ToAxisAngle(&ErrQuaternion, &ErrAxisAngle);


	//轴角校正限幅

	memcpy(&ComAxisangle, &ErrAxisAngle, sizeof(QuaternionTypedef));
	if (ComAxisangle.W > ATTITUDE_COMPENSATE_LIMIT)
	{
		ComAxisangle.W = ATTITUDE_COMPENSATE_LIMIT;
	}
	Quaternion_FromAxisAngle(&Compensate, &ComAxisangle);


	//执行校正

	memcpy(&Last, &EstQuaternion, sizeof(QuaternionTypedef));
	Quaternion_Multi(&EstQuaternion, &Last, &Compensate);


	Quaternion_ToEulerAngle(&EstQuaternion, &EstEulerAngle);
	EstEulerAngle.Yaw = EulerAngle.Yaw;//不使用加速度计测偏航角
	Quaternion_FromEulerAngle(&Quaternion, &EstEulerAngle);
	Quaternion_ToEulerAngle(&Quaternion, &EulerAngle);
	Quaternion_ToAxisAngle(&Quaternion, &AxisAngle);
}


void Attitude_UpdateGyro(void)  //快速更新
{

	QuaternionTypedef   g1, tmp;
	EulerAngleTypedef   LastEulerAngle;
	QuaternionTypedef   LastQuanternion;

	//保留上一次的欧拉角和四元数
	memcpy(&LastEulerAngle, &EulerAngle, sizeof(EulerAngleTypedef));
	memcpy(&LastQuanternion, &Quaternion, sizeof(QuaternionTypedef));

	//进行姿态更新
	float gx = XG / 180 * PI;
	float gy = YG / 180 * PI;
	float gz = ZG / 180 * PI;

	Quaternion_UpdateFromGyro(&Quaternion, gx, gy, gz, PERIODS);
	Quaternion_ToEulerAngle(&Quaternion, &EulerAngle);
	Quaternion_ToAxisAngle(&Quaternion, &AxisAngle);

	// 计算欧拉角速度
	// Yaw为偏航角速度,为绕NED中的D轴(Z轴)旋转的角速度,使用四元数计算

	g1.W = 0; g1.X = gx; g1.Y = gy; g1.Z = gz;

	Quaternion_Invert(&LastQuanternion, &LastQuanternion);
	Quaternion_Multi(&tmp, &LastQuanternion, &g1);
	Quaternion_Invert(&LastQuanternion, &LastQuanternion);
	Quaternion_Multi(&g1, &tmp, &LastQuanternion);

	EulerAngleRate.Yaw = g1.Z;
	//Pitch为俯仰角速度, 为绕Y轴旋转的角速度, 使用???计算
	if (fabs(LastEulerAngle.Pitch - EulerAngle.Pitch) < PI / 2)
	{
		EulerAngleRate.Pitch = EulerAngle.Pitch - LastEulerAngle.Pitch;
	}
	else if (EulerAngle.Pitch - LastEulerAngle.Pitch > PI / 2)
	{
		EulerAngleRate.Pitch = -PI + (EulerAngle.Pitch - LastEulerAngle.Pitch);
	}
	else if (EulerAngle.Pitch - LastEulerAngle.Pitch < -PI / 2)
	{
		EulerAngleRate.Pitch = PI + (EulerAngle.Pitch - LastEulerAngle.Pitch);
	}

	EulerAngleRate.Pitch /= PERIODS;
	//Roll为横滚角速度, 绕X''轴旋转的角速度, 直接使用陀螺仪数据
	EulerAngleRate.Roll = gx;
}
int32 PID_Position_angle(pid_s *pid,float *coef,int32 result,int32 point)
{
       int32 realize;
       
       int32 error;

       error=point-result;

       //pid->errorsum+=error*coef[Ki];

       //pid->errorsum=XIANFU(pid->errorsum,-coef[Kt],coef[Kt]);

       realize=(int32)(error*coef[0]-SystemAttitudeRate.Pitch*100*coef[1]);//没有I项，为了UI方便而已

       pid->lasterror=error;

       return realize;

}



/*-----------------------------------------------------------------------------------------
* PID_Position
* 位置式PID
---------------------------------------------------------------------------------------------*/

int32 PID_Position(pid_s *pid,float *coef,int32 result,int32 point)
{
       int32 error,realize;

       error=point-result;

       pid->errorsum+=error*coef[Ki];

       pid->errorsum=XIANFU(pid->errorsum,-coef[Kt],coef[Kt]);

     //  realize=error*coef[Kp]+(error-pid->lasterror)*coef[Kd]+pid->errorsum;
        realize=error*coef[Kp]+(error-pid->lasterror)*coef[Kd];
       pid->lasterror=error;

       return realize;

}

int32 PID_Position_v(pid_s *pid,float *coef,int32 result,int32 point)
{
       int32 error,realize;

       error=point-result;

       pid->errorsum+=error*coef[Ki];

       pid->errorsum=XIANFU(pid->errorsum,-coef[Kt],coef[Kt]);

       realize=error*coef[Kp];

       pid->lasterror=error;

       return realize;

}
/*-----------------------------------------------------------------------------------------
* PID_Increase
* 增量式PID
---------------------------------------------------------------------------------------------*/

int32 PID_Increase(pid_s *pid,float *coef,int32 result,int32 point)
{
     int32 error,increase;

     error=point-result;

     increase=coef[Kp]*(error-pid->lasterror)+coef[Ki]*error+coef[Kd]*(error-2*pid->lasterror+pid->preerror);

     pid->preerror=pid->lasterror;

     pid->lasterror=error;

     return increase;

}
  //转向内环输出
  //static uint16 Count = 0;
void GyroOffset_init(void)      /////////陀螺仪零飘初始化
{
 static uint16 Count = 0;
  if(Count == 0)
  {
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
    /***********
                    1/22
        xdata:3225734349  3226879394
        ydata:3230373249  3229236593   
        zdata:1072416424  1072718414

       *********/
  }
  
  if(Count == 1000)
  {
    GyroOffset.Xdata /= 1000;
    GyroOffset.Ydata /= 1000;
    GyroOffset.Zdata /= 1000;
    IsGyroOffsetReset = 0;
    DataSave();
    Count = 0;
  }
  else
  {
    if(sensor==1)
    BMX055_DataRead(&sensor_data, 0);
    else if(sensor==0)
    FXAS2100_FXOS8700_DataRead(&sensor_data, 0);
    GyroOffset.Xdata += sensor_data.GYROXdata;
    GyroOffset.Ydata += sensor_data.GYROYdata;
    GyroOffset.Zdata += sensor_data.GYROZdata;
    Count++;
  }
}


void Angle_get (void)
{
  static _Bool IsAttitudeinit = 0;
   // i=0;
  if(IsGyroOffsetReset)
  {
    GyroOffset_init();             ///////零偏矫正过程中不进行姿态结算，并保证姿态模块静止
    return;
  }
    if(sensor==1)
    BMX055_DataRead(&sensor_data, 0);
    else if(sensor==0)
    FXAS2100_FXOS8700_DataRead(&sensor_data, 0);
  if(sensor==1)
  {
    sensor_data.GYROXdata = (sensor_data.GYROXdata - GyroOffset.Xdata) * 0.030517578;   
    sensor_data.GYROYdata = (sensor_data.GYROYdata - GyroOffset.Ydata) * 0.030517578;
    sensor_data.GYROZdata = (sensor_data.GYROZdata - GyroOffset.Zdata) * 0.030517578;
      ///////1000 / 32768     //////BMX055本身零飘几乎可以忽略不计，但是安全起见还是矫正一下
    sensor_data.ACCXdata *= 0.001953125;    ///////4 / 2048
    sensor_data.ACCYdata *= 0.001953125;
    sensor_data.ACCZdata *= 0.001953125;
  }
  else if(sensor==0)
  {
    sensor_data.GYROXdata = (sensor_data.GYROXdata - GyroOffset.Xdata) * 0.03060163;
    sensor_data.GYROYdata = (sensor_data.GYROYdata - GyroOffset.Ydata) * 0.03060163;
    sensor_data.GYROZdata = (sensor_data.GYROZdata - GyroOffset.Zdata) * 0.03060163;
    sensor_data.ACCXdata *= 0.00048828;
    sensor_data.ACCYdata *= 0.00048828;
    sensor_data.ACCZdata *= 0.00048828;
    sensor_data.ACCZdata = (sensor_data.ACCZdata + 0.95) / (0.95 + 1.05) * 2 - 1;
    sensor_data.ACCYdata = (sensor_data.ACCYdata + 1.04) / (0.96 + 1.04) * 2 - 1;
    sensor_data.ACCXdata = (sensor_data.ACCXdata + 0.97) / (0.97 + 1.03) * 2 - 1;
  }

  
  
  Acc.Xdata = sensor_data.ACCXdata;
  Acc.Ydata = sensor_data.ACCYdata;
  Acc.Zdata = sensor_data.ACCZdata;
  Gyro.Xdata = sensor_data.GYROXdata;
  Gyro.Ydata = sensor_data.GYROYdata;
  Gyro.Zdata = sensor_data.GYROZdata;

  if(IsAttitudeinit == 0)
  {
    Quaternion_init();                    ////姿态解算初始化        
    IsAttitudeinit = 1;
  }
  else
  {
    Attitude_UpdateGyro();                /////快速更新
    Attitude_UpdateAcc();                 /////深度融合更新
    
    SystemAttitude.Pitch = -EulerAngle.Roll / PI * 180;            ////转为角度   俯仰角
    SystemAttitude.Roll = EulerAngle.Pitch / PI * 180;             ////翻滚角
    SystemAttitude.Yaw = EulerAngle.Yaw / PI * 180;                ////偏航角
    SystemAttitudeRate.Pitch = -EulerAngleRate.Roll / PI * 180;   ////俯仰角速度
    SystemAttitudeRate.Yaw = EulerAngleRate.Yaw / PI * 180;       ////偏航角速度
    SystemAttitudeRate.Roll=EulerAngleRate.Pitch/PI*180;          ////横滚角速度
        /////为了更精准的获得俯仰角，再次进行互补滤波
    SystemAttitudeRate.Yaw=XIANFU(SystemAttitudeRate.Yaw,-310,310);
    
    float AccZ, AccZAdjust;    
    AccZ = -Acc.Zdata;
    if (AccZ > 1)
	AccZ = 1;
    if (AccZ < -1)
	AccZ = -1;    
    a_angle = asinf(AccZ) * 180 / PI;
    car_angle = SystemAttitude.Pitch;
    AccZAdjust = (a_angle - SystemAttitude.Pitch);//Error
    SystemAttitude.Pitch += (-Gyro.Xdata + AccZAdjust) * PERIODS;//所得当前角度
//    static float Yawoffset=0;
//    Yawoffset+=PERIODS;
//    SystemAttitude.Yaw=EulerAngle.Yaw / PI * 180+Yawoffset;
    

  /*****************************************************************/
  /*我是分界线，上面是四元数姿态解算*/
  /******************************************************************/
  }
//      car_diroutcore=PID_Position(&wx_pid,wx_coef,SysYaw_last*100,SystemAttitude.Yaw*100);
//      SysYaw_last=SystemAttitudeRate.Yaw;
}