#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
struct ADC_data{
	u16 ac1[1000];
	u16 dc1[100];
	u16 ac2[1000];
	u16 dc2[100];
};

extern TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 
extern TIM_HandleTypeDef TIM4_Handler;      //定时器句柄 

void TIM3_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);

void start_the_time(void);
int stop_the_time(void);
#endif

