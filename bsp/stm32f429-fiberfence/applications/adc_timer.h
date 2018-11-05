#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
struct ADC_data{
	u16 ac1[1000];
	u16 dc1[100];
	u16 ac2[1000];
	u16 dc2[100];
};

extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 
extern TIM_HandleTypeDef TIM4_Handler;      //��ʱ����� 

void TIM3_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);

void start_the_time(void);
int stop_the_time(void);
#endif

