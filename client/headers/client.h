#ifndef CLIENT_H_
#define CLIENT_H_


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
	char * queryFile;
	int numThreads;
	int servPort;
	char *servIP;	
} Params;

typedef struct 
{
	char **	queries;
	int current;
	int total;
} Query_array;



int digitValidate( char * );

Params inputValidate ( int argc, char *argv[] );

void readQueryFile( Params params );

int count_queries( char * );

void * send_query( void * );

void initQueryArray( int );

void write_to_socket( int , char * );


#endif