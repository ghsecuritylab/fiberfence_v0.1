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

TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM4_Handler;      //定时器句柄 

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //通用定时器3
    TIM4_Handler.Init.Prescaler=psc;                     //分频系数
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM4_Handler.Init.Period=arr;                        //自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM4_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器4和定时器4更新中断：TIM_IT_UPDATE   
}


//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}
	
	if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM3中断   
	}
}


//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);
}


//回调函数，定时器中断服务函数调用
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
//		//获取防区a光功率
//		result = adc_ReadOneSample(0x8310);
//		adc_data_a.dc1[id%100] = result&0x0fff;
//		
//		
//		//获取防区b光功率
//		result = adc_ReadOneSample(0x8730);
//		adc_data_a.dc2[id%100] = result&0x0fff;

//		
//		//获取防区b交流信号
//		result = adc_ReadOneSample(0x8b30);
//		if(data_flag==0)
//			adc_data_a.ac2[id] = result;//&0x0fff;
//		else
//			adc_data_b.ac2[id] = result;//&0x0fff;
//		
//		
//		//获取防区a交流信号
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
			
			if(error != RT_EOK)                      //检查邮箱是否已满
				rt_kprintf("mailbox full error:%d\n", (int)error);

			data_flag = ~data_flag; 
		}

	}
	

	if(htim==(&TIM4_Handler))       //定时器3
	{
			timeout++;
	}
}

void start_the_time()
{
		timeout = 0;
		__HAL_TIM_SET_COUNTER(&TIM4_Handler,0);//重设TIM4定时器的计数器值
}

int stop_the_time()
{
	return __HAL_TIM_GET_COUNTER(&TIM4_Handler)+(u32)timeout*65536;//计算所用时间
}
