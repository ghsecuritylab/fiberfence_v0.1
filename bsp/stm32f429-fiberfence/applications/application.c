/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>
#include <drivers/mtd_nand.h>
#include "finsh.h"
#include "time.h"
#include "drv_gpio.h"
#include "adc_timer.h"
#include "displayInfo.h"
#include "AD7924.h"
#include "lcd.h"
#include "key.h"
#include "key_app.h"
#include "udp_demo.h"

#include <rtthread.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>

#ifdef RT_USING_DFS
/* dfs init */

/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>

#endif

#ifdef RT_USING_LWIP
#include <lwip/lwip_sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "drv_eth.h"
#endif

rt_thread_t tid, tid1, powerid, alarmid_A, alarmid_B, keyid;

struct rt_mailbox mb_a, mb_b, mb_udp;

static char mb_a_pool[4], mb_b_pool[4], mb_udp_pool[4];

//static char sd[3000];

extern struct ADC_data adc_data_a;

//extern rt_uint16_t adc_ReadOneSample(rt_uint16_t cmd);
void udpinit(char *send_data)
{
    int sock, port=8089;
    struct hostent *host;
    struct sockaddr_in server_addr;

    /* ????????url??host??(?????,??????) */
    host = (struct hostent *) gethostbyname("192.168.1.102");

    /* ????socket,???SOCK_DGRAM,UDP?? */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        rt_kprintf("Socket error\n");
        return;
    }

    /* ???????????? */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

		sendto(sock, send_data, 1000, 0,
					 (struct sockaddr *) &server_addr, sizeof(struct sockaddr));

		sendto(sock, send_data+1000, 1000, 0,
					 (struct sockaddr *) &server_addr, sizeof(struct sockaddr));
    lwip_close(sock);
}


void rt_init_thread_entry(void* parameter)
{    
     /* initialization RT-Thread Components */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_init();
#endif
  
		AD7924_Init();
		TIM4_Init(65535-1, 90-1);
		TIM3_Init(250-1, 90-1);
		info_init();
		lcd1602_init();
    
#ifdef RT_USING_DFS   
	dfs_init();
        
#ifdef RT_USING_DFS_ELMFAT
	elm_init();
    
    /* mount sd card fat partition 0 as root directory */
    if (dfs_mount("W25Q64", "/", "elm", 0, 0) == 0)
    {
        rt_kprintf("spi flash mount to / succesfull!\n");
    }
    else
    {
        rt_kprintf("spi flash mount to / failed!\n");
    }
    
    /* mount sd card fat partition 0 as root directory */
    if (dfs_mount("sd0", "/sdcard", "elm", 0, 0) == 0)
    {
        rt_kprintf("sd card mount to /sdcard succesfull!\n");        
    }
    else
    {
        rt_kprintf("sd card mount to /sdcard failed!\n");
    }
#endif /* RT_USING_DFS_ELMFAT */
        
#endif /* DFS */
		
		//udp_demo_test();
		
		while(1){
			//adc_ReadOneSample(0x8310);
			rt_thread_delay(1000);
		}
}

/*
*******************LED指示灯信号**********************
*/
void rt_system_led_thread_entry(void* parameter)
{ 
	while(1)
	{
		HAL_GPIO_WritePin(GPIOG, LED9_Pin|CPU_RUN_Pin, GPIO_PIN_SET);
		rt_thread_delay(100);
		HAL_GPIO_WritePin(GPIOG, LED9_Pin|CPU_RUN_Pin, GPIO_PIN_RESET);
		rt_thread_delay(100);
	}
}

/*
*******************光功率监测**********************
*/
void rt_optic_power_thread_entry(void* parameter)
{
	int i;
	while(1){
//		for(i=0;i<100;i++)
				rt_kprintf("%d   %d\n",adc_data_a.dc1[10]&0x0fff, adc_data_a.dc2[10]&0x0fff);
		if(adc_data_a.dc1[10]<info.item3.param1)
			LED_PowerA = OFF;
		else
			LED_PowerA = ON;
		
		if(adc_data_a.dc2[10]<info.item3.param1)
			LED_PowerB = OFF;
		else
			LED_PowerB = ON;
		rt_thread_delay(50);

	}
}

/*
*******************防区A报警信号处理**********************
*/
void rt_alarm_process_A_thread_entry(void* parameter)
{
	int i, count=0;
	rt_uint16_t *value;
	rt_err_t error;

	while(1)
	{
		if (rt_mb_recv(&mb_a, (rt_uint32_t*)&value, RT_WAITING_FOREVER)== RT_EOK)
		{
			
			//等待一个报警时间间隔
			if(count<4*info.item4.param1){
				count++;
				goto send_data;           //报警时间间隔未到，直接将数据发送到PC
			}
			
			//开始信号处理
			for(i=0; i<1000; i++)
			{
				if(value[i]>info.item1.param1 || value[i]<(2*2048-info.item1.param1))
				{
					HAL_GPIO_WritePin(GPIOG, LED12_Pin, GPIO_PIN_RESET);            //打开报警LED
					//HAL_GPIO_WritePin(CTRL_B_GPIO_Port, CTRL_B, GPIO_PIN_SET);      //输出报警开关量
					
					info.item7.param1++;      //报警计数+1
					if(info.item7.active)     //如果lcd当前显示报警计数，更新显示
					{
						//lcdDisplayItem(info.item7);
					}
					count=0;        //报警时间间隔清零，开始计数
  				break;
				}
			}
			
			//rt_kprintf("adc1:%d\n", value[100]);
		}
		
		//达到报警时间间隔，关闭报警
		if(count>=4*info.item4.param1)
		{
			HAL_GPIO_WritePin(GPIOG, LED12_Pin, GPIO_PIN_SET);                    //关闭报警LED
			HAL_GPIO_WritePin(CTRL_B_GPIO_Port, CTRL_B, GPIO_PIN_RESET);          //关闭报警开关量
		}
		
		//将数据打包发送到PC处理
		send_data:
			udpinit((char*)value);
//			error = rt_mb_send(&mb_udp, (rt_uint32_t)value);
//			for(i=0; i<1000; i++)
//			{
//				sd[i*3+0] = 0x55;               //添加校验码
//				sd[i*3+1] = value[i]&0x00ff;
//				sd[i*3+2] = value[i]>>8;
//			}	
//			HAL_UART_Transmit_DMA(&UART6_Handler, (uint8_t *)sd, 3000);   //通过dma方式将数据包发送到PC
//			rt_thread_suspend(alarmid_B);                                 //线程挂起，等待传输完成（传输完成后中断回调函数自动激活线程）
//			rt_schedule();
			
	}
}

/*
*******************防区B报警信号处理**********************
*/
void rt_alarm_process_B_thread_entry(void* parameter)
{
	int i, count=0;
	rt_uint16_t *value;

	while(1){
		if (rt_mb_recv(&mb_b, (rt_uint32_t*)&value, RT_WAITING_FOREVER)== RT_EOK)
		{
			//等待一个报警时间间隔
			if(count<4*info.item4.param1){
				count++;
				goto send_data;           //报警时间间隔未到，直接将数据发送到PC
			}
			
			//开始信号处理
			for(i=0; i<1000; i++)
			{
				if( (value[i]&0x0FFF)>info.item2.param1 || (value[i]&0x0FFF)<(2*2048-info.item2.param1))
				{
					HAL_GPIO_WritePin(GPIOG, LED7_Pin, GPIO_PIN_RESET);               //打开报警LED
					//HAL_GPIO_WritePin(CTRL_A_GPIO_Port, CTRL_A, GPIO_PIN_SET);        //输出报警开关量
					
					info.item8.param1++;        //报警计数+1
					if(info.item8.active)       //如果lcd当前显示报警计数，更新显示
					{
						//lcdDisplayItem(info.item8);
					}
					count=0;          //报警时间间隔清零，开始计数
  				break;
				}
			}
			
			//rt_kprintf("adc2:%d\n", (value[i]&0x0FFF));
		}
		
		//达到报警时间间隔，关闭报警
		if(count>=4*info.item4.param1)
		{
			HAL_GPIO_WritePin(GPIOG, LED7_Pin, GPIO_PIN_SET);                  //关闭报警LED
			HAL_GPIO_WritePin(CTRL_A_GPIO_Port, CTRL_A, GPIO_PIN_RESET);       //关闭报警开关量
		}
		
		
		//将数据打包发送到PC处理
		send_data:
//			for(i=0; i<1000; i++)
//			{
//				sd[i*3+0] = 0x55;               //添加校验码
//				sd[i*3+1] = value[i]&0x00ff;
//				sd[i*3+2] = value[i]>>8;
//			}	
//			HAL_UART_Transmit_DMA(&UART6_Handler, (uint8_t *)sd, 3000);   //通过dma方式将数据包发送到PC
//			rt_thread_suspend(alarmid_B);                                 //线程挂起，等待传输完成（传输完成后中断回调函数自动激活线程）
//			rt_schedule();
		;
	}
	
}

/*
*******************按键处理**********************
*/
void rt_key_thread_entry(void* parameter)
{	
	int key_value;
	
	
	while(1)
	{
		key_value = KEY_Scan(1);
		switch(key_value)
		{
			case KEY0_PRES:                //back
				key_back_perss();
				rt_kprintf("KEY0_PRES\n");
				break;
			case KEY1_PRES:                //enter
				LcdCommandWrite(0x01);  //
				delay_ms(20000); 
				LcdCommandWrite(0x80+3); 
				delay_ms(100);
				lcdStrWrite("hello world!");
				delay_ms(100);
				rt_kprintf("KEY1_PRES\n");
				break;
			case KEY2_PRES:                //sub
				key_sub_press();
				rt_kprintf("KEY2_PRES\n");
				break;
			case KEY3_PRES:                //add
				key_plus_press();
				rt_kprintf("KEY3_PRES\n");
				break;
			case KEY4_PRES:                //down
				key_down_press();
				rt_kprintf("KEY4_PRES\n");
				break;
			case KEY5_PRES:                //up
				key_up_press();
				rt_kprintf("KEY5_PRES\n");
				break;
			default:
				break;	
		}
		rt_thread_delay(60);
	}
}

int rt_application_init()
{

		rt_mb_init(&mb_a,
								"mbt_a", /* name */
								&mb_a_pool[0], /* mail mb_pool */
								sizeof(mb_a_pool)/4, /*size*/
								RT_IPC_FLAG_FIFO);
								
		rt_mb_init(&mb_b,
								"mbt_b", /* name */
								&mb_b_pool[0], /* mail mb_pool */
								sizeof(mb_b_pool)/4, /*size*/
								RT_IPC_FLAG_FIFO);
								
		rt_mb_init(&mb_udp,
								"mbt_udp", /* name */
								&mb_udp_pool[0], /* mail mb_pool */
								sizeof(mb_udp_pool)/4, /*size*/
								RT_IPC_FLAG_FIFO);
	
    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3-4, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
		
		tid1 = rt_thread_create("system_run_led",
        rt_system_led_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3+2, 20);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
		
		keyid = rt_thread_create("key",
        rt_key_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3-3, 20);

    if (keyid != RT_NULL)
        rt_thread_startup(keyid);
		
		powerid = rt_thread_create("optic_power",
        rt_optic_power_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (powerid != RT_NULL)
        rt_thread_startup(powerid);
    
		alarmid_A = rt_thread_create("alarm_process_A",
        rt_alarm_process_A_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3-1, 20);

    if (alarmid_A != RT_NULL)
        rt_thread_startup(alarmid_A);
		
		alarmid_B = rt_thread_create("alarm_process_B",
        rt_alarm_process_B_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3-1, 20);

    if (alarmid_B != RT_NULL)
        rt_thread_startup(alarmid_B);

    return 0;
}
