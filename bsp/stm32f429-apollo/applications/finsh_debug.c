#include <rtthread.h>
#include "displayInfo.h"


extern struct Display_Info info;
extern struct rt_mailbox mb_a, mb_b;



void list_all_param()
{
	rt_kprintf("--------Fiber Fence System Parameter List Tab-------\n");
	rt_kprintf("alarm threshold A:      %4d\n", info.item1.param1);
	rt_kprintf("alarm threshold B:      %4d\n", info.item2.param1);
	rt_kprintf("optic power threshold:  %4d\n", info.item3.param1);
	rt_kprintf("alarm time interval:    %4d\n", info.item4.param1);
	rt_kprintf("optic power A:          %4d\n", info.item5.param1);
	rt_kprintf("optic power B:          %4d\n", info.item6.param1);
	rt_kprintf("alarm count A:          %4d\n", info.item7.param1);
	rt_kprintf("alarm count B:          %4d\n", info.item8.param1);
	rt_kprintf("mailbox count: %d ,  %d\n", mb_a.entry, mb_b.entry);
	rt_kprintf("---------------------end----------------------------\n");
}


#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(list_all_param, list all parameter);
FINSH_FUNCTION_EXPORT_ALIAS(list_all_param, lap, list all parameter);
#endif
