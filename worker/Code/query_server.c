#include "../Headers/query_server.h"




/*** Query Server Functions ***/


char * diseaseFrequency( char * input, HashTable * disease_HT, Patient_list *list)
{

	Date d1, d2;
	char * disease;
	char * country;
	int counter = 0;

	int flag = 1, found = 0;//search for patients whos ENTRY date lies in given range

	char * result = NULL;

	df_tokenize(input, &disease, &country, &d1, &d2 );

	// printf("Country is : %s\n", country);
	// printf("Disease is %s\n",disease );
	// print_date(d1);
	// print_date(d2);

	if(record_exist(disease, disease_HT)) //find if given disease exist
	{			
		int index = hash_fun(disease, disease_HT->size);
		Bucket_Node *temp = disease_HT->lists_of_buckets[index].head;
		while(temp !=NULL)
		{
			for (int i = 0; i < temp->slot_counter; ++i)
			{
				if(strcmp(temp->bucket_item[i].string, disease))
				{
					continue;
				}
				else
				{
					if(country == NULL)
					{
						tree_search_dateRange( temp->bucket_item[i].root, d1, d2, &counter, flag);
						//printf("Disease %s has a total of %d incidents between ", temp->bucket_item[i].string, counter);
						// print_date(d1); print_date(d2); printf("\n");
						found = 1;
						break;
					}
					else
					{
						tree_search_Country_dateRange(temp->bucket_item[i].root, d1, d2, country, &counter, flag);
						//printf("Disease %s has a total of %d incidents between ", temp->bucket_item[i].string, counter);
						// print_date(d1); print_date(d2); printf(" in Country %s\n", country);
						found = 1;
						break;
					}
				}
			}
		temp = temp->next;
		}
	}


	if(!found)
	{
		result = malloc(sizeof(char)* 2);
		sprintf(result, "0");
		free(disease);
		free(country);
		return result;
	}
	
	if(country)
	{
		result = malloc(sizeof(char)* 10);
		//printf("Country is %s\n", country);
		sprintf(result, "%s : ", country);
		sprintf(result, "%s%d", result, counter);\
	}
	else
	{
		result = malloc(sizeof(char)* 10);
		sprintf(result, "%d", counter);
		//printf("NPA result is %s", result);
	}
	free(disease);
	free(country);
	return result;

}



char * numPatientAdmissions( char * input, HashTable * disease_HT, Patient_list *list, char * country, int flag)
{
	char *disease = NULL, *result = NULL, *temp_country = NULL;	

	Date d1, d2;
	int counter = 0;
	int found = 0;

	//printf("NumPAtient received your queryL::::::\n");
	char temp_input[50];
	strcpy(temp_input , input);

	df_tokenize(temp_input, &disease, &temp_country, &d1, &d2);


	// printf("Country is : %s\n", country);
	// printf("Disease is %s\n",disease );
	// print_date(d1);
	// print_date(d2);

	if(record_exist(disease, disease_HT)) //find if given disease exist
	{			
		int index = hash_fun(disease, disease_HT->size);
		Bucket_Node *temp = disease_HT->lists_of_buckets[index].head;
		while(temp !=NULL)
		{
			for (int i = 0; i < temp->slot_counter; ++i)
			{
				if(strcmp(temp->bucket_item[i].string, disease))
				{
					continue;
				}
				else
				{	
					if(country != NULL)
					{						
						//printf("beno na psakso xoris na dosei xora o xristis\n");
						tree_search_Country_dateRange(temp->bucket_item[i].root, d1, d2, country, &counter, flag);
						//printf("Disease %s has a total of %d incidents between ", temp->bucket_item[i].string, counter);
						// print_date(d1); print_date(d2); printf(" in Country %s\n", country);					
						found = 1;
						break;
					}
					else if(temp_country != NULL)
					{
						//printf("beno na psakso me xora pou edose o xristis\n");
						tree_search_Country_dateRange(temp->bucket_item[i].root, d1, d2, temp_country, &counter, flag);
						//printf("Disease %s has a total of %d incidents between ", temp->bucket_item[i].string, counter);
						// print_date(d1); print_date(d2); printf(" in Country %s\n", country);					
						found = 1;
					}
				}
			}
		temp = temp->next;
		}
	}

	//printf("Calculation over::: Counter is %d\n", counter);

	if(!found)
	{
		result = malloc(sizeof(char)* 2);
		sprintf(result, "0");
		free(disease);
		return result;
	}
	
	if(country)
	{
		result = malloc(sizeof(char)* 10);
		//printf("Country is %s\n", country);
		sprintf(result, "%s : ", country);
		sprintf(result, "%s%d", result, counter);
		free(disease);
		return result;
	}
	else if(temp_country)
	{
		result = malloc(sizeof(char)* 10);
		//printf("Country is %s\n", country);
		sprintf(result, "%s : ", temp_country);
		sprintf(result, "%s%d", result, counter);
		free(disease);
		free(temp_country);
		return result;		
	}

}


Patient searchPatientRecord(char * input, Patient_list * list)
{
	Patient_Node *temp = list->head;
	Patient dummy;
	dummy.recordID = 0;

	while(temp != NULL)
	{	
		if(strcmp(temp->patient.recordID, input)==0)
		{
			// printf("I eggrafi vrethike me stoixeia \n");
			// printPatientData(temp->patient);
			return temp->patient;
		}

		temp = temp->next;
	}
	return dummy;
}

char * patient_stringify(Patient patient)
{
	unsigned int message_length = calculate_patient_chars(patient);
	char *result = malloc(sizeof(char) * (message_length));
	sprintf(result, "%s", patient.recordID);
	sprintf(result, "%s %s", result, patient.firstName);
	sprintf(result, "%s %s", result, patient.lastName);
	sprintf(result, "%s %s", result, patient.diseaseID);
	sprintf(result, "%s %d", result, patient.age);
	sprintf(result, "%s %d", result, patient.entryDate.day);
	sprintf(result, "%s-%d", result, patient.entryDate.month);
	sprintf(result, "%s-%d", result, patient.entryDate.year);
	if(patient.exitDate.day > 0)
	{
		sprintf(result, "%s %d", result, patient.exitDate.day);
		sprintf(result, "%s-%d", result, patient.exitDate.month);
		sprintf(result, "%s-%d", result, patient.exitDate.year);
	}
	else
	{
		sprintf(result, "%s  --", result);
	}

	//printf("result is %s\n", result);
	return result;
}

unsigned int calculate_patient_chars( Patient patient)
{
	return strlen(patient.recordID) + strlen(patient.firstName) + strlen(patient.lastName) + strlen(patient.diseaseID) + 30; 
}


// void topDiseases( char * input, HashTable* disease_HT, Patient_list *list )
// {
// 	Date d1, d2;
// 	char * country;
// 	int k;
// 	topK_tokenize(input, &country, &d1, &d2, &k); //if no dates required, d1 is set to zero.

// 	int current_counter = 0; // counter to keep track of this bucket slot diseases
// 	int counter = 0;
//  	int prev_counter = 0;
//  	Max_Heap max_heap;
//  	initMaxHeap(&max_heap);

// 	if (d1.year ==0)
// 	{
// 		for (int i = 0; i < disease_HT->size; ++i)
// 		{	
// 			if( disease_HT->lists_of_buckets[i].head != NULL)
// 			{
// 				Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
// 				while(temp !=NULL)
// 				{
// 					for (int j = 0; j < temp->slot_counter; ++j)
// 					{
// 						tree_country_search(temp->bucket_item[j].root, country, &counter);

// 						if (counter > 0)
// 							heap_insert(&max_heap, counter,  temp->bucket_item[j].string);

// 						counter = 0;
// 					}
// 					temp = temp->next;
// 				}
// 			}
// 		}
		
// 		if (k > max_heap.total_records)
// 		{
// 			printf("Please enter a k between 1 and %d \n", max_heap.total_records);
// 			heap_destroy(max_heap.root);
// 			free(country);
// 			return;
// 		}

// 		printf("%s's most %d diseases are \n", country, k);
// 		for (int i = 0; i < k; ++i)
// 		{	
// 			printf("%s with %d infections\n", max_heap.root->record, max_heap.root->counter );
// 			heap_root_delete(&max_heap);
// 		}
// 	}
// 	else
// 	{
// 		for (int i = 0; i < disease_HT->size; ++i)
// 		{	
// 			if( disease_HT->lists_of_buckets[i].head != NULL)
// 			{
// 				Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
// 				while(temp !=NULL)
// 				{
// 					for (int j = 0; j < temp->slot_counter; ++j)
// 					{
// 						tree_search_Country_dateRange(temp->bucket_item[j].root, d1, d2, country, &counter);

// 						if (counter > 0)
// 							heap_insert(&max_heap, counter,  temp->bucket_item[j].string);

// 						counter = 0;
// 					}
// 					temp = temp->next;
// 				}
// 			}
// 		}
	
// 		if (k > max_heap.total_records)
// 		{
// 			printf("Please enter a k between 0 and %d \n", max_heap.total_records);
// 			heap_destroy(max_heap.root);
// 			free(country);
// 			return;
// 		}

// 		printf("%s's most %d diseases between", country, k);
// 		print_date(d1); print_date(d2); printf(" are\n");

// 		for (int i = 0; i < k; ++i)
// 		{	
// 			printf("%s with %d infections\n", max_heap.root->record, max_heap.root->counter );
// 			heap_root_delete(&max_heap);
// 		}

// 	}
// 	heap_destroy(max_heap.root);
// 	free(country);
// }


/********************/


/***** Utillity Functions ****/

void dateTokenize( char * input, Date *d1, Date *d2)
{	
	char * token = NULL;

 	token = strtok(input, "-");
    d1->day = atoi (token);

    token = strtok(NULL, "-");
    d1->month = atoi (token);


    token = strtok(NULL, " ");
    d1->year = atoi (token);


    token = strtok(NULL, "-");
    d2->day = atoi (token);


    token = strtok(NULL, "-");
    d2->month = atoi (token);


    token = strtok(NULL, "\n");
    d2->year = atoi (token);  
}

void df_tokenize (char *input, char ** disease_holder, char ** country_holder, Date *d1, Date *d2)	//printf(" disease holder is%s\n", *disease_holder );
{	
	if (strlen(input) <= 30) //tokenize without country name
	{
		char *token = strtok( input, " ");
	
 		*disease_holder = malloc(sizeof(char)*strlen(token)+1);
		strcpy(*disease_holder, token);
		token = strtok( NULL, "\n");	
		char *date_holder = malloc(sizeof(char)*strlen(token)+1);
		strcpy(date_holder, token);
		dateTokenize(date_holder , d1, d2);	
		*country_holder = NULL;
	}
 	else
	{
		char *token = strtok( input, " ");
	
 		*disease_holder = malloc(sizeof(char)*strlen(token)+1);
		strcpy(*disease_holder, token);
		token = strtok(NULL, "-");
        d1->day = atoi (token);

        token = strtok(NULL, "-");
        d1->month = atoi (token);

        token = strtok(NULL, " ");
        d1->year = atoi (token);

        token = strtok(NULL, "-");
        d2->day = atoi (token);

        token = strtok(NULL, "-");
        d2->month = atoi (token);

        token = strtok(NULL, " ");
        d2->year = atoi (token);

        token = strtok(NULL, "\n");
        *country_holder = malloc(sizeof(char)*strlen(token)+1);
		strcpy(*country_holder, token);
 	}
}

