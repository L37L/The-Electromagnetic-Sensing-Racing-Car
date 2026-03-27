#include "motor.h"
motor_param_t motor_l, motor_r;
pid_param_t motor_pid_l, motor_pid_r;

float L_error, R_error;
float diff = 0.16;
int16 goalspeed;
int16 speed_strait = 80;//135
int16 speed_ben = 70;//120
int16 huan_speed = 145;//145
int16 zhangai_speed = 0;
int16 podao_speed = 55;

void motor_pid_init(void)
{
	PID_CREATE(&motor_pid_l, 63, 36, 120, 1, MOTOR_PWM_DUTY_MAX, 500, MOTOR_PWM_DUTY_MAX);
	PID_CREATE(&motor_pid_r, 63, 36, 120, 1, MOTOR_PWM_DUTY_MAX, 500, MOTOR_PWM_DUTY_MAX);
//	PID_CREATE(&motor_pid_l, 0, 120, 0, 1, MOTOR_PWM_DUTY_MAX, MOTOR_PWM_DUTY_MAX, MOTOR_PWM_DUTY_MAX);
//	PID_CREATE(&motor_pid_r, 0, 120, 0, 1, MOTOR_PWM_DUTY_MAX, MOTOR_PWM_DUTY_MAX, MOTOR_PWM_DUTY_MAX);
}

void dif_motor_control(void)
{
	motor_l.exp_speed = goalspeed;
	motor_r.exp_speed = goalspeed;
	
	if((zone_flag == 1) && in_flag == 0 && (zhangai_state != 1))
	{
		if (change_duty > 0)
		{
			motor_l.exp_speed = motor_l.exp_speed * (float)(change_duty * (-0.005f) + 0.80f); // && space_flag!=1
			motor_r.exp_speed = (goalspeed + change_duty * 0.1);
		}
		if (change_duty < 0) 
		{
			motor_r.exp_speed = motor_r.exp_speed * (float)(change_duty * (0.005f) + 0.80f); 
			motor_l.exp_speed = (goalspeed + (-1 * change_duty * 0.1));

		}
	}
	if((zone_flag != 1) && (in_flag==0) && (zhangai_state != 1))
	{
		if (change_duty > 0)
		{
			motor_l.exp_speed = motor_l.exp_speed * (float)(change_duty * (-0.006f) + 0.80f); // && space_flag!=1
			motor_r.exp_speed = (goalspeed + change_duty * diff);
		}
		if (change_duty < 0) 
		{
			motor_r.exp_speed = motor_r.exp_speed * (float)(change_duty * (0.006f) + 0.80f); 
			motor_l.exp_speed = (goalspeed + (-1 * change_duty * diff));

		}
	}
	if(zhangai_state==1)
	{
		if (change_duty > 0)
		{
			motor_l.exp_speed = motor_l.exp_speed * (float)(change_duty * (-0.008f) + 0.75f); // && space_flag!=1
			motor_r.exp_speed = goalspeed + change_duty * 0.20;
		}
		if (change_duty < 0) 
		{
			motor_r.exp_speed = motor_r.exp_speed * (float)(change_duty * (0.008f) + 0.75f); 
			motor_l.exp_speed = goalspeed + change_duty * (-0.20);

		}
	}
	
	if (change_duty > 0 && (in_flag == 4 || in_flag == 3) && in_flag != 5)
	{
		motor_l.exp_speed = motor_l.exp_speed * (float)(change_duty * (-0.0075f) + 0.8f);//0.0072
        motor_r.exp_speed = goalspeed;
	}
	if (change_duty < 0 && (in_flag == 4 || in_flag == 3) && in_flag != 5)
	{
		motor_r.exp_speed = motor_r.exp_speed * (float)(change_duty * (0.0075f) + 0.8f);//0.0072
        motor_l.exp_speed = goalspeed;
	}
	
}

void motor_init(void)
{
	pwm_init(PWMA_CH1P_P60, 17000, 0);
	pwm_init(PWMA_CH2P_P62, 17000, 0);
	pwm_init(PWMA_CH3P_P64, 17000, 0);
	pwm_init(PWMA_CH4P_P66, 17000, 0);
}

void motor_control(void)
{
	
	if (ad0 <= 5 &&  ad1 <= 5 && ad3 <= 5 && ad2 <= 5 && ad4 <= 5 && in_flag == 0 && zhangai_flag == 0)
	{
		lost_flag = 1;
	}
	else
	{
		lost_flag = 0;
	}

	if (lost_flag == 0)
	{
		dif_motor_control();
//		motor_l.target_speed = motor_l.exp_speed;
//		motor_r.target_speed = motor_r.exp_speed;
	}

	if (lost_flag == 1) 
	{
		goalspeed = 0;
		motor_l.exp_speed = 0;
		motor_r.exp_speed = 0;
	}
	
//	if(motor_l.exp_speed > motor_l.encoder_speed)
//	{
//		motor_l.target_speed+=5;
//		if(motor_l.exp_speed - motor_l.encoder_speed < 5)
//		{
//			motor_l.target_speed = motor_l.exp_speed;
//		}
//	}
//	if(motor_r.exp_speed > motor_r.encoder_speed)
//	{
//		motor_r.target_speed+=5;
//		if(motor_r.exp_speed - motor_r.encoder_speed < 5)
//		{
//			motor_r.target_speed = motor_r.exp_speed;
//		}
//	}
//	if(motor_l.exp_speed < motor_l.encoder_speed)
//	{
//		motor_l.target_speed-=5;
//		if(motor_l.encoder_speed - motor_l.exp_speed < 5)
//		{
//			motor_l.target_speed = motor_l.exp_speed;
//		}
//	}
//	if(motor_r.exp_speed < motor_r.encoder_speed)
//	{
//		motor_r.target_speed-=5;
//		if(motor_r.encoder_speed - motor_r.exp_speed < 5)
//		{
//			motor_r.target_speed = motor_r.exp_speed;
//		}
//	}
	L_error = (motor_l.exp_speed - motor_l.encoder_speed);
	R_error = (motor_r.exp_speed - motor_r.encoder_speed);
	
	motor_l.duty = pid_solve(&motor_pid_l, L_error);
	motor_r.duty = pid_solve(&motor_pid_r, R_error);
//	motor_l.duty = increment_pid_solve(&motor_pid_l, L_error);
//	motor_r.duty = increment_pid_solve(&motor_pid_r, R_error);

	//电机pwm限幅
	if(motor_l.duty>6000)motor_l.duty = 6000;
	if(motor_l.duty < -6000) motor_l.duty = -6000;
	if(motor_r.duty>6000)motor_r.duty = 6000;
	if(motor_r.duty < -6000) motor_r.duty = -6000;
	
	//控制左边电机正反转
	if(start_flag == 1)
	{
		pwm_duty(PWMA_CH1P_P60, (motor_l.duty >= 0) ? motor_l.duty : 0);
		pwm_duty(PWMA_CH2P_P62, (motor_l.duty >= 0) ? 0 : (-motor_l.duty));
		//控制右边电机正反转
		pwm_duty(PWMA_CH3P_P64, (motor_r.duty >= 0) ? motor_r.duty : 0);
		pwm_duty(PWMA_CH4P_P66, (motor_r.duty >= 0) ? 0 : (-motor_r.duty));
	}
}



	


