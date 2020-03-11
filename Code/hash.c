#include "../Headers/hash.h"

/*** Constructors ***/

HashTable * initHashTable( int size )
{
	HashTable * this_ht = malloc(sizeof(HashTable));
	this_ht->buckets = malloc(sizeof(Bucket*)*size);
	for (int i = 0; i < size; ++i)
	{
		this_ht->buckets[i] = NULL;
	}
	return this_ht;
}

Bucket * createNewBucket( int b )
{
	Bucket *bucket = malloc(sizeof(Bucket));
	bucket->next = NULL;
	bucket->slot_counter = 0;

	bucket->bucket_size = b;

	bucket->max_slots = b-(sizeof(int)+sizeof(Bucket*)+sizeof(BucketItem));

	bucket->bucket_item = malloc(sizeof(BucketItem)* (bucket->max_slots));

	return bucket;
}

/*****************************************/

/*** Hash Table Functions ***/

int hash_table_insert( HashTable * ht, char * string, int table_size, int bucket_size)
{
	int index = hash_fun(string, table_size);
	//printf("index of %s is %d\n",string, index );
	Bucket * target_bucket = ht->buckets[index];

	if(target_bucket == NULL) //This means that this buckets is still free
	{
		Bucket *new_bucket = createNewBucket(bucket_size); //allocate memory for a new bucket
		ht->buckets[index] = new_bucket;
		printf("counter1 is :%d\n", new_bucket->slot_counter );
		new_bucket->bucket_item[new_bucket->slot_counter].string = string; //put the requested item in the slot counter posision and the increase that slot counter.
		printf("%s\n", new_bucket->bucket_item[new_bucket->slot_counter].string );
		new_bucket->slot_counter++;
		
	}
	else // This means that we have a collision
	{
		if( target_bucket->slot_counter < target_bucket-> max_slots) //if an available slot exsist in current bucket
		{
			target_bucket->bucket_item[target_bucket->slot_counter].string = string;
			printf("%s\n", target_bucket->bucket_item[target_bucket->slot_counter].string );
			printf("counter2 is :%d\n", target_bucket->slot_counter );
			target_bucket->slot_counter++;
		}
		else //if slots of current buckets are full
		{	
			Bucket *new_bucket = createNewBucket(bucket_size); //allocate memory for a new bucket

			while(target_bucket->next != NULL)
			{
				target_bucket = target_bucket->next;
				//printf("%s\n", new_bucket->bucket_item[new_bucket->slot_counter].string );
				
			}

			new_bucket->bucket_item[new_bucket->slot_counter].string = string;
			printf("%s\n", new_bucket->bucket_item[new_bucket->slot_counter].string );
			printf(" counter3 is :%d\n", new_bucket->slot_counter );
			new_bucket->slot_counter++;
			target_bucket->next = new_bucket;
		}
	}
	return 1;

}



void hash_table_print( HashTable *ht, int size)
{
	for (int i = 0; i < size; ++i)
	{	
		printf("Data of %d raw of hashtable is ", i);
		if( ht->buckets[i] != NULL)
			bucket_print(ht->buckets[i]);  
		else
			printf("NULL\n");
	}
}


/*** Others ***/

int hash_fun( char * string, int max_size)
{
	int length = strlen(string);
	int hash_value = 0;

	for (int i = 0; i < length; ++i)
	{
		hash_value += string[i];
		hash_value = (hash_value * string[i] % (max_size));
	}
	return hash_value;

}

void bucket_print(Bucket *bucket)
{
	Bucket *temp = bucket;
	while(temp!= NULL)
	{
		for (int i = 0; i < bucket->slot_counter; ++i)
		{
			printf("%s", temp->bucket_item[i].string);
			printf("\n");
		}
		temp = temp->next;
	}
}

/********************/