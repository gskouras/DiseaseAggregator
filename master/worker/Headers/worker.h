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
#include "../../headers/input_list.h"


#define PERMISSIONS 0666
#define MAX_STATS 1000000


typedef struct 
{
	char fileName[50];
	int disHashSize;
	int bucketsize;
	int server_portNum;
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

int readInputDirs(Params , HashTable *, Patient_list *, Directory_list *, int , Logfile_Info *, char **, char **);

int readPatientRecordsFile (  Params ,  HashTable * , Patient_list *, int, Logfile_Info *, char **);

int digitValidate( char * );

Params init_params(char *, char *, char **, char **, int);

Patient line_tokenize( char * , Patient, char *, char * );

char * query_handler(char * , HashTable *, HashTable * , Patient_list *, Directory_list * , int );

/****************************/

/*** Fifo Related Functions ***/

char * read_from_fifo( int read_fd, int buffersize );

char * calculate_summary_stats( HashTable *, char *, Date, int, Params );

void send_summary_stats(char *, Directory_list * , int, Params );

/*************************/

/*** Socket Related Functions ***/

int create_socket( struct sockaddr**, struct sockaddr** , struct sockaddr_in *, struct sockaddr_in *, int * , socklen_t * );

void write_to_socket(int  write_fd, char * message );

/***************************/

/*** Signal Handlers ****/

void signal_handler( int );

void perror_exit( char * );

/*************************/


#endif