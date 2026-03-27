#ifndef CODE_ENCODER_H_
#define CODE_ENCODER_H_
#include "headfile.h"
#include "motor.h"
#include "element.h"


//定义脉冲引脚
#define SPEEDL_PLUSE   CTIM3_P04		//左
#define SPEEDL_DIR		P53				//1为正
#define SPEEDR_PLUSE   CTIM0_P34		//右
#define SPEEDR_DIR		P35


extern float dis;		//路程

extern void encoder_init(void);

extern float get_total_encoder(void);     //得到车走过的距离

extern void Encoder_read(void);
#endif /* CODE_ENCODER_H_ */