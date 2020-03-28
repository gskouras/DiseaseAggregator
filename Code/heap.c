#include "../Headers/heap.h"

/* The following code is a modified version of this 
https://github.com/hariuserx/MinHeap/blob/master/src/MinHeapWithoutArrays.java?fbclid=IwAR1HjssxLIgzHlWUobjAZg5491m52Ei47CkF2tYQcd4s1hSqH96nOFNveZk
source code for the needs of this project
*/

/*** Constructors ***/

void initMaxHeap(Max_Heap *this_heap)
{
	this_heap->root = NULL;
	this_heap->tail = NULL;
	this_heap->total_records = 0;
}

Heap_Node * createHeapNode(int counter, char * record)
{
	Heap_Node *new_node = malloc(sizeof(Heap_Node));
	new_node->parent = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->prevTail = NULL;
	new_node->counter = counter;
	strcpy(new_node->record, record);
	return new_node;
}

/***********************/

/*** Max Heap Functions ***/

void heap_insert(Max_Heap * heap,  int counter, char * record )
{
	if(heap->root == NULL) //if heap is empty new record becomes thes root
	{
		heap->root = createHeapNode(counter, record);
		heap->tail = heap->root;
	}
	else if( heap->tail->left == NULL) //left and right childs must have a record in order to keep heap properties
	{
		heap->tail->left = createHeapNode(counter, record);
		heap->tail->left->parent = heap->tail;
		maxHeapify(heap->tail->left);
	}
	else
	{
		heap->tail->right = createHeapNode(counter, record);
		heap->tail->right->parent = heap->tail;
		maxHeapify(heap->tail->right);
		Heap_Node * prevTail = heap->tail;
		setTail(heap, heap->tail); //set new tail for max_heap
		heap->tail->prevTail = prevTail;
	}

	heap->total_records++;
}


void heap_root_delete(Max_Heap *heap)
{
	if( heap->root == NULL)
	{
		printf("Heap is already empty\n");
		return;
	}

	if (heap->tail->right != NULL)
	{
		swapHeapNodeData(heap->tail->right, heap->root);
		free(heap->tail->right);
		heap->tail->right = NULL;
		revMaxHeapify(heap->root);
	}
	else if(heap->tail->left != NULL)
	{
		swapHeapNodeData(heap->tail->left, heap->root);
		free(heap->tail->left);
		heap->tail->left = NULL;
		revMaxHeapify(heap->root);
	}
	else
	{
		heap->tail = heap->tail->prevTail;
		heap_root_delete(heap);
		//heap->total_records++;
	}

	heap->total_records--;
}


void maxHeapify(Heap_Node * this_node ) // Keep Heap Properties By Recursively exchange child not data with parent's when neccesary
{
	if(this_node->parent != NULL) //Base Case (when this becomes true it means we arrived at the root of the heap)
	{
		if(this_node->parent->counter < this_node->counter) //if parent is smaller than the child swap their data
		{
			swapHeapNodeData(this_node->parent, this_node); 
			maxHeapify(this_node->parent);
		}
	}
}  


void revMaxHeapify( Heap_Node * this_node)
{
	if(this_node == NULL || this_node->left == NULL)
		return;

	Heap_Node * new_max = this_node->left;

	if (this_node->right != NULL && new_max->counter < this_node->right->counter) //find the new max of the heap after current max deleted
		new_max = this_node->right;

	if(new_max->counter > this_node->counter) 
	{
		swapHeapNodeData(this_node, new_max);
		revMaxHeapify(new_max);
	}
}

void heap_print(Heap_Node * root)
{
	if (root != NULL) 
    {    	
        printf("Disease %s %d\n",root->record, root->counter );
        heap_print(root->left); 
        heap_print(root->right); 
    } 
}

void heap_destroy(Heap_Node * root)
{
	if(root == NULL)
		return;	

	heap_destroy(root->left);
	heap_destroy(root->right);
	free(root);
}

/********************/

/*** Utillity Functions ***/

void setTail(Max_Heap *heap, Heap_Node *this_node)
{
	/* If we reach this stage that means a level is completely filled
	and we need to proceed to the next level by going to the extreme left. 
	*/
    if(this_node->parent == NULL)
    {
    	heap->tail = this_node;
		while(heap->tail->left != NULL)
		{
			heap->tail = heap->tail->left;
		}

    }
	/*If current this_node is the left this_node, go to the right this_node and
	proceed left from there to reach the left most this_node. 
	*/
   else if(this_node->parent->left == this_node)
   {
    	heap->tail = this_node->parent->right;
        while(heap->tail->left != NULL)
        {
			heap->tail = heap->tail->left;
        }
    }
	else if(this_node->parent->right == this_node)
	{
		setTail(heap, this_node->parent);
	}

}

void  swapHeapNodeData(Heap_Node *a, Heap_Node *b)
{
	int temp_counter = a->counter;
	a->counter = b->counter;
	b->counter = temp_counter;

	char temp_record [20];
	strcpy(temp_record, a->record);
	strcpy(a->record, b->record);
	strcpy(b->record, temp_record);

}


int getMaximum(Max_Heap *this_heap)
{
    return this_heap->root->counter;
}



