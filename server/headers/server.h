#ifndef SERVE_H_
#define SERVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/socket.h>              
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>	

typedef struct
{
	int queryPortNum;
	int statisticsPortNum;
	int numThreads;
	int bufferSize;
} Params;

typedef struct {
	int flag; // 0 is means a job to send to workers 1 means a job that is send from workers
	int start;
	int tail;
}Job;

typedef struct {
	Job *job_array;
	int job_in;
	int job_out;
	int count;
}Cycle_Buffer;



int digitValidate(char *a);

Params inputValidate (int argc, char *argv[]);

void perror_exit(char *);


#endif