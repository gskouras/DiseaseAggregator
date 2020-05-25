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

int digitValidate( char * );

Params inputValidate ( int , char * argv[] );

int readPatientRecordsFile (  Params ,  HashTable * , HashTable *, Patient_list *, int, Logfile_Info *);

Patient line_tokenize( char * , Patient, char *, char * );

void write_summary_stats( HashTable *, char *, Date, int );

char * read_from_fifo( int read_fd, int buffersize);

void write_to_fifo(int  write_fd, char * message);

void signal_handler();