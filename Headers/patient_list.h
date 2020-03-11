#ifndef PATIENT_LIST_H
#define PATIENT_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
	int day;
	int month;
	int year;
} Date;

typedef struct
{
	int recordID;
	char * firstName;
	char * lastName;
	char * diseaseID;
	char * country;
	Date entryDate;
	Date exitDate; //date is set to one if a patients haven't take discharge from hospital yet
} Patient;

typedef struct Patient_Node
{
	Patient patient;
	struct Patient_Node *next;
} Patient_Node;

typedef struct 
{
	Patient_Node *head;
	Patient_Node *tail;
	int counter;
} Patient_list;


Patient_Node *createPatientNode( Patient );

Patient_list * initPatientList();

void insertNewPatient( Patient_list * , Patient );

void printPatientList( Patient_list * );

void printPatientData(Patient);

void freePatientList( Patient_list * );


#endif