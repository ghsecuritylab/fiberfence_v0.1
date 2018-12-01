#include "key.h"
#include "delay.h"
#include "rtthread.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//KEY��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//������ʼ������
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOD_CLK_ENABLE();                  //����GPIOAʱ��
    
    GPIO_Initure.Pin =GPIO_PIN_10|GPIO_PIN_11|\
											GPIO_PIN_12|GPIO_PIN_13|\
											GPIO_PIN_14|GPIO_PIN_15;    //PD10-15
    GPIO_Initure.Mode=GPIO_MODE_INPUT;             //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;               //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;            //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>....
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
    if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0))
    {
        //delay_ms(10);
				rt_thread_delay(100);
        key_up=0;
        if(KEY0==0)       return KEY0_PRES;
        else if(KEY1==0)  return KEY1_PRES;
        else if(KEY2==0)  return KEY2_PRES;
				else if(KEY3==0)  return KEY3_PRES;
				else if(KEY4==0)  return KEY4_PRES;
				else if(KEY5==0)  return KEY5_PRES;          
    }else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1)key_up=1;
    return 0;   //�ް�������
}
