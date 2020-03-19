#include "../Headers/main.h"


void cli(HashTable * , HashTable *, Patient_list* );
void open_manual();

int main(int argc, char *argv[])
{
    Params params = inputValidate(argc, argv);

    Patient_list patient_list;

    HashTable disease_HT;
    HashTable country_HT;

    printf("\nProccesing Input...\n");

	readPatientRecordsFile ( params, &disease_HT, &country_HT, &patient_list);

    printf("Parse of file Completed Succesfully!\n\n");

    //print_hash_table(&disease_HT);
    print_hash_table(&country_HT);

    cli(&disease_HT, &country_HT, &patient_list);
    // Date d1, d2;
    // d1.day = 1;
    // d1.month = 9;
    // d1.year = 2002;
    // d2.day = 4;
    // d2.month = 8;
    // d2.year = 2010;
    // if(date_cmp(d1, d2) == 0)
    //     printf("d1 is smaller than d2\n");

    destroyHashTable(&disease_HT);
    destroyHashTable(&country_HT);
    freePatientList(&patient_list);
}


int readPatientRecordsFile ( Params params, HashTable * disease_HT, HashTable * country_HT, Patient_list *patient_list)
{
    FILE *fp = fopen(params.fileName, "r");

    if (fp == NULL) //error handling when opening the file
    {
        perror(" Requested file failed to open");
        return 0;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int line_pos;

    Patient patient_attributes; 

    Patient_Node * new_patient_node = NULL;

    initHashTable(disease_HT, params.disHashSize, params.bucketsize); 
    initHashTable(country_HT, params.countryHashSize, params.bucketsize); 

    initPatientList(patient_list);

    while ((nread = getline(&line, &len, fp)) != -1) 
    {   

        patient_attributes = line_tokenize(line, patient_attributes);
        
        if (id_exist(patient_list, patient_attributes.recordID))
        {
            printf("Patient with Record ID %d has been already inserted, thus it ommited\n", patient_attributes.recordID);
            free(patient_attributes.firstName);
            free(patient_attributes.lastName);
            free(patient_attributes.diseaseID);
            free(patient_attributes.country);
   
        }
        else
        {
            new_patient_node =  insertNewPatient(patient_list, patient_attributes);
            insert_to_hash_table(disease_HT, patient_attributes.diseaseID, new_patient_node);
            insert_to_hash_table(country_HT, patient_attributes.country, new_patient_node);
        }
    }
    free(line);
    fclose(fp);

    return 1;
}


int digitValidate(char *a)
{
	for (unsigned int i = 0; i < strlen(a); i++)
	{
		if(!isdigit(a[i]))
			return -1;
	}
	return 0;
}

Params inputValidate (int argc, char *argv[])
{
    Params params;
    
    if(argc==1)
    {
        params.fileName = "sample";
        params.disHashSize = 10;
        params.countryHashSize= 50;
        params.bucketsize = 256;
        return params;
    }
    else
    { 

    	if ( argc != 9 )
    	{
    		printf("Error. Arguement related error: Got %d, expectetd 9\n", argc);
    		exit(0);
    	}

    	if ( (strcmp(argv[1], "-p") == 0 && strcmp(argv[3], "-h1") == 0 && strcmp(argv[5], "-h2") == 0 && strcmp(argv[7], "-b") == 0)
    	&& !digitValidate(argv[4]) && !digitValidate(argv[6]) && !digitValidate(argv[8]))
    	{
            if(atoi(argv[8]) >= 128)
            {
        		params.fileName = argv[2];
        		params.disHashSize = atoi(argv[4]);
        		params.countryHashSize = atoi(argv[6]);
        		params.bucketsize = atoi(argv[8]);
            }
            else
            {
                printf("Bucket size must be greater than 128 bytes\n");
                exit(0);
            }
    	} 
    	else 
    	{
    		printf("Error. Arguement related error.\n");
    		exit(0);
    	}
    	return params;
    }
}


Patient line_tokenize(char *line, Patient patient )
{
        char * token;

        token = strtok(line, " ");
        patient.recordID = atoi(token);
        //printf("_%d_\n", patient.recordID);

        token = strtok(NULL, " ");
        patient.firstName = malloc(sizeof(char)*strlen(token)+1);
        strcpy( patient.firstName, token);
        //printf("_%s_\n", patient.firstName);

        token = strtok(NULL, " ");
        patient.lastName = malloc(sizeof(char)*strlen(token)+1);
        strcpy( patient.lastName, token);
        //printf("_%s_\n", patient.lastName);

        token = strtok(NULL, " ");
        patient.diseaseID = malloc(sizeof(char)*strlen(token)+1);
        strcpy( patient.diseaseID, token);
        //printf("_%s_\n", patient.diseaseID);

        token = strtok(NULL, " ");
        patient.country = malloc(sizeof(char)*strlen(token)+1);
        strcpy( patient.country, token);
        //printf("_%s_\n", patient.country);

        token = strtok(NULL, "-");
        patient.entryDate.day = atoi (token);
        //printf("_%d_\n", patient.entryDate.day);

        token = strtok(NULL, "-");
        patient.entryDate.month = atoi (token);
        //printf("_%d_\n", patient.entryDate.month);

        token = strtok(NULL, " ");
        patient.entryDate.year = atoi (token);
        //printf("_%d_\n", patient.entryDate.year);
        token = strtok(NULL, "- ");
        

        if (atoi(token) == 0 ) //this means that current patient hasnt take discharge from hospital
        {
            patient.exitDate.day = 0;
            patient.exitDate.month = 0;
            patient.exitDate.year = 0;
            return patient;
        }
        else
        {
            patient.exitDate.day = atoi (token);
            //printf("_%d_\n", patient.exitDate.day);
        
            token = strtok(NULL, "-");
            patient.exitDate.month = atoi (token);
            //printf("_%d_\n", patient.exitDate.month);

            token = strtok(NULL, "\n");
            patient.exitDate.year = atoi (token);
            //printf("_%d_\n", patient.exitDate.year);
        }

    return patient;
}