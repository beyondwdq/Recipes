#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "edsim_naive.h"

double exponential(double r);

double g_avg_arrv_intv = 5;
double g_avg_serv_time = 50;

int g_online_user_cnt = 0;

FILE *g_log_file = NULL;

void logOnlineUserCount(void);
void userDeparture(void);
void userArrival(void);

int main(int argc, const char *argv[])
{
	g_log_file = fopen("online_user_cnt.txt", "w");
	if (g_log_file==NULL){
		fprintf(stderr, "Open log file failed\n");
		return 1;
	}

	double end_time = 10000;
	Event first_arrv(0.0, &userArrival);
	enqueue(first_arrv);

	run_sim(end_time);

	fclose(g_log_file);
	return 0;
}

void logOnlineUserCount(void)
{
	fprintf(g_log_file, "%f %d\n", now(), g_online_user_cnt);
}

void userDeparture(void)
{
	--g_online_user_cnt;
	logOnlineUserCount();
}

void userArrival(void)
{
	++g_online_user_cnt;
	logOnlineUserCount();

	double interval = exponential(g_avg_arrv_intv);
	double serv_time = exponential(g_avg_serv_time);
	Event next_arrv(now()+interval, &userArrival);
	Event departure(now()+serv_time, &userDeparture);
	enqueue(next_arrv);
	enqueue(departure);
}

//random number generation
double uniform()
{
	return ((double)rand())/( ((double)(RAND_MAX)) + 1);
}

double exponential()
{ 
	return (-log(uniform())); 
}
double exponential(double r)
{ 
	return (r * exponential());
}
