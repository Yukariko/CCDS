#include "lvm.h"

// should be additional implements for protect of injection

bool LVM::lv_size_up(const string& lv_name, int size)
{
	char buf[4096];
	sprintf(buf, "lvextend -L+%dMB /dev/vg0/%s", size, lv_name.c_str());
	return system(buf) == 0;
}

bool LVM::lv_size_down(const string& lv_name, int size)
{
	char buf[4096];
	sprintf(buf, "lvextend -L-%dMB /dev/vg0/%s", size, lv_name.c_str());
	return system(buf) == 0;
}


bool LVM::lv_create(const string& lv_name, int size)
{
	char buf[4096];
	sprintf(buf, "lvcreate -L %dM -n %s vg0", size, lv_name.c_str());
	return system(buf) == 0;	
}