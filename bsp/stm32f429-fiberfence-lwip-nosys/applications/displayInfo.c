#include "displayInfo.h"

int current_display_id = 0;

struct Display_Item alarm_A_th_item, alarm_B_th_item, optic_power_th_item, 
				optic_power_A_item, optic_power_B_item, 
				alarm_count_A_item, alarm_count_B_item, 
				alarm_time_interval_item;

struct Display_Info info;

void info_init()
{
	alarm_A_th_item.active = 0;
	alarm_A_th_item.label = "alarm_A_th:";
	alarm_A_th_item.param1 = 2200;
	
	alarm_B_th_item.active = 0;
	alarm_B_th_item.label = "alarm_B_th:";
	alarm_B_th_item.param1 = 2200;
	
	optic_power_th_item.active = 0;
	optic_power_th_item.label  = "power_th:";
	optic_power_th_item.param1 = 180;
	
	alarm_time_interval_item.active = 0;
	alarm_time_interval_item.label  = "alarm_time:";
	alarm_time_interval_item.param1 = 5;
	
	optic_power_A_item.active = 0;
	optic_power_A_item.label  = "power_A:";
	optic_power_A_item.param1 = 0;
	
	optic_power_B_item.active = 0;
	optic_power_B_item.label  = "power_B:";
	optic_power_B_item.param1 = 0;
	
	alarm_count_A_item.active = 0;
	alarm_count_A_item.label  = "alarm_count_A:";
	alarm_count_A_item.param1 = 0;
	
	alarm_count_B_item.active = 0;
	alarm_count_B_item.label  = "alarm_count_B:";
	alarm_count_B_item.param1 = 0;
	
	info.active = 1;
	info.root_label1 = "Welcome";
	info.root_label2 = "Fiber Fence";
	info.item1 = alarm_A_th_item;
	info.item2 = alarm_B_th_item;
	info.item3 = optic_power_th_item;
	info.item4 = alarm_time_interval_item;
	info.item5 = optic_power_A_item;
	info.item6 = optic_power_B_item;
	info.item7 = alarm_count_A_item;
	info.item8 = alarm_count_B_item;
	
}
