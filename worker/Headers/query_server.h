#ifndef QUERY_SERVER_H_
#define QUERY_SERVER_H_
#include <stdio.h>

#include "../Headers/worker.h"
#include "../Headers/heap.h"




/*** Queries ***/

char * diseaseFrequency( char * , HashTable *, Patient_list * );

Patient searchPatientRecord( char * , Patient_list * );

char * numPatientAdmissions( char * , HashTable * , Patient_list *);

void topCountries( char *, HashTable *, Patient_list * );

void recordPatientExit( char * , Patient_list * );

void numCurrentPAtients( char * , HashTable * );

void insertPatientRecord( char * , HashTable * , HashTable * , Patient_list * );

/********************/


/***** Queries Functions ****/

void dateTokenize( char *, Date *, Date *);

void open_manual();

Patient line_tokenize_without_exitDate( char * );

void df_tokenize ( char *, char **, char **, Date *, Date *);

void topK_tokenize(char * , char ** , Date * , Date *, int *);

char * patient_stringify( Patient );

unsigned int calculate_patient_chars( Patient );



/*********************/

#endif