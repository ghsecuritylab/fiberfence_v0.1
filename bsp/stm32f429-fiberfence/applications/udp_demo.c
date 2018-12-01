#include "udp_demo.h"

#include "usart.h"
#include "lwip/api.h"
#include "lwip/lwip_sys.h"
//#include "lwip/ip4.h"
#include "lwip/ip_addr.h"
#include "string.h"
#include "adc_timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4&F7������
//NETCONN API��̷�ʽ��UDP���Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/2/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
//TCP�ͻ�������
#define UDP_PRIO		6
//�����ջ��С
#define UDP_STK_SIZE	5000

char ADC_Value[15000]={'0'};
rt_uint16_t *value;
extern struct rt_mailbox mb_udp;
extern struct ADC_data adc_data_a, adc_data_b;

u8 udp_flag;							//UDP���ݷ��ͱ�־λ

//udp������
void udp_demo_test(void)
{
	err_t err;
	static struct netconn *udpconn;
	static struct netbuf  *sentbuf;
	ip_addr_t destipaddr;
	udpconn = netconn_new(NETCONN_UDP);  //����һ��UDP����
	udpconn->recv_timeout = 10;  		
	
	if(udpconn != NULL)  //����UDP���ӳɹ�
	{
		err = netconn_bind(udpconn,IP_ADDR_ANY,UDP_DEMO_PORT); 
		IP4_ADDR(&destipaddr,192,168, 1,102); //����Ŀ��IP��ַ
    netconn_connect(udpconn,&destipaddr,UDP_DEMO_PORT); 	//���ӵ�Զ������
		
		
		if(err == ERR_OK)//�����
		{
			while(1)
			{
					if (rt_mb_recv(&mb_udp, (rt_uint32_t*)&value, RT_WAITING_FOREVER)== RT_EOK)
					{
						sentbuf = netbuf_new();
						netbuf_alloc(sentbuf, 2000);
						memcpy(sentbuf->p->payload,(void*)ADC_Value, 2000);
						
						err = netconn_send(udpconn,sentbuf);  	//��netbuf�е����ݷ��ͳ�ȥ
						if(err != ERR_OK)
						{
							rt_kprintf("����ʧ��\r\n");
						}	
						netbuf_delete(sentbuf);      	//ɾ��buf
						
//						sentbuf = netbuf_new();
//						netbuf_alloc(sentbuf, 1000);
//						memcpy(sentbuf->p->payload,(void*)(value+500), 1000);
//						
//						err = netconn_send(udpconn,sentbuf);  	//��netbuf�е����ݷ��ͳ�ȥ
//						if(err != ERR_OK)
//						{
//							rt_kprintf("����ʧ��\r\n");
//						}	
//						netbuf_delete(sentbuf);      	//ɾ��buf
						//rt_thread_delay(10);
					}
			}
		}
		else 
			rt_kprintf("UDP��ʧ��\r\n");

	}
	else 
		rt_kprintf("UDP���Ӵ���ʧ��\r\n");
}




