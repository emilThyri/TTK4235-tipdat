#include <stdio.h>
#include <sys/time.h>

#include "liftWatchdog.h"

double start_time;
double end_time;

double getWallTime(void){
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}


void startTimer() {
	start_time = getWallTime();
	end_time = start_time + 3;
}

int wait3Seconds() {
	double poll_time = getWallTime();
	double secs = poll_time-start_time;
	
	if (poll_time >= end_time){
		start_time = 0;
		end_time = 0;
		return 1;
	}
	return 0;
}



