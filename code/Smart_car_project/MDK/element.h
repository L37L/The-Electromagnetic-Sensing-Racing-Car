#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include "headfile.h"
#include "adc.h"
#include "motor.h"
#include "servomotor.h"
#include "SEEKFREE_ICM20602.h"
#include "isr.h"
#include "math.h"
#include "key.h"
#include "SEEKFREE_DL1A.h"
#include "SEEKFREE_DL1B.h"
#include "pid.h"
#include "encoder.h"
#include "gyroscope.h"


//霍尔
#define HALL_PIN P26

typedef enum		
{
	NONE = 0,
	PRE_ENROLL,				//预入环
	ENROLL,					//入环
	INSIDER,				//环内
	PRE_OUT,				//预出环
	OUT,					//出环
}huandao_type_e;

//标志位
extern int8 podao_flag;
extern int8 lhuandao_flag, rhuandao_flag;
extern int8 zhangai_flag;
extern int8 rank;		//圈数


//出入库
extern int8 in_flag;
extern int8 chuku_flag;
extern int8 out_flag;
extern int16 ruku_time;
extern int16 cnt;
extern float ruku_dis;
extern float chu_kp, chu_kd;
extern int8 number;

//丟线
extern int8 lost_flag;

extern int16 angle_z;				//角度观测值

//循迹
extern int16 change_duty;
extern int8 zone_flag;
extern float kp_ben, kd_ben;		//弯道PD
extern float huan_kp, huan_kd;

//偏差
extern float s_error;
extern float error_heng, error_xie;
extern int16 a,b, Limit, ad_referens;

extern uint32 podao_time;		//坡道时间
extern uint32 huandao_time;		//环岛时间
extern huandao_type_e huandao_state;
extern int8 lhuandao_gyro, rhuandao_gyro;

extern int8 zhangai_state;
extern int16 Dlla_control[4];	//大于0为避障，小于0为坡道，0不做处理
extern uint16 horizon;


extern void element_deal(void);
extern void zhangai(void);
extern void huandao(void);
extern  void Po(void);
extern void sensor_init(void);
extern void PROCESS(void);
extern void lhuandao_deal(void);
extern void rhuandao_deal(void);
extern void zhangai_deal(void);
extern void out_storage(void);
extern void huoer(void);
extern void In_ku(void);


#endif /* _ELEMENT_H_ */