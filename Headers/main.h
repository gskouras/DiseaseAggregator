#include <stdio.h>
#include <ctype.h> 
#include <string.h>
#include  <stdlib.h>
#include "../Headers/hash.h"


typedef struct 
{
	char *fileName;
	int disHashSize;
	int countryHashSize;
	int bucketsize;
} Params;

int digitValidate( char * );

Params inputValidate ( int , char * argv[]);

int readPatientRecordsFile ( Params , HashTable * , Patient_list *);

Patient string_tokenize(char *line, Patient);