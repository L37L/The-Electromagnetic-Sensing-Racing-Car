#include "encoder.h"

#define PI 3.14159265

float low_pass_encoder =  0.3;
float dis = 0;		//路程

void encoder_init(void)
{
    ctimer_count_init(SPEEDL_PLUSE);//定时编码器引脚初始化 (两轮)
    ctimer_count_init(SPEEDR_PLUSE);
	motor_l.encoder_raw = 0; motor_r.encoder_raw = 0;
	motor_l.total_encoder = 0; motor_r.total_encoder = 0;
}

void Encoder_read(void)
{
	motor_l.encoder_raw = ctimer_count_read(SPEEDL_PLUSE) / 2.;     // 定时器取值
	motor_r.encoder_raw = ctimer_count_read(SPEEDR_PLUSE) / 2.;    // 定时器取值
	if(SPEEDL_DIR == 0)		//反转
	{
		motor_l.encoder_raw = -1 * motor_l.encoder_raw;
	}
	if(SPEEDR_DIR == 1)
	{
		motor_r.encoder_raw = -1 * motor_r.encoder_raw;
	}
	//后一值*低通系数 + 前一值*（1-低通系数）
    motor_l.encoder_speed = motor_l.encoder_speed * low_pass_encoder + motor_l.encoder_raw * (1-low_pass_encoder);//低通滤波
    motor_r.encoder_speed = motor_r.encoder_speed * low_pass_encoder + motor_r.encoder_raw * (1-low_pass_encoder);

	motor_l.total_encoder += (motor_l.encoder_raw * 4 * (PI) * 0.032) / 2250;
	motor_r.total_encoder += (motor_r.encoder_raw * 4 * (PI) * 0.032) / 2250;
//	R_dis = L_dis;
	ctimer_count_clean(SPEEDL_PLUSE);                // 定时器清空
    ctimer_count_clean(SPEEDR_PLUSE);                // 定时器清空

}

float get_total_encoder(void)
{

	return (float)((motor_l.total_encoder + motor_r.total_encoder) / 2);
	
}


