#include "../Headers/heap.h"


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
	if(heap->root == NULL)
	{
		heap->root = createHeapNode(counter, record);
		heap->tail = heap->root;
	}
	else if( heap->tail->left == NULL)
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
		setTail(heap, heap->tail);
		heap->tail->prevTail = prevTail;
	}
	heap->total_records++;
}

void maxHeapify(Heap_Node * this_node )
{
	if(this_node->parent != NULL)
	{
		if(this_node->parent->counter < this_node->counter)
		{
			swapHeapNodeData(this_node->parent, this_node);
			maxHeapify(this_node->parent);
		}
	}
}  

void heap_print(Heap_Node * root)
{
	if (root != NULL) 
    {    	
        printf("Bla %d Bla %s\n", root->counter, root->record );
        heap_print(root->left); 
        heap_print(root->right); 
    } 
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
	int temp = a->counter;
	a->counter = b->counter;
	b->counter = temp;
}


int getMaximum(Max_Heap *this_heap)
{
    return this_heap->root->counter;
}


