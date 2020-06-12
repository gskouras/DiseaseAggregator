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
#include <poll.h>
#include "../headers/worker_list.h"


#define FD_POOL 2

typedef struct
{
	int queryPortNum;
	int statisticsPortNum;
	int numThreads;
	int bufferSize;
} Params;


typedef struct 
{
	int flag;
	int fd;
}Socket_fd;

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

Params inputValidate (int argc, char *argv[]);

void perror_exit(char *);

/**** Cycle Buffer Functions ***/

void init_cycle_buffer();

int put_job(Job); //puts a job in the first available position

int buffer_isEmpty();

int buffer_isFull();

Job get_job();

/*************************/

int accept_connection(struct sockaddr**, socklen_t *, Socket_fd *, int , int );

int create_passive_endpoints( struct sockaddr**, struct sockaddr** , struct sockaddr_in *, struct sockaddr_in *, int * , int *, socklen_t * , Params);

void * handle_request();

void handle_worker(Job job);

int count_countries( char* );


/*** Poll() helper functions ***/

int checkAllFlags(Socket_fd * socket_fds); //return 1 oso kati den exei diavastei

void initSocketFd(Socket_fd * socket_fds, int, int);

 /****************************/


#endif