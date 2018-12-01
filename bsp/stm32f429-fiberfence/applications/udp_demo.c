#include "udp_demo.h"

#include "usart.h"
#include "lwip/api.h"
#include "lwip/lwip_sys.h"
//#include "lwip/ip4.h"
#include "lwip/ip_addr.h"
#include "string.h"
#include "adc_timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F4&F7开发板
//NETCONN API编程方式的UDP测试代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/2/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 
//TCP客户端任务
#define UDP_PRIO		6
//任务堆栈大小
#define UDP_STK_SIZE	5000

char ADC_Value[15000]={'0'};
rt_uint16_t *value;
extern struct rt_mailbox mb_udp;
extern struct ADC_data adc_data_a, adc_data_b;

u8 udp_flag;							//UDP数据发送标志位

//udp任务函数
void udp_demo_test(void)
{
	err_t err;
	static struct netconn *udpconn;
	static struct netbuf  *sentbuf;
	ip_addr_t destipaddr;
	udpconn = netconn_new(NETCONN_UDP);  //创建一个UDP链接
	udpconn->recv_timeout = 10;  		
	
	if(udpconn != NULL)  //创建UDP连接成功
	{
		err = netconn_bind(udpconn,IP_ADDR_ANY,UDP_DEMO_PORT); 
		IP4_ADDR(&destipaddr,192,168, 1,102); //构造目的IP地址
    netconn_connect(udpconn,&destipaddr,UDP_DEMO_PORT); 	//连接到远端主机
		
		
		if(err == ERR_OK)//绑定完成
		{
			while(1)
			{
					if (rt_mb_recv(&mb_udp, (rt_uint32_t*)&value, RT_WAITING_FOREVER)== RT_EOK)
					{
						sentbuf = netbuf_new();
						netbuf_alloc(sentbuf, 2000);
						memcpy(sentbuf->p->payload,(void*)ADC_Value, 2000);
						
						err = netconn_send(udpconn,sentbuf);  	//将netbuf中的数据发送出去
						if(err != ERR_OK)
						{
							rt_kprintf("发送失败\r\n");
						}	
						netbuf_delete(sentbuf);      	//删除buf
						
//						sentbuf = netbuf_new();
//						netbuf_alloc(sentbuf, 1000);
//						memcpy(sentbuf->p->payload,(void*)(value+500), 1000);
//						
//						err = netconn_send(udpconn,sentbuf);  	//将netbuf中的数据发送出去
//						if(err != ERR_OK)
//						{
//							rt_kprintf("发送失败\r\n");
//						}	
//						netbuf_delete(sentbuf);      	//删除buf
						//rt_thread_delay(10);
					}
			}
		}
		else 
			rt_kprintf("UDP绑定失败\r\n");

	}
	else 
		rt_kprintf("UDP连接创建失败\r\n");
}




