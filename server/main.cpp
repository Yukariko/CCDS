#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "ccds.h"

using namespace std;

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage : %s port", argv[0]);
		exit(1);
	}

	CCDS ccds(atoi(argv[1]));
	ccds.start();
	return 0;
}