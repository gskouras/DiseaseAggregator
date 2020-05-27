#include "../Headers/patient_list.h"


Patient_Node *createPatientNode(Patient patient)
{
	Patient_Node *new_patient_node = malloc(sizeof(Patient_Node));
	new_patient_node->patient = patient;
	new_patient_node->next = NULL;
	return new_patient_node;
}

void initPatientList(Patient_list *this_list)
{
	this_list->head = NULL;
	this_list->tail = NULL;
	this_list->counter = 0;
}

Patient_Node* insertNewPatient(Patient_list * list, Patient patient)
{
	Patient_Node *new_patient = createPatientNode(patient);
	if (list->counter == 0)
	{
		list->head = new_patient;
		list->tail = new_patient;
		list->counter = 1;
		return new_patient;
	}

	list->counter++;
	list->tail->next = new_patient;
	list->tail = new_patient;
	return new_patient;
}


int id_exist(Patient_list *list, char * id)
{
	Patient_Node *temp = list->head;

	if(list->counter == 0)
		return 0;

	while(temp != NULL)
	{	
		if(strcmp(temp->patient.recordID, id)==0)
			return 1;
		temp = temp->next;
	}
	return 0;
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

Patient_Node* list_date_modify( Patient_list *list, Date date, char * id)
{
	Patient_Node *temp = list->head;
	while(temp != NULL)
	{
		if(strcmp(temp->patient.recordID, id)==0)
		{	
			temp->patient.exitDate = date;
			return temp;
		}
		temp = temp->next;
	}
	if (temp == NULL)
	{
	    printf("Error. No patients with id %s found!", id);
	}
}

void printPatientData(Patient patient)
{
	printf(" %s %s %s %s %s %s %d-%d-%d %d-%d-%d %d\n", patient.recordID, patient.status, patient.firstName, patient.lastName,
	patient.country, patient.diseaseID, patient.entryDate.day, patient.entryDate.month, patient.entryDate.year,
	patient.exitDate.day, patient.exitDate.month, patient.exitDate.year, patient.age );
}


void updatePatientRecord( char * id, Date date, Patient_list *list)
{
	Patient_Node * this = list_date_modify(list, date,  id);
    // printf("Exit date of Patient with recordID %s updated!", id );
    // printf(" New patient details are : ");
    // printPatientData(this->patient);
    // printf("\n");
}








void freePatientList(Patient_list * list )
{
	Patient_Node *temp = list->head;
	while (list->counter != 0)
	{
		temp = list->head;
		list->head = list->head->next;
		list->counter--;
		free(temp->patient.recordID);
		free(temp->patient.firstName);
		free(temp->patient.lastName);
		free(temp->patient.diseaseID);
		free(temp->patient.country);
		free(temp);
	}
}
