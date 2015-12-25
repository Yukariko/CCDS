#ifndef LVM_H
#define LVM_H

#include <iostream>
#include <lvm2app.h>

using namespace std;

class LVM
{
public:
	bool lv_create(const string& lv_name, int size);
	bool lv_size_up(const string& lv_name, int size);
private:
};

#endif