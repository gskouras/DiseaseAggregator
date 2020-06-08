#ifndef CLIENT_H_
#define CLIENT_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 


typedef struct
{
	char * queryFile;
	int numThreads;
	int servPort;
	char *servIP;	
} Params;


int digitValidate(char *a);

Params inputValidate (int argc, char *argv[]);


#endif