#include <stdio.h>
#include <ctype.h> 
#include <string.h>
#include  <stdlib.h>


typedef struct 
{
	char *fileName;
	int disHashNum;
	int countryHashNum;
	int bucketsize;
} Params;

int digitValidate( char * );

Params inputValidate ( int , char * argv[]);