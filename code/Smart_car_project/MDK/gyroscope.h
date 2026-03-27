#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "SEEKFREE_IMU660RA.h"
#include "math.h"
#include "headfile.h"
#include "element.h"

extern float Yaw;
extern int16 Pitch,roll;
extern float GZ;
extern float Angle_pitch;
void gyroscope_get(); 
void gyroscope_open();
void Angle_Add();
void Angle_Get();

#endif
