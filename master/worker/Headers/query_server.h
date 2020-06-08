#ifndef QUERY_SERVER_H_
#define QUERY_SERVER_H_
#include <stdio.h>

#include "../Headers/worker.h"


/*** Queries ***/

char * diseaseFrequency( char * , HashTable *, Patient_list * );

char * topAgeRanges(char *input, HashTable * disease_HT, Patient_list *list);

Patient searchPatientRecord( char * , Patient_list * );

char * numPatientAdmissions( char * , HashTable * , Patient_list *, char *, int );

/********************/


/***** Queries Functions ****/

void dateTokenize( char *, Date *, Date *);

void df_tokenize ( char *, char **, char **, Date *, Date *);

void topK_tokenize(char * , char ** , char **, Date * , Date *, int *);

char * patient_stringify( Patient ); //converts a Patient into a string

unsigned int calculate_patient_chars( Patient ); //calculate how many charactes exist in a Patient Record 

int max_index(int *, int ); //find the index of a maximum element of the array

/*********************/

#endif