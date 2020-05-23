#ifndef QUERY_SERVER_H_
#define QUERY_SERVER_H_
#include <stdio.h>

#include "../Headers/worker.h"
#include "../Headers/heap.h"

/*** Queries ***/

char * listCountries();

void topDiseases( char *, HashTable *, Patient_list *);

void topCountries( char *, HashTable *, Patient_list * );

void recordPatientExit( char * , Patient_list * );

void numCurrentPAtients( char * , HashTable * );

void diseaseFrequency( char * , HashTable *, Patient_list * );

void insertPatientRecord( char * , HashTable * , HashTable * , Patient_list * );

/********************/


/***** Queries Functions ****/

void dateTokenize( char *, Date *, Date *);

void open_manual();

Patient line_tokenize_without_exitDate( char * );

void df_tokenize ( char *, char **, char **, Date *, Date *);

void topK_tokenize(char * , char ** , Date * , Date *, int *);

/*********************/
#endif