#ifndef HASH_H_
#define HASH_H_

#include <stdio.h>
#include <stdlib.h>
#include "../headers/tree.h"


typedef struct {
	char *string; /* string to hold the information we want */
	int age_ranges[4];
	int total_patients;
	int patients_hospitalized;
	Tree_Node * root; /* Pointer to the root of the AVL tree */
} BucketItem;


typedef struct Bucket
{
	unsigned long int  capacity; /* How many records a bucket can hold */
	int slot_counter; /* Counter to have knowledge of how many records currently are in this bucket */
	BucketItem * bucket_item; /* Array of BucketItems in this bucket */
	struct Bucket *next; /* Pointer to the next bucket */
} Bucket_Node;


typedef struct 
{
	Bucket_Node *head; /* Pointer to the head of this list */
	Bucket_Node *tail; /* Pointer to the tail of this list */
	int counter; /*Total amount of buckets this list has */
	unsigned long int capacity; /* How many records a bucket can hold */
} Bucket_List;


typedef struct
{
	Bucket_List *lists_of_buckets; /* each slot of the hash table holds a list structure of buckets */
	int size; /* size of current HastTable */
	int total_bucket_items; /* total number of different items that have been inserted to Hash Table */
	int bucket_capacity; /* How many records a bucket can hold */
} HashTable;



/**** Hash Table Constuctors ****/

void  initHashTable( HashTable * ht, int  , int );

void initBucketLists(Bucket_List * , int , int);

Bucket_Node * createNewBucketNode( int );

/**********************/


/**** Hash Table Functions ****/

void insert_to_hash_table(HashTable *, char * , Patient_Node * );

void print_hash_table(HashTable *);

int record_exist(char *, HashTable *);

void destroyHashTable(HashTable *);

/***************************/




/**** Hash Table List Functions ****/


/* Return 1 if a Record succesfully inserted to the bucket list or 0 if the Record already excisted thus the Record is inserted only to the AVL tree */
int  insert_to_bucket_list (Bucket_List * , char * , Patient_Node * );

/*Returns 0 if a Record successfully inserted a record to a bucket. Return 1 if it this record was te last record a bucket can hold thus a new a new bucket created*/
int insert_to_bucket(Bucket_Node * , char * , Patient_Node * );

/* Returns 1 if a Record excist in the bucket list thus the recorded is inserted only to the avl tree. Return 0 if requested records didnt found*/
int isExist( Bucket_List * , char * , Patient_Node * );

int getBucketSlotRecords(BucketItem *);

void print_bucket_list(Bucket_List * );

void freeBucketNodes(Bucket_List *);

/***************************/


/*** Others ***/

int hash_fun( char * string, int max_size); /* A simple Hash Function that assigns a string to a posision of the Hash Table */ 

void bucket_print(Bucket_Node *bucket);

/***************/


#endif
