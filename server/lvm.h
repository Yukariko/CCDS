#ifndef LVM_H
#define LVM_H

#include <iostream>
#include <lvm2app.h>

using namespace std;

class LVM
{
public:
	bool lv_size_up(const string& volume, int size);
private:
};

#endif