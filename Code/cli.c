#include "../Headers/cli.h"



void cli(HashTable * disease_HT, HashTable * country_HT, Patient_list *list)
{
	int fd;
    char *input = NULL, *line = NULL, *cmd = NULL;
    size_t len = 0;
    ssize_t read;

	printf("\t\t\t##########  WELCOME TO DISEASE MONITOR COMMAND LINE INTERFACE  ##########\n\n"); 
	printf("You can use the available commands listed in the manual page. Type ./man to see the manual page or type ./exit to quit\n\n");
    putchar('>');

	while ((read = getline(&line, &len, stdin)) != EOF) 
	{
		input = line;
		cmd = strtok_r(input, " \n", &input);
		input = strtok(input, "\n");

		if (cmd != NULL) 
		{
		    if (strcmp(cmd, "./globalDiseaseStats") == 0 || strcmp(cmd, "./gl") == 0) 
		    {
		    	if (input != NULL && strlen(input) < 19)
		    	{
		    		printf("If you Enter a Date you must also enter another \n");
		    		putchar('>');
		    		continue;
		    	}
		    	else
		    	{
		        	globalDiseaseStats(input, disease_HT); //done
		    	}
		    } 
		    else if (strcmp(cmd, "./topk-Diseases") == 0 || strcmp(cmd, "./tkd") == 0) 
		    {
		    	if (input != NULL && (strlen(input) < 24  && strlen(input) > 12))
		    	{
		    		printf("If you Enter a Date you must also enter another \n");
		    		putchar('>');
		    		continue;
		    	}
		    	else
		    	{
		        	topDiseases(input, disease_HT, list);	        	
		    	}

		    } 
		    else if (strcmp(cmd, "./topk-Countries") == 0 || strcmp(cmd, "./tkc") == 0 ) 
		    {

		        topCountries(input, country_HT, list);

		    } 
		    else if (strcmp(cmd, "./recordPatientExit") == 0 || strcmp(cmd, "./rpe") == 0) 
		    {
		    	if (input != NULL && strlen(input) < 10)
		    	{
		    		printf("If you Enter a Date you must also enter another \n");
		    		putchar('>');
		    		continue;
		    	}
		    	else
		    	{
		    		recordPatientExit(input, list); //done
		    	}
		    } 
		    else if (strcmp(cmd, "./numCurrentPatients") == 0 || strcmp(cmd, "./ncp") == 0) 
		    {

		        numCurrentPAtients(input, disease_HT); //done

		    } 
		    else if (strcmp(cmd, "./diseaseFrequency") == 0 || strcmp(cmd, "./df") == 0) 
		    {
				if (input != NULL && strlen(input) < 10)
		    	{
		    		printf("Please Insert valid Data\n");
		    		putchar('>');
		    		continue;
		    	}
		    	else 
		    	{
		        	diseaseFrequency(input, disease_HT, list);
		    	}

		    } 
		    else if (strcmp(cmd, "./insertPatientRecord") == 0 || strcmp(cmd, "./ipr") == 0) 
		    {	
		    	if (input != NULL && strlen(input) < 30)
		    	{
		    		printf("Please Insert valid Data\n");
		    		putchar('>');
		    		continue;
		    	}
		    	else 
		    	{
		    		insertPatientRecord(input, disease_HT, country_HT, list); //done
		    	}
		        

		    } 
		    else if (strcmp(cmd, "./man") == 0)
		    {
		    	open_manual();
		    }
		    else if (strcmp(cmd, "./exit") == 0) 
		    {
		    	printf("\nExiting Monitor Disease..\n\n"); //Done
		    	free(line);
		    	return;
		    } 
		    else 
		    {
		        fprintf(stdout, "~ error: %s: command not found!\n", cmd);
		    }
		    
		}
		
		putchar('>');
		putchar(' ');
	}
}


/*** CLI commands ***/

void open_manual()
{
	FILE *fptr; 
  
    char filename[100], c; 
  
    // Open file 
    fptr = fopen("./resources/manual.txt", "r"); 
    if (fptr == NULL) 
    { 
        printf("Cannot open file at this time \n"); 
        return;
    } 
  
    // Read contents from file 
    c = fgetc(fptr); 
    while (c != EOF) 
    { 
      /* no timeout */
        printf ("%c", c); 
       	c = fgetc(fptr); 
    } 

    printf("\n\n");
    printf("\t\t\t\t\t\t\tPress ENTER to Continue\n");  
    getchar(); 
    return;  
}

void globalDiseaseStats( char * input, HashTable * disease_HT )
{
	if(input != NULL)
	{
		int current_counter = 0; // counter to keep track of this bucket slot diseases
		int counter = 0;
 		int prev_counter = 0;
		Date d1, d2;
		dateTokenize(input, &d1, &d2);

		if(date_cmp(d1, d2) == 1) //date 2 is bigger	
		{
			printf("First Date must be smaller than second Date\n");
			return;
		}

		for (int i = 0; i < disease_HT->size; ++i)
		{	
			
			if( disease_HT->lists_of_buckets[i].head != NULL)
			{
				Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
				while(temp !=NULL)
				{					 
					for (int j = 0; j < temp->slot_counter; ++j)
					{
						tree_search_dateRange( temp->bucket_item[j].root, d1, d2, &counter);
						current_counter = counter - prev_counter;
						prev_counter = counter;
						printf("Disease %s has a total of %d incidents between ", temp->bucket_item[j].string, current_counter);
						print_date(d1); print_date(d2); printf("\n");
						int counter = 0;
						int current_counter = 0; // counter to keep track of this bucket slot diseases
						int prev_counter = 0; //counter to keep track of the amount of previus diseases
					}
					temp = temp->next;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < disease_HT->size; ++i)
		{	
			if( disease_HT->lists_of_buckets[i].head != NULL)
			{
				Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
				while(temp !=NULL)
				{
					for (int j = 0; j < temp->slot_counter; ++j)
					{
						printf("Disease %s has a total of %d incidents \n", temp->bucket_item[j].string, getBucketSlotRecords(&temp->bucket_item[j]));
					}
					temp = temp->next;

				}
			}
		}
	}

}

void topDiseases( char * input, HashTable* disease_HT, Patient_list *list )
{
	Date d1, d2;
	char * country;
	int k;
	topK_tokenize(input, &country, &d1, &d2, &k); //if no dates required, d1 is set to zero.

	int current_counter = 0; // counter to keep track of this bucket slot diseases
	int counter = 0;
 	int prev_counter = 0;
 	Max_Heap max_heap;
 	initMaxHeap(&max_heap);

	if (d1.year ==0)
	{
		for (int i = 0; i < disease_HT->size; ++i)
		{	
			if( disease_HT->lists_of_buckets[i].head != NULL)
			{
				Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
				while(temp !=NULL)
				{
					for (int j = 0; j < temp->slot_counter; ++j)
					{
						tree_country_search(temp->bucket_item[j].root, country, &counter);
						current_counter = counter - prev_counter;
						prev_counter = counter;
						heap_insert(&max_heap, counter,  temp->bucket_item[j].string);
						//printf("Disease %s had a total of %d incidents in country %s\n", temp->bucket_item[j].string, counter, country);
						counter = 0;
						current_counter = 0; // counter to keep track of this bucket slot diseases
						prev_counter = 0; //counter to keep track of the amount of previus diseases
					}
					temp = temp->next;
				}
			}
		}
	
		if (k > max_heap.total_records)
		{
			printf("Please enter a k between 0 and %d \n", max_heap.total_records);
			return;
		}

		printf("%s's most %d diseases are \n", country, k);
		for (int i = 0; i < k; ++i)
		{	
			printf("%s with %d incidents\n", max_heap.root->record, max_heap.root->counter );
			heap_root_delete(&max_heap);
		}
	}
	else
	{
		for (int i = 0; i < disease_HT->size; ++i)
		{	
			if( disease_HT->lists_of_buckets[i].head != NULL)
			{
				Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
				while(temp !=NULL)
				{
					for (int j = 0; j < temp->slot_counter; ++j)
					{
						tree_search_Country_dateRange(temp->bucket_item[j].root, d1, d2, country, &counter);
						current_counter = counter - prev_counter;
						prev_counter = counter;
						heap_insert(&max_heap, counter,  temp->bucket_item[j].string);
						//printf("Disease %s had a total of %d incidents in country %s\n", temp->bucket_item[j].string, counter, country);
						counter = 0;
						current_counter = 0; // counter to keep track of this bucket slot diseases
						prev_counter = 0; //counter to keep track of the amount of previus diseases
					}
					temp = temp->next;
				}
			}
		}
	
		if (k > max_heap.total_records)
		{
			printf("Please enter a k between 0 and %d \n", max_heap.total_records);
			return;
		}

		printf("%s's most %d diseases in Date Range between", country, k);
		print_date(d1); print_date(d2); printf(" are\n");

		for (int i = 0; i < k; ++i)
		{	
			printf("%s with %d incidents\n", max_heap.root->record, max_heap.root->counter );
			heap_root_delete(&max_heap);
		}

	}
	heap_destroy(max_heap.root);
	free(country);
}

void topCountries( char * input , HashTable* country_HT, Patient_list *list)
{
	Date d1, d2;
	char * disease;
	int k;
	topK_tokenize(input, &disease, &d1, &d2, &k);


	int current_counter = 0; // counter to keep track of this bucket slot diseases
	int counter = 0;
 	int prev_counter = 0;
 	Max_Heap max_heap;
 	initMaxHeap(&max_heap);

	if (d1.year ==0)
	{
		for (int i = 0; i < country_HT->size; ++i)
		{	
			if( country_HT->lists_of_buckets[i].head != NULL)
			{
				Bucket_Node *temp = country_HT->lists_of_buckets[i].head;
				while(temp !=NULL)
				{
					for (int j = 0; j < temp->slot_counter; ++j)
					{
						tree_disease_search(temp->bucket_item[j].root, disease, &counter);
						current_counter = counter - prev_counter;
						prev_counter = counter;
						heap_insert(&max_heap, counter,  temp->bucket_item[j].string);
						//printf("Disease %s had a total of %d incidents in country %s\n", temp->bucket_item[j].string, counter, country);
						counter = 0;
						current_counter = 0; // counter to keep track of this bucket slot diseases
						prev_counter = 0; //counter to keep track of the amount of previus diseases
					}
					temp = temp->next;
				}
			}
		}
	
		if (k > max_heap.total_records)
		{
			printf("Please enter a k between 0 and %d \n", max_heap.total_records);
			return;
		}

		printf("%s's most %d countries infected are \n", disease, k);
		for (int i = 0; i < k; ++i)
		{	
			printf("%s with %d incidents\n", max_heap.root->record, max_heap.root->counter );
			heap_root_delete(&max_heap);
		}
	}
	else
	{
		for (int i = 0; i < country_HT->size; ++i)
		{	
			if( country_HT->lists_of_buckets[i].head != NULL)
			{
				Bucket_Node *temp = country_HT->lists_of_buckets[i].head;
				while(temp !=NULL)
				{
					for (int j = 0; j < temp->slot_counter; ++j)
					{
						tree_search_Disease_dateRange(temp->bucket_item[j].root, d1, d2, disease, &counter);
						current_counter = counter - prev_counter;
						prev_counter = counter;
						heap_insert(&max_heap, counter,  temp->bucket_item[j].string);
						//printf("Disease %s had a total of %d incidents in country %s\n", temp->bucket_item[j].string, counter, country);
						counter = 0;
						current_counter = 0; // counter to keep track of this bucket slot diseases
						prev_counter = 0; //counter to keep track of the amount of previus diseases
					}
					temp = temp->next;
				}
			}
		}
	
		if (k > max_heap.total_records)
		{
			printf("Please enter a k between 0 and %d \n", max_heap.total_records);
			return;
		}

		printf("%s's most %d infected countries in Date Range between", disease, k);
		print_date(d1); print_date(d2); printf(" are\n");

		for (int i = 0; i < k; ++i)
		{	
			printf("%s with %d incidents\n", max_heap.root->record, max_heap.root->counter );
			heap_root_delete(&max_heap);
		}
	}
	heap_destroy(max_heap.root);
	free(disease);
}

void recordPatientExit( char * input , Patient_list *list)
{
	if(input != NULL)
	{
		Date date;
		char * token = NULL;

		token = strtok(input, " ");
		int id = atoi(token);

	 	token = strtok(NULL, "-");
	    date.day = atoi (token);
	    //printf("_%d_\n", date.day);

	    token = strtok(NULL, "-");
	    date.month = atoi (token);
	    //printf("_%d_\n", date.month);

	    token = strtok(NULL, "\n");
	    date.year = atoi (token);
	    if(id_exist(list, id))
	    {
			Patient_Node * this = list_date_modify(list, date,  id);
		    printf("Exit date of Patient with recordID %d updated!", id );
		    printf(" New patient details are : ");
		    printPatientData(this->patient);
		    printf("\n");
	    }
	    else
	    {
	    	printf("No patient with id %d found!\n",id );
	    }

	}

	else
	{
		printf("Enter Valid Data\n");
	}
}


void numCurrentPAtients( char * input, HashTable * disease_HT)
{
	if(input != NULL)
	{
		int found = 0;
		char *token;
		token = strtok(input, "\n");
		int index = hash_fun(input, disease_HT->size);
		Bucket_Node *temp = disease_HT->lists_of_buckets[index].head;
		while(temp !=NULL)
		{
			for (int i = 0; i < temp->slot_counter; ++i)
			{
				if(strcmp(temp->bucket_item[i].string, token))
				{
					continue;
				}
				else
				{
					printf("Disease %s has a total of %d patients still hospitalized \n", temp->bucket_item[i].string, temp->bucket_item[i].patients_hospitalized);
					found = 1;
					break;
				}
			}
			temp = temp->next;

		}
		if(!found)
			printf("No Patient with %s disease found in our hospital records \n", token);
	}
	else
	{
		int counter;
		for (int i = 0; i < disease_HT->size; ++i)
		{	
			if( disease_HT->lists_of_buckets[i].head != NULL)
			{
				Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
				while(temp !=NULL)
				{
					for (int i = 0; i < temp->slot_counter; ++i)
					{	
						printf("Disease %s has a total of %d patients still hospitalized \n", temp->bucket_item[i].string, temp->bucket_item[i].patients_hospitalized);
					}
					temp = temp->next;

				}
			}
		}
	}
}


void diseaseFrequency( char * input, HashTable * disease_HT, Patient_list *list)
{
	if (input == NULL)
	{
		printf("Please Insert Valid Data\n");
		return;
	}

	Date d1, d2;
	char * disease;
	char * country;

	df_tokenize(input, &disease, &country, &d1, &d2 );

	if(date_cmp(d1, d2)== 1)
	{
		printf("First Date must be smaller than second Date\n");
		return;
	}


	if(record_exist(disease, disease_HT)) //find if given disease exist
	{			
		int counter = 0;
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
						printf("Disease %s has a total of %d incidents between ", temp->bucket_item[i].string, counter);
						print_date(d1); print_date(d2); printf("\n");
						break;
					}
					else
					{
						tree_search_Country_dateRange(temp->bucket_item[i].root, d1, d2, country, &counter);
						printf("Disease %s has a total of %d incidents between ", temp->bucket_item[i].string, counter);
						print_date(d1); print_date(d2); printf(" in Country %s\n", country);
						break;
					}
				}
			}
		temp = temp->next;
		}
	}
	else
	{
		printf("No such disease found in our records!\n");
		free(disease);
		free(country);
		return;
	}
	free(disease);
	free(country);
}


void insertPatientRecord( char * input, HashTable * disease_HT, HashTable *country_HT, Patient_list *list)
{	
	if (input == NULL)
	{
		printf("Please Insert Valid Data\n");
		return;
	}
	printf("strlen of input is %ld\n",strlen(input) );
	Patient patient_attributes;

	if (strlen(input) < 45)
		patient_attributes = line_tokenize_without_exitDate(input);
	else
		patient_attributes = line_tokenize(input, patient_attributes);

	Patient_Node * new_patient_node = NULL;

	if(patient_attributes.entryDate.day == 0)
	{
	 	printf("First Date must be smaller than second Date\n");
	 	return;
	}	

    if (id_exist(list, patient_attributes.recordID) )
    {
        printf("Patient Record ID %d already exist!\n", patient_attributes.recordID);
        return;
    }

    new_patient_node =  insertNewPatient(list, patient_attributes);
    insert_to_hash_table(disease_HT, patient_attributes.diseaseID, new_patient_node);
    insert_to_hash_table(country_HT, patient_attributes.country, new_patient_node);

    printf("New patient with following attributes succesfully inserted\n"); 
    printf("recordID : %d\n", patient_attributes.recordID);
    printf("FirstName : %s\n", patient_attributes.firstName);
    printf("LastName : %s\n", patient_attributes.lastName);
    printf("diseaseID : %s\n", patient_attributes.diseaseID);
    printf("Country : %s\n", patient_attributes.country);
    printf("Entry date : ");
    print_date(patient_attributes.entryDate);
    printf("\n");

    if(patient_attributes.exitDate.year > 1)
    {
    	printf("exitDate: ");
    	print_date(patient_attributes.exitDate);
    	printf("\n");
    }
}

/********************/


/***** Utillity Functions ****/

void dateTokenize( char * input, Date *d1, Date *d2)
{	
	char * token = NULL;

 	token = strtok(input, "-");
    d1->day = atoi (token);
    //printf("_%d_\n", d1->day);

    token = strtok(NULL, "-");
    d1->month = atoi (token);
    //printf("_%d_\n", d1->month);

    token = strtok(NULL, " ");
    d1->year = atoi (token);
    //printf("_%d_\n", d1->year);

    token = strtok(NULL, "-");
    d2->day = atoi (token);
    //printf("_%d_\n", d2.day);

    token = strtok(NULL, "-");
    d2->month = atoi (token);
    //printf("_%d_\n", d2.month);

    token = strtok(NULL, "\n");
    d2->year = atoi (token);
    //printf("_%d_\n", d2.year);    
}

Patient line_tokenize_without_exitDate(char * input)
{
	Patient patient;
	char * token;

    token = strtok(input, " ");
    patient.recordID = atoi(token);
    printf("_%d_\n", patient.recordID);

    token = strtok(NULL, " ");
    patient.firstName = malloc(sizeof(char)*strlen(token)+1);
    strcpy( patient.firstName, token);
    printf("_%s_\n", patient.firstName);

    token = strtok(NULL, " ");
    patient.lastName = malloc(sizeof(char)*strlen(token)+1);
    strcpy( patient.lastName, token);
    printf("_%s_\n", patient.lastName);

    token = strtok(NULL, " ");
    patient.diseaseID = malloc(sizeof(char)*strlen(token)+1);
    strcpy( patient.diseaseID, token);
    printf("_%s_\n", patient.diseaseID);

    token = strtok(NULL, " ");
    patient.country = malloc(sizeof(char)*strlen(token)+1);
    strcpy( patient.country, token);
    printf("_%s_\n", patient.country);

    token = strtok(NULL, "-");
    patient.entryDate.day = atoi (token);
    printf("_%d_\n", patient.entryDate.day);

    token = strtok(NULL, "-");
    patient.entryDate.month = atoi (token);
    printf("_%d_\n", patient.entryDate.month);

    token = strtok(NULL, "\n");
    patient.entryDate.year = atoi (token);
    // printf("_%d_\n", patient.entryDate.year);
    
	patient.exitDate.day = 0;
    patient.exitDate.month = 0;
    patient.exitDate.year = 0;
    return patient;
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
        //printf("_%d_\n", patient.entryDate.day);

        token = strtok(NULL, "-");
        d1->month = atoi (token);
        //printf("_%d_\n", patient.entryDate.month);

        token = strtok(NULL, " ");
        d1->year = atoi (token);

        token = strtok(NULL, "-");
        d2->day = atoi (token);
        //printf("_%d_\n", patient.entryDate.day);

        token = strtok(NULL, "-");
        d2->month = atoi (token);
        //printf("_%d_\n", patient.entryDate.month);

        token = strtok(NULL, " ");
        d2->year = atoi (token);

        token = strtok(NULL, "\n");
        *country_holder = malloc(sizeof(char)*strlen(token)+1);
		strcpy(*country_holder, token);
 	}
}


void topK_tokenize(char * input, char ** string, Date * d1, Date *d2, int *k)
{
	char *token;
	if(input != NULL)
	{
		if ( strlen(input)< 15)
		{
			token = strtok(input, " ");
			*k = atoi(token); // number of incidents

			token = strtok(NULL, "\n");
			*string = malloc(sizeof(char)*strlen(token)+1);
			strcpy(*string, token);
			d1->day = 0;
			d1->month = 0;
			d1->year = 0;
		}
		else
		{
			token = strtok(input, " ");
			*k = atoi(token); // number of incidents

			token = strtok(NULL, " ");
			*string = malloc(sizeof(char)*strlen(token)+1);
			strcpy(*string, token);
			token = strtok( NULL , "\n");
			dateTokenize(token, d1, d2);
		}
	}
	else
	{
		printf("Please Enter Valid Data");
	}	
}

/*********************/