#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "ccdc.h"

using namespace std;

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage : %s ip port\n",argv[0]);
		exit(1);
	}


	CCDC ccdc(argv[1], atoi(argv[2]));
	ccdc.start();
	return 0;
}
