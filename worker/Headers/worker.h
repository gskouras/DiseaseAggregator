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
#include "../Headers/hash.h"
#include "../Headers/query_server.h"


#define PERMISSIONS 0666


typedef struct 
{
	char fileName[50];
	int disHashSize;
	int countryHashSize;
	int bucketsize;
} Params;


typedef struct
{
	char ** countries;
	int total;
	int success;
	int fail;
} Logfile_Info;




/*** Utillity Functions ***/

int readPatientRecordsFile (  Params ,  HashTable * , HashTable *, Patient_list *, int, Logfile_Info *);

int digitValidate( char * );

Params inputValidate ( int , char * argv[] );

Patient line_tokenize( char * , Patient, char *, char * );

/****************************/

/*** Fifo Related Functions ***/

void write_summary_stats( HashTable *, char *, Date, int );

char * read_from_fifo( int read_fd, int buffersize);

void write_to_fifo(int  write_fd, char * message);

/*************************/

/*** Signal Handlers ****/

void signal_handler( int );

/*************************/

#endif