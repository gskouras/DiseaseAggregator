#include "../Headers/cli.h"


void cli(HashTable * disease_HT, HashTable * country_HT, Patient_list *list)
{
	int fd;
    char *input = NULL, *line = NULL, *cmd = NULL;
    size_t len = 0;
    ssize_t read;

	printf("##########  WELCOME TO DISEASE MONITOR COMMAND LINE INTERFACE  ##########\n\n"); 

	printf("You can use the available commands from the list above, or type ./exit to quit\n\n");

	printf("• ./globalDiseaseStats(./gl) [date1 date2]\t\t-->\t Print for every disease the total incidents in range of [date1, date2].\n\n"\

           "• ./topk-Diseases(./tkc) k country  [date1 date2]\t--> \t Print, for a given country, top k diseases in range [date1, date2].\n\n"\

           "• ./topk-Countries(./tkcd) k disease [date1 date2](tkcd) -->\t Print, for a given diseases, countries that top l incidents have apperead.\n\n"\

           "• ./recordPatientExit(./rpe) recordID exitDate\t\t-->\t Insert an exit Date to the patient with the given id.\n\n"\

           "• ./numCurrentPatients(./ncp) [disease]\t\t-->\t\t Print for a given disease the total patients that are still in the hospital. \n\n"\

           "• ./diseaseFrequency(./df) virusName [country] date1 date2 -->   Printf total incidents of a [country] in range date1, date2.\n\n"\

           "• ./insertPatientRecord(./ipr) recordID patientFirstName patientLastName diseaseID country entryDate [exitDate]--> Insert a new Record in the System with the given attributes.\n\n");

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
		    	if (input != NULL && strlen(input) < 10)
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
		    else if (strcmp(cmd, "./topk-Diseases") == 0 || strcmp(cmd, "./tkc") == 0) 
		    {

		        topDiseases(input);

		    } 
		    else if (strcmp(cmd, "./topk-Countries") == 0 || strcmp(cmd, "./tkcd") == 0 ) 
		    {

		        topCountries(input);

		    } 
		    else if (strcmp(cmd, "./recordPatientExit") == 0 || strcmp(cmd, "./rpe") == 0) 
		    {

		        recordPatientExit(input, list); //done

		    } 
		    else if (strcmp(cmd, "./numCurrentPatients") == 0 || strcmp(cmd, "./ncp") == 0) 
		    {

		        numCurrentPAtients(input, disease_HT); //done

		    } 
		    else if (strcmp(cmd, "./diseaseFrequency") == 0 || strcmp(cmd, "./df") == 0) 
		    {

		        diseaseFrequency(input);

		    } 
		    else if (strcmp(cmd, "./insertPatientRecord") == 0 || strcmp(cmd, "./ipr") == 0) 
		    {

		        insertPatientRecord(input, disease_HT, country_HT, list); //done

		    } 
		    else if (strcmp(cmd, "./exit") == 0) 
		    {
		    	printf("\nExiting Programm...\n\n"); //done
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

void globalDiseaseStats( char * input, HashTable * disease_HT )
{
	if(input != NULL)
	{
		int current_counter = 0; // counter to keep track of this bucket slot diseases
		int counter = 0;
 		int prev_counter = 0;
		Date d1, d2;
		if(date_cmp(d1, d2) == 1) //date 2 is bigger
			printf("First Date must be bigger than second Date\n");
			return;

		dateTokenize(input, &d1, &d2);
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
						printf("Disease %s has a total of %d incidents between [", temp->bucket_item[j].string, current_counter);
						print_date(d1); print_date(d2); printf("]\n");
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
					for (int i = 0; i < temp->slot_counter; ++i)
					{
						printf("Disease %s has a total of %d incidents \n", temp->bucket_item[i].string, getBucketSlotRecords(&temp->bucket_item[i]));
					}
					temp = temp->next;

				}
			}
		}
	}

}

void topDiseases( char * input )
{

}

void topCountries( char * input )
{

}

void recordPatientExit( char * input , Patient_list *list)
{
	Date date;
	char * token = NULL;

	token = strtok(input, " ");
	int id = atoi(token);

 	token = strtok(NULL, "-");
    date.day = atoi (token);
    printf("_%d_\n", date.day);

    token = strtok(NULL, "-");
    date.month = atoi (token);
    printf("_%d_\n", date.month);

    token = strtok(NULL, "\n");
    date.year = atoi (token);
    Patient_Node * this = list_date_modify(list, date,  id);
    printf("Exit date of Patient with recordID %d updated. ", id );
    printf("New patient details are : ");
    printPatientData(this->patient);
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


void diseaseFrequency( char * input )
{

}

void insertPatientRecord( char * input, HashTable * disease_HT, HashTable *country_HT, Patient_list *list)
{
	Patient patient_attributes;
	Patient_Node * new_patient_node = NULL;

	patient_attributes = string_tokenize(input, patient_attributes);

	if(date_cmp(patient_attributes.entryDate, patient_attributes.exitDate) == 1) //date 2 is bigger
	{
		printf("First Date must be bigger than second Date\n");
		return;
	}	

    if (id_exist(list, patient_attributes.recordID) )
    {
        printf("Patient with Record ID %d has been already inserted\n", patient_attributes.recordID);
        return;
    }

    new_patient_node =  insertNewPatient(list, patient_attributes);
    insert_to_hash_table(disease_HT, patient_attributes.diseaseID, new_patient_node);
    insert_to_hash_table(country_HT, patient_attributes.country, new_patient_node);

    printf("New with given Record "); 
    printPatientData(new_patient_node->patient);
    printf(" Succesfully Inserted!\n");
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


/*********************/