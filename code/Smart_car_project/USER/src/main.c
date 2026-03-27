/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"
#include "motor.h"
#include "servomotor.h"
#include "adc.h"
#include "encoder.h"
#include "stdio.h"
#include "element.h"
#include "isr.h"
#include "ui.h"
#include "SEEKFREE_DL1A.h"
#include "key.h"
#include "wireless.h"
void main()
{   
	DisableGlobalIRQ();		//关闭总中断
	
    //sys_clk可选值:35000000，30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600。
    //设置系统频率，此频率需要跟STC-ISP软件中的 <输入用户程序运行的IRC频率>选项的频率一致。
    //如果频率设置不对，将会导致串口的数据不正常,PWM的工作不正常等等。
    sys_clk = 35000000;     //设置系统频率为35MHz
	
	board_init();													// 初始化寄存器,勿删除此句代码。
	Adinductance_init();									//电感初始化
	
	motor_init();													//电机初始化
	motor_pid_init();											//电机PID初始化
	
	encoder_init();												//编码器初始化

	smotor_pin_init();										//舵机初始化
	
	UI_Init();														
	sensor_init();												//TOF,陀螺仪初始化
//	wireless_uart_init(); 							//无线串口初始化

	pit_timer_ms(TIM_1, 5);								//5ms中断开启
	
	EnableGlobalIRQ();
	
	out_flag = 1;													//出库状态默认为未出库	
	start_flag = 1;												//电机使能初始为0，按键触发使能
	
  while(1)
	{
//		out_storage();											//出库
		if(out_flag == 1)										//完成出库，执行循迹
		{
			PROCESS();
		}
//		if(out_flag == 2)										//入库（霍尔判断）
//		{
//			In_ku();
//		}
		if(Shut_flag == 0)									//标志位置1后关闭屏幕
		{
			UI_Main();
		}

//上位机
//		JustFloat((float)motor_l.encoder_speed,(float)motor_l.exp_speed);
//		JustFloat((float)ad2,(float)zone_flag);

	}
			
}

