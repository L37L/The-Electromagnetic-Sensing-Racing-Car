#include "element.h"
//出入库
float chu_kp = 7.5, chu_kd = 6.5;				//出库舵机PID参数
int8 out_flag;													//行进状态
int8 in_flag = 0;												//入库状态
int16 cnt = 0;													//入库计时位
float ruku_dis = 0;											//入库距离记录
int8 number = 0;												//霍尔检测标志位
int16 ruku_time = 105;									//倒车时间阈值

//弯直道
int8 zone_flag = 0;											//弯直道标志位
float kp_str = 2.0, kd_str = 4.0;

//坡道
int8 podao_flag = 0; 										//坡道标志位
uint32 podao_time = 0;									//记录坡道时间
int8 po_finish = 0;											//坡道完成标志

//丟线
int8 lost_flag = 0;

//障碍
int8 zhangai_flag = 0;									//障碍标志位	
int8 zhangai_state = 0;									//障碍状态
float zhangai_dis = 0;
int8 zhangai_finish = 0;
int16 Dlla_control[4] = {1, 0, 0, 0};			//坡道和避障区分，大于0为避障，小于0为坡道，0不做处理
int8 control_point = 0;

//环岛
int8 lhuandao_flag = 0;
int8 rhuandao_flag = 0;
int8 huandao_num = 0;										//记录过环岛次数
uint32 huandao_time = 0;								//记录环岛时间
huandao_type_e huandao_state = NONE;		//环岛状态机
float huan_kp=1.5, huan_kd=1.0;					//环内循迹PID参数

//循迹
float s_error;
float last_error, Difference_error;
float backup, error_heng, error_xie;			//横电感差值，横电感差比和，斜电感差比和
float abserror, ratio;
int16 a=55, b=200, Limit=100, 		ad_referens = 54;//直道中间电感值


int16 change_duty = 0;
int16 angle_z = 0;
int16 angle_y = 0;

int8 rank = 0;		//圈数

/*********************************************
*函数：传感器初始化函数
*函数名：sensor_init()
*备注：
*********************************************/
void sensor_init(void)
{
//	imu660ra_init();	//陀螺仪初始化
//	dl1a_init();
}

/*********************************************
*函数：元素处理函数
*函数名：element_deal()
*备注：
*********************************************/

void element_deal(void)
{
	abserror = abs(ad_referens - ad2);					//中间电感与在直道时的偏差
  ratio = (abserror / ad_referens) * a;				
	Difference_error = s_error - last_error;
	if(zone_flag == 1)							//直道（差比和差）
	{
		goalspeed = speed_strait;
		last_error = s_error;
		s_error = (float)(b * (sqrt(ad4) - sqrt(ad0))) / (ad4 + ad0 + ad2);
		smotor_pid.kp = kp_str; smotor_pid.kd = kd_str;
		change_duty = pid_solve(&smotor_pid, s_error);		
	}
	if(zone_flag == 2 /*|| zone_flag ==1*/)						//弯道（三电感循迹 + 模糊PID）
	{	
		if (error_heng < -0.23 && error_xie < -0.47)     //右弯
    {
			goalspeed = speed_ben;
			last_error = s_error;
      s_error = -1* ratio;
			smotor_pid.kp = Fuzzy_P(s_error, Difference_error);
      smotor_pid.kd = Fuzzy_D(s_error, Difference_error);
			change_duty = pid_solve(&smotor_pid, s_error);
    }
    if (error_heng > 0.23 && error_xie > 0.47)      //左弯
    {
      goalspeed = speed_ben;
			last_error = s_error;
      s_error = ratio;
			smotor_pid.kp = Fuzzy_P(s_error, Difference_error);
      smotor_pid.kd = Fuzzy_D(s_error, Difference_error);
			change_duty = pid_solve(&smotor_pid, s_error);
		}
	}
	
	
	if(lhuandao_flag == 1)
	{
		lhuandao_deal();
	}
	
	if(rhuandao_flag == 1)
	{
		rhuandao_deal();
	}
	
//	if(podao_flag == 1)
//	{	
//		goalspeed = speed_slope;
//		if((podao_time >= 370))
//		{
//			podao_flag = 0;
//			po_finish++;
//			control_point++;
//		}
//	}
	if(zhangai_flag == 1)
	{
		zhangai_deal();
	}
}

/*********************************************
*函数：障碍识别函数
*函数名：element_judge()
*备注：
*********************************************/
void zhangai(void)
{
	if(zhangai_state == 0 && dl1a_distance_mm <= 550 && podao_flag!=1 && (zone_flag == 1) && (Dlla_control[control_point] == 1))			//障碍
	{
		zhangai_flag = 1;
		zhangai_state = 1;
		zone_flag = 4;
		zhangai_dis = dis;
	}
}

/*********************************************
*函数：环岛识别函数
*函数名：环岛_judge()
*备注：
*********************************************/
void huandao(void)
{
	if(huandao_state == NONE && ad2>=67 && ad1>= 65 && ad4>=90 && podao_flag==0)
	{
		{
			lhuandao_flag = 1;
			huandao_state = PRE_ENROLL;
			huandao_time = 0;
		}
	}
	
	if(huandao_state == NONE && ad2>=67 && ad3>= 65 && ad0>=90 && podao_flag==0)
	{
		{
			rhuandao_flag = 1;
			huandao_state = PRE_ENROLL;
			huandao_time = 0;
		}
	}
}
/*********************************************
*函数：弯直道识别函数
*函数名：zw_judge()
*备注：
*********************************************/
void zw_judge(void)
{
	if(lhuandao_flag !=1 && zhangai_flag !=1 && rhuandao_flag !=1  && (((error_heng <= 0.23 || error_heng >= -0.23) && (error_xie <= 0.47 || error_xie >= -0.47)) || ad2 >= (50)/*ad_referens *0.90*/))
	{
		zone_flag = 1;		//直道
	}
	if(lhuandao_flag !=1 && zhangai_flag !=1 && rhuandao_flag !=1  && ad2 <= (50)/*ad_referens *0.90*/ && (error_heng > 0.23 || error_heng < -0.23) && (error_xie > 0.47 || error_xie < -0.47))

	{
		zone_flag = 2;		//弯道
	}
}

/*********************************************
*函数：执行函数
*函数名：PROCESS()
*备注：
*********************************************/
void PROCESS(void)
{
	error_heng = (ad4 - ad0) / (ad4 + ad0);
	error_xie =  (ad3 - ad1) / (ad3 + ad1);
	zw_judge();
//	huandao();
//	Po();
//	zhangai();
	servo_control();
}

/*********************************************
 *函数：坡道识别函数
 *函数名：
 *备注：
 *********************************************/
void Po(void)
{
	if(podao_flag==0  && lhuandao_flag == 0 && rhuandao_flag == 0 && dl1a_distance_mm <= 350 && (Dlla_control[control_point] == -1))
	{
			podao_flag = 1;
			podao_time = 0;
  }
    
}

/*********************************************
*函数：避障执行函数
*函数名：zhangai_deal()
*备注：
*********************************************/
void zhangai_deal(void)
{
	if(zhangai_state == 1)
	{
		goalspeed = speed_strait;
		change_duty = 80;
		if(dis - zhangai_dis > 0.47)
		{
			zhangai_state = 2;
		}
	}
	if(zhangai_state == 2)
	{
		goalspeed = speed_ben;
		change_duty = 0;
		if((dis - zhangai_dis) > 0.56)//0.78
		{
			change_duty = -80;
			if((dis - zhangai_dis) > 1.14)
			{
				change_duty = 0;
				if(ad2>=0.7)
				{
					zhangai_flag = 0;
					zhangai_state = 0;
					zhangai_finish++;
					control_point++;
					zone_flag = 2;
				}
			}		
		}
	}
}

/*********************************************
*函数：左环岛处理
*函数名：lhuandao_deal()
*备注：
*********************************************/
void lhuandao_deal(void)
{
	if(huandao_state == PRE_ENROLL)
	{
		goalspeed = speed_strait * 0.75;		//预入环减速
	}
	if(huandao_state == PRE_ENROLL && (ad2 >=70 && ad2<= 90) && ad3>=85 && ad3>=ad1)//ad2<88
	{
		zone_flag = 3;
		huandao_state = ENROLL;
		angle_z = Yaw;		
	}
	if(huandao_state == ENROLL)
	{
		change_duty = 73;			//入环状态，控制舵机固定打角
		
		if((Yaw - angle_z) >= 40 || ad2 <= ad_referens*0.67)//如角度累计大于阈值则进入环内状态或是中间电感低于阈值
		{
			huandao_state = INSIDER;
		}
	}
	if(huandao_state == INSIDER)
	{
		goalspeed = huan_speed;
		smotor_pid.kp = huan_kp; smotor_pid.kd = huan_kd;//2.0
		s_error = ratio;
		change_duty = pid_solve(&smotor_pid, s_error);
		if((Yaw-angle_z) >= 265 || ad2 >= ad_referens*0.55)			//如角度累计大于阈值则进入环内状态或是中间电感高于阈值
		{
			huandao_state = PRE_OUT;
		}
	}
	if(huandao_state == PRE_OUT)
	{
		change_duty = 62;			//预出环状态，控制舵机固定打角		
		if((Yaw-angle_z) >= 320 || ad2 >= ad_referens*0.93)			//基本出环
		{
			huandao_state = OUT;
		}
	}
	if(huandao_state == OUT)
	{
		goalspeed = speed_ben;
		last_error = s_error;
		s_error = (float)(b * (sqrt(ad4) - sqrt(ad0))) / (ad4 + ad0);
		smotor_pid.kp = 5.5; smotor_pid.kd = 4.5;
		change_duty = pid_solve(&smotor_pid, s_error);
		if(huandao_time >= 150)																	//定时消环，退出环岛状态
		{
			angle_z = 0;						//记录角至0
			huandao_state = NONE;
			huandao_num++;
			lhuandao_flag = 0;
		}
	}		
}


/*********************************************
*函数：右环岛处理
*函数名：lhuandao_deal()
*备注：
*********************************************/
void rhuandao_deal(void)
{
	if(huandao_state == PRE_ENROLL)
	{
		goalspeed = speed_strait * 0.75;			//预入环减速
	}
	if(huandao_state == PRE_ENROLL && (ad2 >=70 && ad2<= 87) && ad1>=85 && ad1>=ad3)//ad2<87
	{
		zone_flag = 5;
		huandao_state = ENROLL;
		angle_z = Yaw;
	}
	if(huandao_state == ENROLL)
	{
		change_duty = -72;			//入环状态，控制舵机固定打角

		if((Yaw - angle_z) <= -40)			//如角度累计大于阈值则进入环内状态
		{
			huandao_state = INSIDER;
		}
	}
	
	if(huandao_state == INSIDER)
	{
		goalspeed = huan_speed;
		smotor_pid.kp = huan_kp; smotor_pid.kd = huan_kd;
		s_error = -1 * ratio;
		change_duty = pid_solve(&smotor_pid, s_error);
		if((Yaw-angle_z) <= -270)			//角度累计大于阈值进入预出环
		{
			huandao_state = PRE_OUT;
		}
	}
	if(huandao_state == PRE_OUT)
	{
		change_duty = -64;			//预出环状态，控制舵机固定打角		
		if((Yaw-angle_z) <= -320)			//基本出环
		{
			huandao_state = OUT;
		}
	}
	if(huandao_state == OUT)
	{
		goalspeed = speed_ben;
		last_error = s_error;
		s_error = (float)(b * (sqrt(ad4) - sqrt(ad0))) / (ad4 + ad0);
		smotor_pid.kp = 5.0; smotor_pid.kd = 4.5;
		change_duty = pid_solve(&smotor_pid, s_error);
		if(huandao_time >= 150)					//定时消环，退出环岛状态
		{
			angle_z = 0;						//记录角至0
			huandao_state = NONE;
			huandao_num++;
			rhuandao_flag = 0;
		}

	}	
}
/*********************************************
*函数：出入库函数
*函数名：out_storage()
*备注：
*********************************************/
void out_storage(void)
{
	if(out_flag == 0)
	{	
		goalspeed = 100;
		last_error = s_error;
		s_error = (float)(b * (sqrt(ad4) - sqrt(ad0))) / (ad4 + ad0);
		smotor_pid.kp = chu_kp; smotor_pid.kd = chu_kd;
		change_duty = pid_solve(&smotor_pid, s_error);
		if(change_duty >= 80)
		{
			change_duty = 80;
		}	
		if(change_duty <= -80)
		{
			change_duty = -80;
		}
		Duty_result = (float)(change_duty + SERVO_MID);
		if(Duty_result >= 900)
			Duty_result = 900;
		if(Duty_result <= 740)
			Duty_result = 740;
		pwm_duty(SMOTOR_PIN, Duty_result);
		if(dis > 0.7)						//距离大于阈值，结束出库状态
		{
			out_flag = 1;
		}
	}
}

void In_ku(void)
{

    if (out_flag == 2 && in_flag == 0)	 					//继续循迹前进
    {
        ruku_dis = dis;
        in_flag = 1;
				goalspeed = 0 ;
				last_error = s_error;
				s_error = (float)(b * (sqrt(ad4) - sqrt(ad0))) / (ad4 + ad0);
				smotor_pid.kp = 1.8; smotor_pid.kd = 3.5;
				change_duty = pid_solve(&smotor_pid, s_error);
				if(change_duty >= 80)
				{
					change_duty = 80;
				}	
				if(change_duty <= -80)
				{
					change_duty = -80;
				}
				Duty_result = (float)(change_duty + SERVO_MID);
				if(Duty_result >= 900)
					Duty_result = 900;
				if(Duty_result <= 740)
					Duty_result = 740;		
				pwm_duty(SMOTOR_PIN, SERVO_MID);
    }
		
    if (in_flag == 1)														//前进一定距离后开始倒车
    {
        if (dis - ruku_dis > 0.25)
        {
						goalspeed = -130;
            ruku_dis = dis;
            in_flag = 2;
        }
    }
		
    if (in_flag == 2)														//计时倒车
    {
				last_error = s_error;
				s_error = (float)(b * (sqrt(ad4) - sqrt(ad0))) / (ad4 + ad0);
				smotor_pid.kp = 1.8; smotor_pid.kd = 3.5;
				change_duty = pid_solve(&smotor_pid, s_error);
				Duty_result = (float)(change_duty + SERVO_MID);
				pwm_duty(SMOTOR_PIN, Duty_result);
        if (cnt > ruku_time)
        {
            cnt = 0;
            in_flag = 3;
        }
    }
		
    if (in_flag == 3)														//打角进库
    {		
				if(sw_state1 == 1 && sw_state2 == 1)				//用拨码开关调整入库方向
				{
					change_duty = (-80);			
				}
				else if(sw_state1 == 0 && sw_state2 == 1)
				{
					change_duty = (80);		
				}
				Duty_result = (float)(change_duty + SERVO_MID);
				pwm_duty(SMOTOR_PIN, Duty_result);
				goalspeed = -140;
        angle_z = Yaw;
				ruku_dis = dis;
        in_flag = 4;
    }
		
    if ((abs(Yaw - angle_z) >= 65 || dis - ruku_dis <= -0.55) && in_flag == 4)		//直行
//	if (dis - ruku_dis <=-0.73 && in_flag == 4)		//直行

    {
		Duty_result = SERVO_MID;
		pwm_duty(SMOTOR_PIN, Duty_result);
		ruku_dis = dis;
        in_flag = 5;
    }
    if (in_flag == 5)		//停车
    {
		goalspeed = -130;
		if((dis - ruku_dis) < -0.26)
		{
			goalspeed = 35;
			delay_ms(100);
			in_flag = 6;
		}
    }
	if(in_flag==6)
	{
		goalspeed = 0;
		motor_l.exp_speed = 0;
		motor_r.exp_speed = 0;
	}
}


///*********************************************
//*函数：霍尔检测
//*函数名：huoer()
//*备注：
//*********************************************/
void huoer(void)
{
	if (HALL_PIN == 0 && number < 24 && out_flag == 1)
	{												 
		number += 1;
		if (number >= 24)
		{
			number = 0;
		}
	}
	if(rank == 0)
	{
		if (number == 1)
		{
			out_flag = 2;
		}
	}
	if(rank == 1)
	{
		if(number == 1)
		{
			control_point = 0;
		}
		if(number == 2)
		{
			out_flag = 2;
		}
	}
}
