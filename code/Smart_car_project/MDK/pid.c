#include "pid.h"


void PID_CREATE(pid_param_t *pid, float kp_p, float ki_p, float kd_p, float low_pass_p, int16 max_p, int16 max_i, int16 max_d)
{
	pid->kp = kp_p;
	pid->ki = ki_p;
	pid->kd = kd_p;
	pid->low_pass = low_pass_p;
	pid->out_p = 0;
	pid->out_i = 0;
	pid->out_d = 0;
	pid->p_max = max_p;
	pid->i_max = max_i;
	pid->d_max = max_d;
}
		
	
// 常规PID
int16 pid_solve(pid_param_t *pid, float error) 
{
    pid->out_d = (error - pid->out_p) * pid->low_pass + pid->out_d * (1 - pid->low_pass); //低通滤波
    pid->out_p = error;
    pid->out_i += error;

    if (pid->ki != 0) 
		pid->out_i = MINMAX(pid->out_i, -pid->i_max / pid->ki, pid->i_max / pid->ki);
	
    return (pid->kp * pid->out_p + pid->ki * pid->out_i + pid->kd * pid->out_d);
}


// 增量式PID
int16 increment_pid_solve(pid_param_t *pid, float error) {
    pid->out_d = MINMAX(pid->kd * (error - 2 * pid->pre_error + pid->pre_pre_error), -pid->d_max, pid->d_max);
    pid->out_p = MINMAX(pid->kp * (error - pid->pre_error), -pid->p_max, pid->p_max);
    pid->out_i = MINMAX(pid->ki * error, -pid->i_max, pid->i_max);

    pid->pre_pre_error = pid->pre_error;
    pid->pre_error = error;

    return (pid->out_p + pid->out_i + pid->out_d);
}


