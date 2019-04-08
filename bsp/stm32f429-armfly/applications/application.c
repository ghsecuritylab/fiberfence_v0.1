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
//#include "udp_demo.h"
#include "lcd.h"
#include "lcd12864.h"

#ifdef RT_USING_DFS
/* dfs init */
#include <dfs.h>
/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#include <dfs_posix.h>
#endif

#ifdef PKG_USING_GUIENGINE
#include "rtgui_demo.h"
#include <rtgui/driver.h>
#endif

#define DM_ADD (*((volatile unsigned int *) 0x64001000))
#define DM_ADD1 (*((volatile unsigned int *) 0x64002000))

#define DM_CMD (*((volatile unsigned int *) 0x64002001))

struct rt_ringbuffer *buffer;

void rt_init_thread_entry(void* parameter)
{    
     /* initialization RT-Thread Components */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_init();
#endif
    
#ifdef RT_USING_DFS  
    #ifdef RT_USING_DFS_ELMFAT  

				/* mount spi flash as root directory */
        if (dfs_mount("W25Q64", "/", "elm", 0, 0) == 0)
        {
            rt_kprintf("spi flash mount to / !\n");
        }
        else
        {
            rt_kprintf("spi flash mount to / failed!\n");
        }		

        /* mount sd card fat partition 0 as root directory */
        if (dfs_mount("sd0", "/sd0", "elm", 0, 0) == 0)
        {
            rt_kprintf("sd0 mount to /sd0 !\n");
        }
        else
        {
            rt_kprintf("sd0 mount to /sd0 failed!\n");
        } 				
    #endif /* RT_USING_DFS_ELMFAT */
        
#endif /* DFS */
				
//				lcd1602_init();
//				Lcd_init();
				
	buffer = rt_ringbuffer_create(50);
			
//	int data;
//	DM_ADD = 0xfeff;
//	DM_ADD1 = 0x28;
//	data=DM_CMD;
//	DM_ADD1 = 0x29;
//	data=DM_CMD;
//	DM_ADD1 = 0x2a;
//	data=DM_CMD;
//	DM_ADD1 = 0x2b;
//	data=DM_CMD;
				
	//udp_demo_test();
		while(1){
			DM_ADD = 0xffff;
			//rt_kprintf("ON\n");
			rt_thread_delay(2000);
			DM_ADD = 0xfdff;
			//rt_kprintf("OFF\n");
			rt_thread_delay(2000);
		}
    
}

void rt_ringbuffer_produce_entry(void* parameter)
{
	rt_uint8_t size, i=0;
	//rt_thread_delay(5000);
	while(1)
	{
		size = rt_ringbuffer_putchar(buffer, i);
		if(size==0)
			rt_kprintf("ringbuffer is full!\n");
		else
			i++;
		
		if(i>=100)
			i=0;
		rt_thread_delay(100);
	}

}

void rt_ringbuffer_consume_entry(void* parameter)
{
	rt_uint8_t temp, i;
	//rt_thread_delay(5000);
	while(1)
	{
		if(rt_ringbuffer_data_len(buffer)>=15)
		{
			rt_kprintf("read 5 char form buffer:");
			for(i=0; i<15; i++)
			{
				rt_ringbuffer_getchar(buffer, &temp);
				rt_kprintf("%02d ",temp);
			}
			rt_kprintf("\n buffer space size:%d\n", rt_ringbuffer_space_len(buffer));
		}
		rt_thread_delay(10);
	}
}

int rt_application_init()
{
    rt_thread_t tid, produce_tid, consume_tid;

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
		
//		produce_tid = rt_thread_create("ringbuffer_produce",
//        rt_ringbuffer_produce_entry, RT_NULL,
//        2048, RT_THREAD_PRIORITY_MAX/3+5, 20);

//    if (produce_tid != RT_NULL)
//        rt_thread_startup(produce_tid);
//		
//		consume_tid = rt_thread_create("ringbuffer_consume",
//        rt_ringbuffer_consume_entry, RT_NULL,
//        2048, RT_THREAD_PRIORITY_MAX/3+5, 20);

//    if (consume_tid != RT_NULL)
//        rt_thread_startup(consume_tid);
		


    return 0;
}
