#include "../headers/input_list.h"

CountryPath_Node *createPatientNode(char * path)
{
	CountryPath_Node *new_node= malloc(sizeof(CountryPath_Node));
	strcpy(new_node->country_path, path);
	new_node->next = NULL;
	return new_node;
}

void initDirectorytList(Directory_list *this_list)
{
	this_list->head = NULL;
	this_list->tail = NULL;
	this_list->counter = 0;
}

CountryPath_Node* insertNewDirectory(Directory_list * list, char * path)
{
	CountryPath_Node *new_path = createPatientNode(path);
	if (list->counter == 0)
	{
		list->head = new_path;
		list->tail = new_path;
		list->counter = 1;
		return new_path;
	}

	list->counter++;
	list->tail->next = new_path;
	list->tail = new_path;
	return new_path;
}



CountryPath_Node * get_directoryList_head(Directory_list * list)
{
	CountryPath_Node *temp = list->head;
	while (list->counter != 0)
	{
		temp = list->head;
		list->head = list->head->next;
		return(temp);
	}
}

void printDirectoryList(Directory_list * list )
{
	CountryPath_Node *temp = list->head;
	while(temp != NULL)
	{
		printf("%s\n", temp->country_path);
		temp = temp->next;
	}
}

CountryPath_Node* get_country(Directory_list * list,  int index) 
{ 
    CountryPath_Node* current = list->head; 

    int count = 0; 
    while (current != NULL) 
    { 
        if (count == index) 
            return(current); 
        count++; 
        current = current->next; 
    }         
} 

void freeDirList(Directory_list * list )
{
	CountryPath_Node *temp = list->head;
	while (list->counter != 0)
	{
		temp = list->head;
		list->head = list->head->next;
		list->counter--;
		free(temp);
	}
}
