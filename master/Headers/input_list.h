#ifndef INPUT_LIST_H_
#define INPUT_LIST_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>




typedef struct CountryPath_Node
{
	char country_path[50];
	struct CountryPath_Node *next;
} CountryPath_Node;

typedef struct 
{
	CountryPath_Node *head;
	CountryPath_Node *tail;
	int counter; /* Counts the Patient Nodes of Patient List */
} Directory_list;


/***** Constructors *****/

CountryPath_Node *createPathNode( char * );

void initDirectorytList(Directory_list *);

/****************************/

/**** Date List Functions *****/
void insertNewDirectory( Directory_list * , char * );

CountryPath_Node * get_directoryList_head( Directory_list * );

void printDirectoryList( Directory_list * );

CountryPath_Node* get_country(Directory_list *,  int );

void freeDirList( Directory_list * );

/****************************/

#endif


