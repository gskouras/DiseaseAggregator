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

void diseaseFrequency( char * );

void insertPAtientRecord( char * , HashTable * , HashTable * , Patient_list * );

/********************/


/***** Utillity Functions ****/

void dateTokenize( char *, Date *, Date *);

/*********************/
#endif