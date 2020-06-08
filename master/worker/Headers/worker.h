#ifndef WORKER_H_
#define WORKER_H_

#include <stdio.h>
#include <ctype.h> 
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h> 
#include <dirent.h> 
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/socket.h>              
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>	
#include "../Headers/hash.h"
#include "../Headers/query_server.h"


#define PERMISSIONS 0666


typedef struct 
{
	char fileName[50];
	int disHashSize;
	int bucketsize;
	int portNum;
	int write_fd;
	int read_fd;
	char * ipAddress;
} Params;


typedef struct
{
	char ** countries;
	int total;
	int success;
	int fail;
} Logfile_Info;




/*** Utillity Functions ***/

int readPatientRecordsFile (  Params ,  HashTable * , Patient_list *, int, Logfile_Info *);

int digitValidate( char * );

Params init_params(char *, char *, char **, char **, int);

Patient line_tokenize( char * , Patient, char *, char * );

/****************************/

/*** Fifo Related Functions ***/

void write_summary_stats( HashTable *, char *, Date, int, Params);

char * read_from_fifo( int read_fd, int buffersize);

void write_to_socket(int  write_fd, char * message);

/*************************/

/*** Signal Handlers ****/

void signal_handler( int );

/*************************/

#endif