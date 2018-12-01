#include "rtthread.h"
#include "adc_timer.h"
#include "drv_gpio.h"
#include "AD7924.h"
//////////////////////////////////////////////////////////////////////////////////	 
//						  
////////////////////////////////////////////////////////////////////////////////// 	

volatile u16 id = 0;
//volatile u16 data_flag = 0;
volatile rt_bool_t data_flag = RT_FALSE;

volatile u8 timeout=0;
volatile float tm;

ALIGN(4)
struct ADC_data adc_data_a, adc_data_b;

extern struct rt_mailbox mb_a, mb_b;

extern rt_uint16_t adc_ReadOneSample(rt_uint16_t cmd);

TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 
TIM_HandleTypeDef TIM4_Handler;      //��ʱ����� 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}

void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��3
    TIM4_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM4_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM4_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�����жϣ�TIM_IT_UPDATE   
}


//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}
	
	if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //����ITM3�ж�   
	}
}


//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);
}


//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint16_t result;
	rt_err_t error;
	
	if(htim==(&TIM3_Handler))
	{
		
		result = ADC_Conversion(CHAN1_CONVERSION);
		adc_data_a.dc1[id%100] = result&0x0fff;
		//rt_kprintf("%d\n", result&0x0fff);
		result = ADC_Conversion(CHAN2_CONVERSION);
		adc_data_a.dc2[id%100] = result&0x0fff;
		result = ADC_Conversion(CHAN3_CONVERSION);
		if(data_flag==0)
			adc_data_a.ac2[id] = result;//&0x0fff;
		else
			adc_data_b.ac2[id] = result;//&0x0fff;
		result = ADC_Conversion(CHAN4_CONVERSION);
		//rt_kprintf("channel4:%d\n", result >> 12);
		if(data_flag==0)
			adc_data_a.ac1[id] = 2560;//result&0x0fff;
		else
			adc_data_b.ac1[id] = 2530;//result&0x0fff;
		
//		//start_the_time();
//		//��ȡ����a�⹦��
//		result = adc_ReadOneSample(0x8310);
//		adc_data_a.dc1[id%100] = result&0x0fff;
//		
//		
//		//��ȡ����b�⹦��
//		result = adc_ReadOneSample(0x8730);
//		adc_data_a.dc2[id%100] = result&0x0fff;

//		
//		//��ȡ����b�����ź�
//		result = adc_ReadOneSample(0x8b30);
//		if(data_flag==0)
//			adc_data_a.ac2[id] = result;//&0x0fff;
//		else
//			adc_data_b.ac2[id] = result;//&0x0fff;
//		
//		
//		//��ȡ����a�����ź�
//		result = adc_ReadOneSample(0x8f10);
//		if(data_flag==0)
//			adc_data_a.ac1[id] = result&0x0fff;
//		else
//			adc_data_b.ac1[id] = result&0x0fff;
		
		//rt_kprintf("time:%d us\n", stop_the_time());
		
		id++;
		if(id >= 1000){
			id = 0;
			
			if(data_flag==0)
			{
				error = rt_mb_send(&mb_a, (rt_uint32_t)&adc_data_a.ac1);
				error = rt_mb_send(&mb_b, (rt_uint32_t)&adc_data_a.ac2);
			}
			else
			{
				error = rt_mb_send(&mb_a, (rt_uint32_t)&adc_data_b.ac1);
				error = rt_mb_send(&mb_b, (rt_uint32_t)&adc_data_b.ac2);
			}
			
			if(error != RT_EOK)                      //��������Ƿ�����
				rt_kprintf("mailbox full error:%d\n", (int)error);

			data_flag = ~data_flag; 
		}

	}
	

	if(htim==(&TIM4_Handler))       //��ʱ��3
	{
			timeout++;
	}
}

void start_the_time()
{
		timeout = 0;
		__HAL_TIM_SET_COUNTER(&TIM4_Handler,0);//����TIM4��ʱ���ļ�����ֵ
}

int stop_the_time()
{
	return __HAL_TIM_GET_COUNTER(&TIM4_Handler)+(u32)timeout*65536;//��������ʱ��
}
