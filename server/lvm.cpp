#include "lvm.h"

bool LVM::lv_size_up(const string& volume, int size)
{
	char buf[4096];
	sprintf(buf, "lvextend -L+%dMB /dev/vg0/%s", size, volume.c_str());
	return system(buf) == 0;
}