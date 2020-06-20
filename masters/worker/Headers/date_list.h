#ifndef DATE_LIST_H
#define DATE_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include "../Headers/patient_list.h"


typedef struct Date_Node
{
	struct Date_Node * next;
	Patient_Node *patient_node;
} Date_Node;

typedef struct 
{
	Date_Node *head;
	Date_Node *tail;
	int counter;
}Date_List;


/*** Constructors ***/

Date_Node *createDateNode();

void initDateList( Date_List * );

/****************************/

/**** Utilty Functions of Date List ***/

void insertNewDate( Date_List * , Patient_Node * );

void printDatetList( Date_List * );

void freeDateList( Date_List *  );

/****************************/


#endif