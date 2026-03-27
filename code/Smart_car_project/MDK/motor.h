#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "headfile.h"
#include "encoder.h"
#include "element.h"
#include "pid.h"
#include "servomotor.h"
#include "adc.h"

#define MOTOR1_A   PWMA_CH1P_P60   //定义1电机正转PWM引脚
#define MOTOR1_B   PWMA_CH2P_P62    //定义1电机反转PWM引脚

#define MOTOR2_A   PWMA_CH3P_P64   //定义2电机反转PWM引脚
#define MOTOR2_B   PWMA_CH4P_P66    //定义2电机正转PWM引脚

#define MOTOR_PWM_DUTY_MAX 10000

typedef struct motor_param_t 
{
    float total_encoder;
    float encoder_raw; //原始值
    int16 encoder_speed; //Measured speed（滤波后的值）
    int16 target_speed;
	int16 exp_speed;
    int16 duty;         //Motor PWM duty
} motor_param_t;

extern void motor_init(void);

extern void motor_control(void);
extern void motor_pid_init(void);
extern motor_param_t motor_l, motor_r;
extern pid_param_t  motor_pid_l, motor_pid_r;
extern int16 goalspeed, speed_strait, speed_ben, huan_speed, zhangai_speed, podao_speed;
extern float diff;

#endif /* CODE_MOTOR_H_ */

