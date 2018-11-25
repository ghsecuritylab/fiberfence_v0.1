#include <rtthread.h>
#include "key_app.h"
#include "common_def.h"
#include "24cxx.h"

extern struct Display_Info info;
extern struct rt_mailbox mb_a, mb_b;

enum KEY_VALUE
{
	KEY_UP=1,
	KEY_DOWN,
	KEY_PLUS,
	KEY_SUB,
	KEY_ENTER,
	KEY_RETURN
};

void key_test(enum KEY_VALUE value)
{
	switch(value)
	{
		case KEY_UP:
			key_up_press();
			break;
		case KEY_DOWN:
			key_down_press();
			break;
		case KEY_PLUS:
			key_plus_press();
			break;
		case KEY_SUB:
			key_sub_press();
			break;
		case KEY_ENTER:
			key_enter_press();
			break;
		case KEY_RETURN:
			key_back_perss();
			break;
		default:
			break;
	}
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(key_test, key test. e.g: key_test(KEY_UP));
#endif

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

void sub_version()
{
	rt_kprintf("\n \\ | /\n");
	rt_kprintf("- RT -     Apollo Uart Project\n");
	rt_kprintf(" / | \\     %d.%d.%d build %s\n",
						 RT_VERSION, RT_SUBVERSION, RT_REVISION, __DATE__);
}

void save_parameter()
{
	struct Params P = {2200,
										2200,
										300,
										5,
										192,
										168,
										103,
										85,
										5900};
	AT24CXX_Write(0,(u8 *)&P, sizeof(struct Params));
}

void load_parameter()
{
	struct Params P;
	AT24CXX_Read(0,(u8 *)&P, sizeof(struct Params));
	rt_kprintf("%d.%d.%d.%d\n", P.addr1,P.addr2,P.addr3,P.addr4);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(list_all_param, list all parameter);
FINSH_FUNCTION_EXPORT_ALIAS(list_all_param, lap, list all parameter);
FINSH_FUNCTION_EXPORT(sub_version,);
FINSH_FUNCTION_EXPORT(save_parameter,);
FINSH_FUNCTION_EXPORT(load_parameter,);
#endif
