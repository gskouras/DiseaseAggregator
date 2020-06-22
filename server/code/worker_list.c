#include "../headers/worker_list.h"

Worker_Node *createWorkerNode(  char ** countries, int portNum, int country_count, char *ip )
{
	Worker_Node *new_node= malloc(sizeof(Worker_Node));
	new_node->portNum = portNum;
	new_node->country_count = country_count;
	new_node->countries= calloc(sizeof(char*) , country_count);
	
	for (int i = 0; i < country_count; ++i)
	{
		new_node->countries[i] = calloc(sizeof(char) , strlen(countries[i]) + 1);
		strcpy(new_node->countries[i] , countries[i]);
	}
	new_node->ip = calloc(sizeof(char) , strlen(ip) + 1);
	strcpy(new_node->ip, ip);
	new_node->next = NULL;
	return new_node;
}

void initWorkertList(Worker_list *this_list)
{
	this_list->head = NULL;
	this_list->tail = NULL;
	this_list->counter = 0;
}

void insertNewWorker(Worker_list * list, char ** countries, int portNum, int country_count, char *ip)
{
	Worker_Node *new_worker = createWorkerNode(countries, portNum, country_count, ip);
	if (list->counter == 0)
	{
		list->head = new_worker;
		list->tail = new_worker;
		list->counter = 1;
		return;
	}

	list->counter++;
	list->tail->next = new_worker;
	list->tail = new_worker;
}

void update_worker(Worker_list *w_list, int portnum, char * ip, int country_pos)
{
	Worker_Node *temp = w_list->head;
	int count = 0;
	while(temp != NULL)
	{
		if(count == country_pos)
		{
			temp->portNum = portnum;
			strcpy(temp->ip, ip);
		}
		count++;
		temp = temp->next;
	}
}

void printWorkerList(Worker_list * list )
{
	Worker_Node *temp = list->head;
	while(temp != NULL)
	{
		printf("I am worker with portNum %d\n",temp->portNum);
		printf("My countries are ::: \n");
		for (int i = 0; i < temp->country_count; ++i)
		{
			printf("%s ",temp->countries[i]);
		}
		printf("\n\n");
		temp = temp->next;
	}
}

Worker_Node* get_worker(Worker_list * list,  int index) 
{ 
    Worker_Node* current = list->head; 

    int count = 0; 
    while (current != NULL) 
    { 
        if (count == index) 
            return(current); 
        count++; 
        current = current->next; 
    }         
} 

void freeWorkerList(Worker_list * list )
{
	Worker_Node *temp = list->head;
	while (list->counter != 0)
	{
		temp = list->head;
		for (int i = 0; i < temp->country_count; ++i)
		{
			free(temp->countries[i]);
		}
		free(temp->countries);
		free(temp->ip);
		free(temp);
		list->head = list->head->next;
		list->counter--;
	}
}
