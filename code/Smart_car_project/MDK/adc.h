#ifndef _ADC_H_
#define _ADC_H_

#include "headfile.h"
#include "pid.h"
#include "servomotor.h"
#include "math.h"


extern void Adinductance_init(void);
extern void read_AD(void);
extern void Get_MAX(void);
extern float ad0,ad1,ad2,ad3,ad4;	//归一化后值
extern float RAW[5];//原始值
extern float MAX[5];//实际情况改值，实际采集的最大值




#endif /* _ADC_H_ */

