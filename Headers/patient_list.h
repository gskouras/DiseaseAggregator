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
	char firstName[20];
	char lastName[20];
	char diseaseID[20];
	char country[20];
	Date entryDate;
	Date exitDate; //date is set to 1 if a patients haven't take discharge from hospital yet
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
	int counter; /* Counts the Patient Nodes of Patient List */
} Patient_list;


/***** Constructors *****/

Patient_Node *createPatientNode( Patient );

void initPatientList(Patient_list *);

/****************************/

/**** Date List Functions *****/
Patient_Node * insertNewPatient( Patient_list * , Patient );

void printPatientList( Patient_list * );

void printPatientData(Patient);

void freePatientList( Patient_list * );

/****************************/
#endif