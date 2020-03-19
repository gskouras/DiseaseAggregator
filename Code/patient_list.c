#include "../Headers/patient_list.h"


Patient_Node *createPatientNode(Patient patient)
{
	Patient_Node *new_patient_node = malloc(sizeof(Patient_Node));
	new_patient_node->patient = patient;
	return new_patient_node;
}

void initPatientList(Patient_list *this_list)
{
	Patient_Node *head = NULL;
	Patient_Node *tail = NULL;
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


int id_exist(Patient_list *list, int id)
{
	Patient_Node *temp = list->head;

	if(list->counter == 0)
		return 0;

	while(temp != NULL)
	{	
		if(temp->patient.recordID == id)
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

Patient_Node* list_date_modify( Patient_list *list, Date date, int id)
{
	Patient_Node *temp = list->head;
	while(temp != NULL)
	{
		if(temp->patient.recordID == id)
		{	
			temp->patient.exitDate = date;
			return temp;
		}
		temp = temp->next;
	}
	if (temp == NULL)
	{
	    printf("Error. No patients with id %d found!", id);
	}
}

void printPatientData(Patient patient)
{
	printf(" %d %s %s %s %s %d-%d-%d %d-%d-%d", patient.recordID, patient.firstName, patient.lastName,
	patient.diseaseID, patient.country, patient.entryDate.day, patient.entryDate.month, patient.entryDate.year,
	patient.exitDate.day, patient.exitDate.month, patient.exitDate.year);
}

void freePatientList(Patient_list * list )
{
	Patient_Node *temp = list->head;
	while (list->counter != 0)
	{
		temp = list->head;
		list->head = list->head->next;
		list->counter--;
		free(temp->patient.firstName);
		free(temp->patient.lastName);
		free(temp->patient.diseaseID);
		free(temp->patient.country);
		free(temp);
	}
}
