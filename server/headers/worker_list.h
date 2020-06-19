#ifndef WORKER_LIST_H_
#define WORKER_LIST_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



typedef struct Worker_Node
{
	char ** countries;
	int portNum;
	int country_count;
	char *ip;
	struct Worker_Node *next;
} Worker_Node;

typedef struct 
{
	Worker_Node *head;
	Worker_Node *tail;
	int counter; /* Counts the Patient Nodes of Patient List */
} Worker_list;


/***** Constructors *****/

Worker_Node *createWorkerNode( char ** , int, int, char * );

void initWorkertList(Worker_list *);

/****************************/

/**** Date List Functions *****/
void insertNewWorker( Worker_list * , char ** , int, int, char *);

void printWorkerList( Worker_list * );

Worker_Node* get_worker(Worker_list *,  int );

void freeWorkerList( Worker_list * );

/****************************/

#endif


