#ifndef HASH_H_
#define HASH_H_

#include <stdio.h>
#include <stdlib.h>
#include "../Headers/patient_list.h"


typedef struct TreeNode
{
	Date date;
	struct TreeNode *left;
	struct TreeNode *right;
} TreeNode;

typedef struct {
	char * string;
	TreeNode * tree_ptr;
} BucketItem;


typedef struct Bucket
{
	int bucket_size;
	int max_slots;
	int slot_counter;
	BucketItem * bucket_item;
	struct Bucket *next;
} Bucket;

typedef struct
{
	Bucket ** buckets;
} HashTable;



/**** Hash Table Constuctors ****/

HashTable * initHashTable( int );

Bucket * createNewBucket( int );

/**********************/


/**** Hash Table Functions ****/

int hash_table_insert( HashTable * , char * , int, int);

void hash_table_print( HashTable * , int);

/***************************/





/*** Others ***/

int hash_fun( char * , int );

void bucket_print(Bucket *);

/***************/


#endif
