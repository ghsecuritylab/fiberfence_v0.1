#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//KEY��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


//����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
#define KEY0        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_10)  //KEY0����GPIOD10
#define KEY1        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_11)  //KEY1����GPIOD11
#define KEY2        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12)  //KEY2����GPIOD12
#define KEY3        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13)  //KEY2����GPIOD13
#define KEY4        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_14)  //KEY2����GPIOD14
#define KEY5        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15)  //KEY2����GPIOD15

#define KEY0_PRES 	1
#define KEY1_PRES		2
#define KEY2_PRES		3
#define KEY3_PRES   4
#define KEY4_PRES   5
#define KEY5_PRES   6

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif
