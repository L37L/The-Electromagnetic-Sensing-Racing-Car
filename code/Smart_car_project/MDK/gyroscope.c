#include "gyroscope.h"
#define PI 3.14159265

//陀螺仪内置三轴角速度计和三轴加速度计


float dt = 0.005f;     //中断周期
float Filter_Weight = 0.0035f;    //滤波权重，值越大表加速度计的可信度越高
float Angle_gz = 0, Angle_gz1 = 0;
float Angle_pitch = 0;
float pit = 0, pittch = 0; // pitch:俯仰角  yaw:偏航角   融合后的角度
float acc_x = 0, acc_y = 0, acc_z = 0, acc_x_last = 0, acc_y_last = 0, acc_z_last = 0; //加速度计采集到的原始数据换算单位后
float alpha = 0.4;
float gyro_x = 0, gyro_y = 0, gyro_z = 0; //角速度计采集到的原始数据换算单位后
int16 Pitch = 0;  
float GX = 0, GY = -1, GZ = 0;                            //角速度计零漂
int16 AX = 553, AY = -150, AZ = 4148;    
float Yaw = 0;                              //最终角度


void gyroscope_get()  //陀螺仪采集回的原始数据处理
{
	   
    if (imu660ra_acc_x < 32764)
        acc_x = imu660ra_acc_x / 4096.0; //计算x轴加速度
    else
        acc_x = 1 - (imu660ra_acc_x - 49152) / 4096.0;
    if (imu660ra_acc_y < 32764)
        acc_y = imu660ra_acc_y / 4096.0; //计算y轴加速度
    else
        acc_y = 1 - (imu660ra_acc_y - 49152) / 4096.0;
    if (imu660ra_acc_z < 32764)
        acc_z = imu660ra_acc_z / 4096.0; //计算z轴加速度
    else
        acc_z = (imu660ra_acc_z - 49152) / 4096.0;
	
    gyro_x = (float)(imu660ra_gyro_x / 16.4);
    gyro_y = (float)(imu660ra_gyro_y / 16.4);
    imu660ra_gyro_z /= 16.4;
}

void Angle_add()
{
    gyroscope_get();
	Angle_pitch = (float)(atan(acc_y / acc_z))*57.324;
    pit = (1 - Filter_Weight) * (pit + (gyro_x * dt)) + Angle_pitch * Filter_Weight; //一阶互补滤波公式
	Angle_gz = Angle_gz + ((float)imu660ra_gyro_z - GZ) * 0.005;	//角速度积分得到倾斜角度,越大积分出来的角度越大
}

void Angle_Get()
{
	Angle_add();
    //if (gyroscope_time > 600)   //上电3s后才获取俯仰角
    //{
        Pitch = (int16)pit;
    //}
	Yaw = (int16)Angle_gz;
}


void gyroscope_open()
{
  imu660ra_get_acc ();                                     // 获取 IMU660RA 加速度计数据
  imu660ra_get_gyro ();
  Angle_Get();
	
}