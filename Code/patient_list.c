#include "../Headers/patient_list.h"

int readPatientRecordsFile ( char * fileName)
{
	FILE *fp = fopen(fileName, "r");

	if (fp == NULL) //error handling when opening the file
	{
    	perror(" Requested file failed to open");
    	return 0;
	}

	char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    Patient patient;
    Patient_list *list = initPatientList(list);

	while ((nread = getline(&line, &len, fp)) != -1) 
	{
        //fwrite(line, nread, 1, stdout);
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

        token = strtok(NULL, "- ");
        patient.entryDate.year = atoi (token);
        //printf("_%d_\n", patient.entryDate.year);


        if ((char)line[(nread-9)] == '-') //this means that current patient hasnt take discharge from hospital
        {
			patient.exitDate.day = 1;
			patient.exitDate.month = 1;
			patient.exitDate.year = 1;
			//printf("_%d_\n", patient.exitDate.year);

        }
        else
        {
        	token = strtok(NULL, "-");
        	patient.exitDate.day = atoi (token);
        	//printf("_%d_\n", patient.exitDate.day);

        	token = strtok(NULL, "-");
        	patient.exitDate.month = atoi (token);
       		//printf("_%d_\n", patient.exitDate.month);

        	token = strtok(NULL, "\n");
        	patient.exitDate.year = atoi (token);
        	//printf("_%d_\n", patient.exitDate.year);
        }

        insertNewPatient(list, patient);
        
	}

	printPatientList(list);
	free(patient.firstName);
	free(patient.lastName);
	free(patient.diseaseID);
	free(patient.country);


    free(line);
    fclose(fp);

	return 1;
}



Patient_Node *createPatientNode(Patient patient)
{
	Patient_Node *new_patient_node = malloc(sizeof(Patient_Node));
	new_patient_node->patient = patient;
	//printPatientData(new_patient_node->patient);
	return new_patient_node;
}

Patient_list * initPatientList( Patient_list * this_list)
{
	this_list = malloc(sizeof(Patient_list));
	Patient_Node *head = NULL;
	Patient_Node *tail = NULL;
	int counter = 0;
	return this_list;
}

void insertNewPatient(Patient_list * list, Patient patient)
{
	Patient_Node *new_patient = createPatientNode(patient);
	if (list->counter == 0)
	{

		list->head = new_patient;
		list->tail = new_patient;
		list->counter = 1;
		return;
	}

	list->counter++;
	list->tail->next = new_patient;
	list->tail = new_patient;
}


void printPatientList(Patient_list * list )
{
	Patient_Node *temp = list->head;
	while(temp != NULL)
	{
		printPatientData(temp->patient);
		printf("\n");
		temp = temp->next;
	}
}


void printPatientData(Patient patient)
{
	printf("%d %s %s %s %s %d %d %d %d %d %d", patient.recordID, patient.firstName, patient.lastName,
	patient.diseaseID, patient.country, patient.entryDate.day, patient.entryDate.month, patient.entryDate.year,
	patient.exitDate.day, patient.exitDate.month, patient.exitDate.year);
}

void freePatientList(Patient_list * list )
{
	Patient_Node *temp = list->head;
	while (list->head != NULL)
	{
		temp = list->head;
		list->head = list->head->next;
		free(temp);
	}
	free(list);
}
