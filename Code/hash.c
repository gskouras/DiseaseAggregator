#include "../Headers/hash.h"

/*** Constructors ***/

void  initHashTable( HashTable * ht, int size , int bucket_size)
{
	ht->lists_of_buckets = malloc(sizeof(Bucket_List) * size);
	ht->size = size;
	ht->bucket_capacity = (bucket_size - (sizeof(int)+sizeof(Bucket_Node*)+sizeof(BucketItem))) / sizeof(BucketItem);
	printf("Hash Table Buckets Capacity is %d\n",ht->bucket_capacity);
	ht->total_bucket_items = 0;

	initBucketLists(ht->lists_of_buckets, size, ht->bucket_capacity);
}


void initBucketLists(Bucket_List * lists_of_buckets, int size, int capacity)
{
	for (int i = 0; i < size; ++i)
	{
		lists_of_buckets[i].head = NULL;
		lists_of_buckets[i].tail = NULL;
		lists_of_buckets[i].counter = 0;
		lists_of_buckets[i].capacity = capacity;
	}

}

Bucket_Node * createNewBucketNode( int capacity )
{
	Bucket_Node *bucket = malloc(sizeof(Bucket_Node));
	bucket->next = NULL;
	bucket->slot_counter = 0;
	bucket->capacity = capacity;
	bucket->bucket_item = malloc(sizeof(BucketItem)* capacity);
	for (int i = 0; i < capacity; ++i)
	{
		//init avl tree of each bucket_slot
		bucket->bucket_item->root == NULL;
	}
	return bucket;
}

/*****************************************/

/*** Hash Table Functions ***/

void insert_to_hash_table(HashTable *ht, char * string, Patient_Node * this_patient)
{
	int index = hash_fun(string, ht->size);
	//printf("index of %s is %d\n", string, index );
	Bucket_List * target_list = &ht->lists_of_buckets[index];

	//printPatientData(this_patient->patient);		
	//printf("insert to hash table in insert hash table\n");
	insert_to_bucket_list(target_list, string, this_patient);
	ht->total_bucket_items++;		
}


void print_hash_table(HashTable * ht)
{
	printf("/**** Hash Table Data *****/\n");
	for (int i = 0; i < ht->size; ++i)
	{		
		printf("Data of raw %d\n", i);
		if( ht->lists_of_buckets[i].head != NULL)
		{
			print_bucket_list(&ht->lists_of_buckets[i]);
			printf("\n");

		}
		else
		{
			printf("IS EMPTY\n");
		}

	}
}

void destroyHashTable(HashTable * this_ht)
{	
	for (int i = 0; i < this_ht->size; ++i)
	{
		freeBucketNodes(&this_ht->lists_of_buckets[i]);
	}
	free(this_ht->lists_of_buckets);
}

/*****************************************/


/**** Hash Table List Functions ****/

int  insert_to_bucket_list (Bucket_List * this_list, char * string, Patient_Node * this_patient)
{
	//printf("This list Memory is %p\n\n", this_list );
	
	
	if (this_list->head == NULL)
	{
		//printf("this list counter == 0\n");
		Bucket_Node * new_bucket = createNewBucketNode(this_list->capacity);
		insert_to_bucket(new_bucket, string, this_patient);
		this_list->head = new_bucket;
		this_list->tail = new_bucket;
	}	
	else if(this_list->counter == 0)
	{
		if(isExist(this_list, string, this_patient))
			return 0;
		if (insert_to_bucket(this_list->head, string, this_patient))
		{
			//printf("insert to bucket list\n");
			this_list->tail= this_list->tail->next;
			this_list->counter++;
		}
		return 1;		
	}


	if(isExist(this_list, string, this_patient)) //de xriastike na kano insert afou i eggrafi iparxei idi, apla ekana to record eisagogi sto dentro
	{
		//printf("is isExist\n");
		return 0;
	}

	if (insert_to_bucket(this_list->tail, string, this_patient))
	{
		//printf("insert_to_bucket\n");
		this_list->tail= this_list->tail->next;
		this_list->counter++;
	}
}


int insert_to_bucket(Bucket_Node *this_bucket, char * string, Patient_Node * this_patient) //
{
	//printf("Memory of this bucket is  %p \n", this_bucket);
	strcpy(this_bucket->bucket_item[this_bucket->slot_counter].string, string);
	//printPatientData(this_patient->patient);
	//printf("%s and slot is %d\n", this_bucket->bucket_item[this_bucket->slot_counter].string, this_bucket->slot_counter);
	this_bucket->bucket_item[this_bucket->slot_counter].root = 
	tree_insert( this_bucket->bucket_item[this_bucket->slot_counter].root , this_patient->patient.entryDate, this_patient);//insert to avl tree
	this_bucket->slot_counter++;
	if (this_bucket->slot_counter == this_bucket->capacity)
	{
		//printf("beno?\n");
		this_bucket->next = createNewBucketNode(this_bucket->capacity);
		return 1;
	}
	return 0;
}


int isExist( Bucket_List * this_list, char * string , Patient_Node * this_patient)
{
	Bucket_Node *temp = this_list->head;
	while(temp != NULL)
	{
		for (int i = 0; i < temp->slot_counter; ++i)
		{
			if(!strcmp( string, temp->bucket_item[i].string)) //vrethike i eggrafi
			{
				//insert to avl
				temp->bucket_item[i].root =
				tree_insert( temp->bucket_item[i].root , this_patient->patient.entryDate, this_patient);//insert to avl tree
				return 1;
			}
		}
		temp= temp->next;
	}
	return 0;
}

void print_bucket_list(Bucket_List * this_list)
{
	Bucket_Node *temp = this_list->head;
	int i = 0;
	while (temp !=NULL)
	{	
		printf("In %d bucket is \n", i);
		bucket_print(temp);
		temp = temp->next;
		i++;
	}
}

void freeBucketNodes(Bucket_List * this_list)
{
	Bucket_Node *temp = this_list->head;
	while (this_list->head != NULL)
	{
		temp = this_list->head;
		this_list->head = this_list->head->next;
		for (int i = 0; i < temp->slot_counter; ++i)
		{
			tree_destroy(temp->bucket_item[i].root);	
		}
		free(temp->bucket_item);
		free(temp);
	}
}

/***************************/

/*** Others ***/

int hash_fun( char * string, int max_size)
{
	int length = strnlen(string, 30);
	int hash_value = 0;
	for (int i = 0; i < length; ++i)
	{
		hash_value += string[i];
		hash_value = (hash_value * string[i]) % max_size;
	}
	return hash_value;
}


void bucket_print(Bucket_Node *bucket)
{
	//printf("Bucket slot_counter is %d\n",  bucket->slot_counter);
	//printf(" this bucket slot_counter is %d \n", bucket->slot_counter);
	for (int i = 0; i < bucket->slot_counter; ++i)
	{
		printf("%s and patients record are : \n", bucket->bucket_item[i].string);
		//printf("Dates are :");
		tree_preorder_print(bucket->bucket_item[i].root);
		printf("\n");
	}
	printf("\n");
}


/********************/