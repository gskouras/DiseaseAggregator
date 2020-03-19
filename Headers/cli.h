#ifndef CLI_H_
#define CLI_H_
#include <stdio.h>

#include "../Headers/main.h"

/*** CLI commands ***/

void globalDiseaseStats( char *, HashTable * );

void topDiseases( char * );

void topCountries( char * );

void recordPatientExit( char * , Patient_list * );

void numCurrentPAtients( char * , HashTable * );

void diseaseFrequency( char * , HashTable *, HashTable *, Patient_list * );

void insertPatientRecord( char * , HashTable * , HashTable * , Patient_list * );

/********************/


/***** Utillity Functions ****/

void dateTokenize( char *, Date *, Date *);

void open_manual();

Patient line_tokenize_without_exitDate( char * );

void df_tokenize ( char *, char **, char **, Date *, Date *);

/*********************/
#endif