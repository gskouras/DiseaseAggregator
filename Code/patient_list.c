#include "../Headers/patient_list.h"

Patient_Node *createPatientNode(Patient patient)
{
	Patient_Node *new_patient_node = malloc(sizeof(Patient_Node));
	new_patient_node->patient = patient;
	//printPatientData(new_patient_node->patient);
	return new_patient_node;
}

Patient_list * initPatientList()
{
	Patient_list *this_list = malloc(sizeof(Patient_list));
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
