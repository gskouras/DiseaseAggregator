#include "../Headers/query_server.h"


char * query_handler(char * message, HashTable * disease_HT, HashTable * country_HT, Patient_list *list, int write_fd)
{

    	char *input = NULL, *cmd = NULL;

        input = message;
        cmd = strtok_r(input, " \n", &input);
        input = strtok(input, "\n");

	if (cmd != NULL) 
	{

		if (strcmp(cmd, "/diseaseFrequency") == 0 || strcmp(cmd, "/df") == 0) 
	    {
	    	return diseaseFrequency(input, disease_HT, list); //done
	    } 
	    else if (strcmp(cmd, "/topk-AgeRanges") == 0 || strcmp(cmd, "/tka") == 0 ) 
	    {
	    	printf("Beno tka\n");
	    } 
	    else if (strcmp(cmd, "/searchPatientRecord") == 0 || strcmp(cmd, "/spr") == 0)
	    {
			Patient patient = searchPatientRecord(input, list);
			if (patient.recordID == 0)
			{
				char * result = malloc(sizeof(char)* 2);
				strcpy(result, "0");
				return result;
			}
			else
			{
				char * result = patient_stringify(patient);	
				return  result;
			}	
			//printf("result before return is %s\n", result);

			
	    } 
	    else if (strcmp(cmd, "/numPatientAdmissions") == 0 || strcmp(cmd, "/npa") == 0) 
	    {
	    	printf("Beno npa\n");
	    } 
	    else if (strcmp(cmd, "/numPatientDischarges") == 0 || strcmp(cmd, "/npd")==0 ) 
	    {
	    	printf("Beno npd\n");

	    } 
	}
}

/*** Query Server Functions ***/


char * diseaseFrequency( char * input, HashTable * disease_HT, Patient_list *list)
{

	Date d1, d2;
	char * disease;
	char * country;
	int counter = 0;

	char * result = NULL;

	df_tokenize(input, &disease, &country, &d1, &d2 );

	// print_date(d1);
	// printf("\n");
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
						tree_search_dateRange( temp->bucket_item[i].root, d1, d2, &counter);
						//printf("Disease %s has a total of %d incidents between ", temp->bucket_item[i].string, counter);
						// print_date(d1); print_date(d2); printf("\n");
						break;
					}
					else
					{
						tree_search_Country_dateRange(temp->bucket_item[i].root, d1, d2, country, &counter);
						//printf("Disease %s has a total of %d incidents between ", temp->bucket_item[i].string, counter);
						// print_date(d1); print_date(d2); printf(" in Country %s\n", country);
						break;
					}
				}
			}
		temp = temp->next;
		}
	}
	else
	{
		//printf("No such disease found in our records!\n");
		free(disease);
		free(country);
		return NULL;
	}
	free(disease);
	free(country);
	result = malloc(sizeof(char)* 10);
	sprintf(result, "%d", counter);
	return result;
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

// void topCountries( char * input , HashTable* country_HT, Patient_list *list)
// {
// 	Date d1, d2;
// 	char * disease;
// 	int k;
// 	topK_tokenize(input, &disease, &d1, &d2, &k);


// 	int current_counter = 0; // counter to keep track of this bucket slot diseases
// 	int counter = 0;
//  	int prev_counter = 0;
//  	Max_Heap max_heap;
//  	initMaxHeap(&max_heap);

// 	if (d1.year ==0)
// 	{
// 		for (int i = 0; i < country_HT->size; ++i)
// 		{	
// 			if( country_HT->lists_of_buckets[i].head != NULL)
// 			{
// 				Bucket_Node *temp = country_HT->lists_of_buckets[i].head;
// 				while(temp !=NULL)
// 				{
// 					for (int j = 0; j < temp->slot_counter; ++j)
// 					{
// 						tree_disease_search(temp->bucket_item[j].root, disease, &counter);

// 						if (counter > 0)
// 							heap_insert(&max_heap, counter,  temp->bucket_item[j].string);
						
// 					}
// 					temp = temp->next;
// 				}
// 			}
// 		}
	
// 		if (k > max_heap.total_records -1)
// 		{
// 			printf("Please enter a k between 0 and %d \n", max_heap.total_records-1);
// 			heap_destroy(max_heap.root);
// 			free(disease);
// 			return;
// 		}

// 		printf("%s's most %d infected countries are \n", disease, k);
// 		for (int i = 0; i < k; ++i)
// 		{	
// 			printf("%s with %d infections\n", max_heap.root->record, max_heap.root->counter );
// 			heap_root_delete(&max_heap);
// 		}
// 	}
// 	else
// 	{
// 		for (int i = 0; i < country_HT->size; ++i)
// 		{	
// 			if( country_HT->lists_of_buckets[i].head != NULL)
// 			{
// 				Bucket_Node *temp = country_HT->lists_of_buckets[i].head;
// 				while(temp !=NULL)
// 				{
// 					for (int j = 0; j < temp->slot_counter; ++j)
// 					{
// 						tree_search_Disease_dateRange(temp->bucket_item[j].root, d1, d2, disease, &counter);
// 						current_counter = counter - prev_counter;
// 						prev_counter = counter;

// 						if (counter > 0)
// 							heap_insert(&max_heap, counter,  temp->bucket_item[j].string);
						
// 						counter = 0;
// 						current_counter = 0; // counter to keep track of this bucket slot diseases
// 						prev_counter = 0; //counter to keep track of the amount of previus diseases
// 					}
// 					temp = temp->next;
// 				}
// 			}
// 		}
	
// 		if (k > max_heap.total_records)
// 		{
// 			printf("Please enter a k between 0 and %d \n", max_heap.total_records);
// 			heap_destroy(max_heap.root);
// 			free(disease);
// 			return;
// 		}

// 		printf("%s's most %d infected countries between", disease, k);
// 		print_date(d1); print_date(d2); printf(" are\n");

// 		for (int i = 0; i < k; ++i)
// 		{	
// 			printf("%s with %d incidents\n", max_heap.root->record, max_heap.root->counter );
// 			heap_root_delete(&max_heap);
// 		}
// 	}
// 	heap_destroy(max_heap.root);
// 	free(disease);
// }

// void recordPatientExit( char * input , Patient_list *list)
// {
// 	if(input != NULL)
// 	{
// 		Date date;
// 		char * token = NULL;

// 		token = strtok(input, " ");
// 		char * id = malloc(sizeof(char)* strlen(token)+1);
//         strcpy(id, token);


// 	 	token = strtok(NULL, "-");
// 	    date.day = atoi (token);
// 	    //printf("_%d_\n", date.day);

// 	    token = strtok(NULL, "-");
// 	    date.month = atoi (token);
// 	    //printf("_%d_\n", date.month);

// 	    token = strtok(NULL, "\n");
// 	    date.year = atoi (token);
// 	    if(id_exist(list, id))
// 	    {
// 			Patient_Node * this = list_date_modify(list, date,  id);
// 		    printf("Exit date of Patient with recordID %s updated!", id );
// 		    printf(" New patient details are : ");
// 		    printPatientData(this->patient);
// 		    printf("\n");
// 	    }
// 	    else
// 	    {
// 	    	printf("No patient with id %s found!\n",id );
// 	    }

// 	free(id);
// 	}
// 	else
// 	{
// 		printf("Enter Valid Data\n");
// 	}
// }


// void numCurrentPAtients( char * input, HashTable * disease_HT)
// {
// 	if(input != NULL)
// 	{
// 		int found = 0;
// 		char *token;
// 		token = strtok(input, "\n");
// 		int index = hash_fun(input, disease_HT->size);
// 		Bucket_Node *temp = disease_HT->lists_of_buckets[index].head;
// 		while(temp !=NULL)
// 		{
// 			for (int i = 0; i < temp->slot_counter; ++i)
// 			{
// 				if(strcmp(temp->bucket_item[i].string, token))
// 				{
// 					continue;
// 				}
// 				else
// 				{
// 					printf("Disease %s has a total of %d patients still hospitalized \n", temp->bucket_item[i].string, temp->bucket_item[i].patients_hospitalized);
// 					found = 1;
// 					break;
// 				}
// 			}
// 			temp = temp->next;

// 		}
// 		if(!found)
// 			printf("No Patients with %s disease found in our hospital records \n", token);
// 	}
// 	else
// 	{
// 		int counter;
// 		for (int i = 0; i < disease_HT->size; ++i)
// 		{	
// 			if( disease_HT->lists_of_buckets[i].head != NULL)
// 			{
// 				Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
// 				while(temp !=NULL)
// 				{
// 					for (int i = 0; i < temp->slot_counter; ++i)
// 					{	
// 						printf("Disease %s has a total of %d patients still hospitalized \n", temp->bucket_item[i].string, temp->bucket_item[i].patients_hospitalized);
// 					}
// 					temp = temp->next;

// 				}
// 			}
// 		}
// 	}
// }





// void insertPatientRecord( char * input, HashTable * disease_HT, HashTable *country_HT, Patient_list *list)
// {	
// 	if (input == NULL)
// 	{
// 		printf("Please Insert Valid Data\n");
// 		return;
// 	}

// 	Patient patient_attributes;

// 	if (strlen(input) < 48)
// 		patient_attributes = line_tokenize_without_exitDate(input);
// 	else
// 		patient_attributes = line_tokenize(input, patient_attributes);

// 	Patient_Node * new_patient_node = NULL;

// 	if(patient_attributes.entryDate.day == 0)
// 	{
// 	 	printf("First Date must be smaller than second Date\n");
// 	 	return;
// 	}	

//     if (id_exist(list, patient_attributes.recordID) )
//     {
//         printf("Patient Record ID %s already exist!\n", patient_attributes.recordID);
//         return;
//     }

//     new_patient_node =  insertNewPatient(list, patient_attributes);
//     insert_to_hash_table(disease_HT, patient_attributes.diseaseID, new_patient_node);
//     insert_to_hash_table(country_HT, patient_attributes.country, new_patient_node);

//     printf("New patient with following attributes succesfully inserted\n"); 
//     printf("recordID : %s\n", patient_attributes.recordID);
//     printf("FirstName : %s\n", patient_attributes.firstName);
//     printf("LastName : %s\n", patient_attributes.lastName);
//     printf("diseaseID : %s\n", patient_attributes.diseaseID);
//     printf("Country : %s\n", patient_attributes.country);
//     printf("Entry date : ");
//     print_date(patient_attributes.entryDate);
//     printf("\n");

//     if(patient_attributes.exitDate.year > 1)
//     {
//     	printf("exitDate: ");
//     	print_date(patient_attributes.exitDate);
//     	printf("\n");
//     }
// }

// /********************/


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

// Patient line_tokenize_without_exitDate(char * input)
// {
// 	Patient patient;
// 	char * token;

//     token = strtok(input, " ");
// 	patient.recordID = malloc(sizeof(char)* strlen(token)+1);
// 	strcpy(patient.recordID, token);
//     if(atoi(token) == 0)
//     {
//     	printf("Please insert a Patient ID\n");
//     	return patient;
//     }

//     token = strtok(NULL, " ");
//     patient.firstName = malloc(sizeof(char)*strlen(token)+1);
//     strcpy( patient.firstName, token);

//     token = strtok(NULL, " ");
//     patient.lastName = malloc(sizeof(char)*strlen(token)+1);
//     strcpy( patient.lastName, token);

//     token = strtok(NULL, " ");
//     patient.diseaseID = malloc(sizeof(char)*strlen(token)+1);
//     strcpy( patient.diseaseID, token);

//     token = strtok(NULL, " ");
//     patient.country = malloc(sizeof(char)*strlen(token)+1);
//     strcpy( patient.country, token);

//     token = strtok(NULL, "-");
//     patient.entryDate.day = atoi (token);

//     token = strtok(NULL, "-");
//     patient.entryDate.month = atoi (token);

//     token = strtok(NULL, "\n");
//     patient.entryDate.year = atoi (token);
    
// 	patient.exitDate.day = 0;
//     patient.exitDate.month = 0;
//     patient.exitDate.year = 0;
//     return patient;
// }


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


// void topK_tokenize(char * input, char ** string, Date * d1, Date *d2, int *k)
// {
// 	char *token;
// 	if(input != NULL)
// 	{
// 		if ( strlen(input)< 15)
// 		{
// 			token = strtok(input, " ");
// 			*k = atoi(token); // number of incidents

// 			token = strtok(NULL, "\n");
// 			*string = malloc(sizeof(char)*strlen(token)+1);
// 			strcpy(*string, token);
// 			d1->day = 0;
// 			d1->month = 0;
// 			d1->year = 0;
// 		}
// 		else
// 		{
// 			token = strtok(input, " ");
// 			*k = atoi(token); // number of incidents

// 			token = strtok(NULL, " ");
// 			*string = malloc(sizeof(char)*strlen(token)+1);
// 			strcpy(*string, token);
// 			token = strtok( NULL , "\n");
// 			dateTokenize(token, d1, d2);
// 		}
// 	}
// 	else
// 	{
// 		printf("Please Enter Valid Data");
// 	}	
// }

// /*********************/