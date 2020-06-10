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
	int fd;
}Job;

typedef struct {
	Job *job_array;
	int job_index;
	int count;
	int size;
}Cycle_Buffer;



int digitValidate(char *);

void * handle_request();

Params inputValidate (int argc, char *argv[]);

void perror_exit(char *);

/**** Cycle Buffer Functions ***/

void init_cycle_buffer();

int put_job(Job); //puts a job in the first available position

int buffer_isEmpty();

int buffer_isFull();

Job get_job();

/*************************/


#endif