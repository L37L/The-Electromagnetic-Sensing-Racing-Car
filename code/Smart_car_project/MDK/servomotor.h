#ifndef _SERVOMOTOR_H_
#define _SERVOMOTOR_H_

#include "headfile.h"
#include "pid.h"
#include "adc.h"
#include "isr.h"
#include "element.h"

#define SMOTOR_PIN	PWMB_CH1_P74
#define SERVO_MID	820	
#define SERVO_R		730	
#define SERVO_L		910	


extern pid_param_t smotor_pid;
extern int16 Duty_result;

extern void servo_control(void);
extern void smotor_pin_init(void);
//extern void pid_param_init_S(void);	//Œª÷√ ΩPID
extern float Fuzzy_P(float E, float EC);
extern float Fuzzy_D(float E, float EC);



#endif /* CODE_SERVOMOTOR_H_ */