#include "../Headers/main.h"

int main(int argc, char *argv[])
{
    Params params;
    if(argc==1)
    {
        params.fileName = "sample";
        params.disHashSize = 1;
        params.countryHashSize= 50;
        params.bucketsize = 128;
    }
	else
    { 
        params = inputValidate(argc, argv);
    }

    HashTable hash_table;
    Patient_list * list = NULL;

	if(readPatientRecordsFile ( params, &hash_table, list))
		return 0;
}


int readPatientRecordsFile ( Params params, HashTable *hash_table, Patient_list * list)
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

    Patient_Node * new_patient_node;
    list = initPatientList();
    initHashTable(hash_table, params.disHashSize, params.bucketsize);

    while ((nread = getline(&line, &len, fp)) != -1) 
    {   

        patient_attributes = string_tokenize(line, patient_attributes);
        new_patient_node =  insertNewPatient(list, patient_attributes);
        insert_to_hash_table(hash_table, patient_attributes.diseaseID, new_patient_node);
        
    }
    print_hash_table(hash_table);
    //printf("hash table total bucket items ar %d\n", hash_table->total_bucket_items);
    //printPatientList(list);
    // free(patient.firstName);
    // free(patient.lastName);
    // free(patient.diseaseID);
    // free(patient.country);

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
	if ( argc != 9 )
	{
		printf("Error. Arguement related error: Got %d, expectetd 9\n", argc);
		exit(0);
	}

	Params params;

	if ( (strcmp(argv[1], "-p") == 0 && strcmp(argv[3], "-h1") == 0 && strcmp(argv[5], "-h2") == 0 && strcmp(argv[7], "-b") == 0)
	&& !digitValidate(argv[4]) && !digitValidate(argv[6]) && !digitValidate(argv[8]))
	{
        if(atoi(argv[8]) > 30)
        {
    		params.fileName = argv[2];
    		params.disHashSize = atoi(argv[4]);
    		params.countryHashSize = atoi(argv[6]);
    		params.bucketsize = atoi(argv[8]);
        }
        else
        {
            printf("Bucket size must be greater than 30\n");
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


Patient string_tokenize(char *line, Patient patient )
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
            patient.exitDate.day = 1;
            patient.exitDate.month = 1;
            patient.exitDate.year = 1;
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