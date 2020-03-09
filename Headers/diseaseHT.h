#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "main.h"


typedef struct 
{
	char * patient.diseaseID;
	disease_tree_node * d_tree_node;
} Disease_bucket_item;

typedef struct Disease_bucket
{
	int size;
	int capacity;
	char * patient.diseaseID;
	struct Disease_bucket *next;

} Disease_bucket;

typedef struct 
{
	Disease_bucket **d_bucket;
} D_HT;



