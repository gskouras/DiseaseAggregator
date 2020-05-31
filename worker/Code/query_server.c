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

char * topAgeRanges(char *input, HashTable * disease_HT, Patient_list *list)
{
	char *disease = NULL, *result = NULL, *country = NULL;
	Date d1, d2;

	int k;
	int counter = 0, found = 0, flag = 1;

	char temp_input[50];
	strcpy(temp_input , input);

	topK_tokenize(temp_input, &country, &disease, &d1, &d2, &k);

	if(date_cmp(d1, d2) == 1) //date 2 is bigger	
	{
		printf("First Date must be smaller than second Date\n");
		return NULL;
	}

	// printf("Disease is %s \n", disease);
	// printf("Country is %s\n", country);
	// printf("k is %d\n",k );
	// print_date(d1);
	// printf("\n");
	// print_date(d2);
	int * age_ranges = malloc(sizeof(int)* 4);
	for (int i = 0; i < 4; ++i)
	{
		age_ranges[i] = 0;
	}

	int index = hash_fun(disease, disease_HT->size);
	Bucket_Node *temp = disease_HT->lists_of_buckets[index].head;
	while(temp !=NULL)
	{
		for (int i = 0; i < temp->slot_counter; ++i)
		{
			//printf("Sigkrino tin asthenia %s me tin asthenia %s\n", temp->bucket_item[i].string, disease);
			if(strcmp(temp->bucket_item[i].string, disease) == 0)
			{
				//printf("beno na psakso xoris na dosei xora o xristis\n");
				//tree_search_Country_dateRange(temp->bucket_item[i].root, d1, d2, country, &counter, flag);
				tree_search_Age_Country_dateRange(temp->bucket_item[i].root, d1, d2, country, age_ranges);
				//printf("Disease %s has a total of %d incidents between ", temp->bucket_item[i].string, counter);
				// print_date(d1); print_date(d2); printf(" in Country %s\n", country);					
				found = 1;
				break;	
			}
		}
	temp = temp->next;
	}

	if(!found)
	{
		//printf("didnt found!!!\n");
		result = malloc(sizeof(char)* 2);
		sprintf(result, "0");
		free(disease);
		return result;
	}


	int sum = 0;
	for (int i = 0; i < 4; ++i) //get the sum of all records in given range for tha country
	{
		sum += age_ranges[i];
	}

	result = malloc(sizeof(char)* 50);
	memset(result, '\0', sizeof(result));

	for (int i = 0; i < k; ++i)
	{
		index = max_index(age_ranges, 4);
		switch (index)
		{
    		case 0:
      			sprintf(result, "%s0-20: %f%%\n", result, ((float)age_ranges[index]/ (float)sum)*100);
      		break;

    		case 1:
      			sprintf(result, "%s21-40: %f%%\n", result, ((float)age_ranges[index]/(float) sum)*100);
      		break;

			case 2:
      			sprintf(result, "%s41-60: %f%%\n", result, ((float)age_ranges[index]/ (float)sum)*100);
      		break;

      		case 3:
      			sprintf(result, "%s60+: %f%%\n", result, ((float)age_ranges[index]/ (float)sum)*100);
      		break;
      	}
		age_ranges[index] = 0;
	}


	//printf("%s\n",result );
	free(country);
	free(disease);
	return result;
}

char * numPatientAdmissions( char * input, HashTable * disease_HT, Patient_list *list, char * country, int flag)
{
	char *disease = NULL, *result = NULL, *temp_country = NULL;	

	Date d1, d2;
	int counter = 0;
	int found = 0;

	// printf("NumPAtient received your queryL:::::: %s \n", input);
	char temp_input[50];
	strcpy(temp_input , input);

	df_tokenize(temp_input, &disease, &temp_country, &d1, &d2);


	if(record_exist(disease, disease_HT)) //find if given disease exist
	{			
		int index = hash_fun(disease, disease_HT->size);
		Bucket_Node *temp = disease_HT->lists_of_buckets[index].head;
		while(temp !=NULL)
		{
			for (int i = 0; i < temp->slot_counter; ++i)
			{
				//printf("Sigkrino tin asthenia %s me tin asthenia %s\n", temp->bucket_item[i].string, disease);
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
		//printf("didnt found!!!\n");
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
		//printf("Country is %s\n", temp_country);
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



/********************/


/***** Utillity Functions ****/



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


int max_index(int *a, int n)
{
	if(n <= 0) 
		return -1;

	int i, max_i = 0;
	int max = a[0];

	for(i = 1; i < n; ++i)
	{
		if(a[i] > max)
		{
			max = a[i];
			max_i = i;
		}
	}
	return max_i;
}
 


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
	if (strlen(input) < 30) //tokenize without country name
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

void topK_tokenize(char * input, char ** country, char ** disease, Date * d1, Date *d2, int *k)
{
	char *token;

	token = strtok(input, " ");
	*k = atoi(token); // number of incidents

	token = strtok(NULL, " ");
	*country = malloc(sizeof(char)*strlen(token)+1);
	strcpy(*country, token);

	token = strtok( NULL , " ");
	*disease = malloc(sizeof(char)*strlen(token)+1);
	strcpy(*disease, token);

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

    token = strtok(NULL, "\n");
    d2->year = atoi (token);  
    printf("hii\n");

}