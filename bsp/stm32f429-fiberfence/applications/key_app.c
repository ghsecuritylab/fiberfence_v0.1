#include "key_app.h"
#include "lcd.h"

extern int current_display_id;

//extern struct Display_Item alarm_th_item, optic_power_item;

extern struct Display_Info info;

void lcdDisplayItem(struct Display_Item item)
{
	LcdCommandWrite(0x01);
	delay_us(20);
	LcdCommandWrite(0x83);
	delay_us(1);
	lcdStrWrite(item.label);
	delay_us(1);
	LcdCommandWrite(0xC5);
	delay_us(1);
	lcdDecimalWrite(item.param1);
}

void lcdDisplayMainwindow()
{
	LcdCommandWrite(0x01);
	delay_us(20);
	LcdCommandWrite(0x85);
	delay_us(1);
	lcdStrWrite(info.root_label1);
	delay_us(1);
	LcdCommandWrite(0xC3);
	delay_us(1);
	lcdStrWrite(info.root_label2);
}

void key_up_press(void)
{
	switch(current_display_id)
	{
		case 0:
			current_display_id++;
			info.active = 0;
			info.item1.active = 1;
			lcdDisplayItem(info.item1);
			break;
		case 1:
			current_display_id++;
			info.item1.active = 0;
			info.item2.active = 1;
			lcdDisplayItem(info.item2);
			break;
		case 2:
			current_display_id++;
			info.item2.active = 0;
			info.item3.active = 1;
			lcdDisplayItem(info.item3);
			break;
		case 3:
			current_display_id++;
			info.item3.active = 0;
			info.item4.active = 1;
			lcdDisplayItem(info.item4);
			break;
		case 4:
			current_display_id++;
			info.item4.active = 0;
			info.item5.active = 1;
			lcdDisplayItem(info.item5);
			break;
		case 5:
			current_display_id++;
			info.item5.active = 0;
			info.item6.active = 1;
			lcdDisplayItem(info.item6);
			break;
		case 6:
			current_display_id++;
			info.item6.active = 0;
			info.item7.active = 1;
			lcdDisplayItem(info.item7);
			break;
		case 7:
			current_display_id++;
			info.item7.active = 0;
			info.item8.active = 1;
			lcdDisplayItem(info.item8);
			break;
		case 8:
			current_display_id=1;
			info.item8.active = 0;
			info.item1.active = 1;
			lcdDisplayItem(info.item1);
			break;
		default:
			break;
	}
}

void key_down_press(void)
{
	switch(current_display_id)
	{
		case 0:
			current_display_id=1;
			info.active = 0;
			info.item1.active = 1;
			lcdDisplayItem(info.item1);
			break;
		case 1:
			current_display_id=8;
			info.item1.active = 0;
			info.item8.active = 1;
			lcdDisplayItem(info.item8);
			break;
		case 2:
			current_display_id--;
			info.item2.active = 0;
			info.item1.active = 1;
			lcdDisplayItem(info.item1);
			break;
		case 3:
			current_display_id--;
			info.item3.active = 0;
			info.item2.active = 1;
			lcdDisplayItem(info.item2);
			break;
		case 4:
			current_display_id--;
			info.item4.active = 0;
			info.item3.active = 1;
			lcdDisplayItem(info.item3);
			break;
		case 5:
			current_display_id--;
			info.item5.active = 0;
			info.item4.active = 1;
			lcdDisplayItem(info.item4);
			break;
		case 6:
			current_display_id--;
			info.item6.active = 0;
			info.item5.active = 1;
			lcdDisplayItem(info.item5);
			break;
		case 7:
			current_display_id--;
			info.item7.active = 0;
			info.item6.active = 1;
			lcdDisplayItem(info.item6);
			break;
		case 8:
			current_display_id--;
			info.item8.active = 0;
			info.item7.active = 1;
			lcdDisplayItem(info.item7);
			break;
		default:
			break;
	}
}

void key_plus_press(void)
{
	switch(current_display_id)
	{
		case 0:
			break;
		case 1:
			if(info.item1.param1<4095)
				info.item1.param1++;
			lcdDisplayItem(info.item1);
			break;
		case 2:
			if(info.item2.param1<4095)
				info.item2.param1++;
			lcdDisplayItem(info.item2);
			break;
		case 3:
			if(info.item3.param1<4095)
				info.item3.param1++;
			lcdDisplayItem(info.item3);
			break;
		case 4:
			if(info.item4.param1<100)
				info.item4.param1++;
			lcdDisplayItem(info.item4);
			break;
		default:
			break;
	}
}

void key_sub_press(void)
{
	switch(current_display_id)
	{
		case 0:
			break;
		case 1:
			if(info.item1.param1>1)
				info.item1.param1--;
			lcdDisplayItem(info.item1);
			break;
		case 2:
			if(info.item2.param1>1)
				info.item2.param1--;
			lcdDisplayItem(info.item2);
			break;
		case 3:
			if(info.item3.param1>1)
				info.item3.param1--;
			lcdDisplayItem(info.item3);
			break;
		case 4:
			if(info.item4.param1>1)
				info.item4.param1--;
			lcdDisplayItem(info.item4);
			break;
		default:
			break;
	}
}

void key_enter_press(void)
{

}

void key_back_perss(void)
{
	switch(current_display_id)
	{
		case 0:
			current_display_id=0;
			info.active = 1;
			lcdDisplayMainwindow();
			break;
		case 1:
			current_display_id=0;
			info.active = 1;
			info.item1.active = 0;
			lcdDisplayMainwindow();
			break;
		case 2:
			current_display_id=0;
			info.active = 1;
			info.item2.active = 0;
			lcdDisplayMainwindow();
			break;
		default:
			break;
	}
}


