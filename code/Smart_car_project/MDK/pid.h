#ifndef _PID_1_H_
#define _PID_1_H_



#include <headfile.h>
//#include "encoder.h"


#define MIN_P(a, b) (((a) < (b)) ? (a) : (b))
#define MAX_P(a, b) (((a) > (b)) ? (a) : (b))
#define MINMAX(input, low, upper)  MIN_P(MAX_P(input, low), upper)

//创建PID结构体
typedef struct {
    float kp;    //P
    float ki;    //I
    float kd;    //D
    int16 i_max; //integrator_max
    int16 p_max; //integrator_max
    int16 d_max; //integrator_max

    float low_pass;	//滤波系数

    int16 out_p;
    int16 out_i;
    int16 out_d;

    float error;
    float pre_error;
    float pre_pre_error;
} pid_param_t;

void PID_CREATE(pid_param_t *pid, float kp_p, float ki_p, float kd_p, float low_pass_p, int16 max_p, int16 max_i, int16 max_d);
extern int16 pid_solve(pid_param_t *pid, float error);
extern int16 increment_pid_solve(pid_param_t *pid, float error);

#endif /* CODE_PID_1_H_ */