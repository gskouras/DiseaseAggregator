#include "../headers/date_list.h"



Date_Node *createDateNode()
{
	Date_Node *new_node = malloc(sizeof(Date_Node));
	new_node->patient_node = NULL;
	new_node->next = NULL;
	return new_node;
}

void initDateList(Date_List * this_list)
{
	this_list->head = NULL;
	this_list->tail = NULL;
	int counter = 0;
}


void insertNewDate(Date_List * list, Patient_Node * patient_node)
{
	Date_Node *new_date = createDateNode();
	new_date->patient_node = patient_node;
	if (list->head == NULL)
	{
		list->head = new_date;
		list->tail = new_date;
		list->counter++;
		return;
	}
	list->counter++;
	list->tail->next = new_date;
	list->tail = new_date;
	//printPatientData(new_date->patient_node->patient);
	return;
}


void printDatetList(Date_List * list )
{

	Date_Node *temp = list->head;
	while(temp != NULL)
	{
		printPatientData(temp->patient_node->patient);
		printf("\n");
		temp = temp->next;
	}
}


void freeDateList(Date_List * list )
{
	Date_Node *temp = list->head;
	while (list->head != NULL)
	{
		temp = list->head;
		list->head = list->head->next;
		free(temp);
	}
}
