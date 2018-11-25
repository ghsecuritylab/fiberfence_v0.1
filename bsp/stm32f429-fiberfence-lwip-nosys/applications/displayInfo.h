#ifndef __DISPLAYINFO_H
#define   __DISPLAYINFO_H

struct Display_Item
{
	int active;
	char *label;
	int param1;
	int param2;
	int param3;
};

struct Display_Info
{
	int active;
	char *root_label1, *root_label2;
	struct Display_Item item1;
	struct Display_Item item2;
	struct Display_Item item3;
	struct Display_Item item4;
	struct Display_Item item5;
	struct Display_Item item6;
	struct Display_Item item7;
	struct Display_Item item8;
};

extern struct Display_Info info;

void info_init(void);

#endif
