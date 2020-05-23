#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Heap_Node
{
	char record[15];
	int counter;
	struct Heap_Node * parent;
	struct Heap_Node * left;
	struct Heap_Node * right;
	struct Heap_Node * prevTail;
} Heap_Node;


typedef struct 
{
	Heap_Node * root;
	Heap_Node *tail;
	int total_records;
} Max_Heap;


/*** Constructors ***/

void initMaxHeap( Max_Heap * );

Heap_Node * createHeapNode( int , char * );

/***********************/

/*** Max Heap Function ***/

void heap_insert( Max_Heap *,  int , char * );

void heap_root_delete( Max_Heap * );

void maxHeapify( Heap_Node * );

void revMaxHeapify( Heap_Node * );

void heap_print( Heap_Node * );

void heap_destroy( Heap_Node * );

/***********************/

/*** Utillity Functions ***/

void setTail( Max_Heap *, Heap_Node * );

void swapHeapNodeData( Heap_Node * , Heap_Node * );

int getMaximum( Max_Heap * );


/*****************************/