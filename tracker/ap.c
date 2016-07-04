#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <wiringPi.h>
#include <mcp3004.h>

#include "gps.h"
#include "ap.h"
#include "misc.h"

double ReadAP(int chan, double FullScale)
{
	int RawValue;
	double Result;

   	RawValue = analogRead(chan);
	Result = (double)RawValue * FullScale / 1024.0;
	//printf("RawValue = %i\n", RawValue);

	return Result;
}


void *APLoop(void *some_void_ptr)
{
	double Pressure;
	FILE *fp;
	struct TGPS *GPS;

	GPS = (struct TGPS *)some_void_ptr;

	if (mcp3004Setup(100, 0)<0)
	{
		printf("failed to setup SPI");
	}
		
	while (1)
	{
		Pressure = ReadAP(100, 1034.21);
		GPS->Pressure = Pressure;
		// printf("Pressure = %lf\n", Pressure);

		sleep(10);
	}

	return 0;
}
