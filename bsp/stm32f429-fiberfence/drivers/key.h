#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//KEY驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


//下面的方式是通过直接操作HAL库函数方式读取IO
#define KEY0        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_10)  //KEY0按键GPIOD10
#define KEY1        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_11)  //KEY1按键GPIOD11
#define KEY2        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12)  //KEY2按键GPIOD12
#define KEY3        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13)  //KEY2按键GPIOD13
#define KEY4        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_14)  //KEY2按键GPIOD14
#define KEY5        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15)  //KEY2按键GPIOD15

#define KEY0_PRES 	1
#define KEY1_PRES		2
#define KEY2_PRES		3
#define KEY3_PRES   4
#define KEY4_PRES   5
#define KEY5_PRES   6

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif
