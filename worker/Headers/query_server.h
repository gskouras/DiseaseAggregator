#ifndef QUERY_SERVER_H_
#define QUERY_SERVER_H_
#include <stdio.h>

#include "../headers/worker.h"
#include "../Headers/heap.h"


/*** Queries ***/

char * diseaseFrequency( char * , HashTable *, Patient_list * );

Patient searchPatientRecord( char * , Patient_list * );

char * numPatientAdmissions( char * , HashTable * , Patient_list *, char *, int );


/********************/


/***** Queries Functions ****/

void dateTokenize( char *, Date *, Date *);

void df_tokenize ( char *, char **, char **, Date *, Date *);

char * patient_stringify( Patient );

unsigned int calculate_patient_chars( Patient );




/*********************/

#endif