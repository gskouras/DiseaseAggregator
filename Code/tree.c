#include "../Headers/tree.h"

/*** The Following Code for Insertion and Right/Left Rotate and othe utility fucntions needed have been taken
from https://www.geeksforgeeks.org/avl-tree-set-1-insertion/ and modified for the needs of the project ***/


/**** Tree Constructors ****/
Tree_Node * createTreeNode(Date date)
{
	Tree_Node * new_node = malloc(sizeof(Tree_Node));
	int height = 1;
	int total_patients_hospitalized = 0;
	new_node->date = date;
	new_node->left = NULL;
	new_node->right = NULL;
	initDateList(&new_node->date_list);
	return new_node;
}


/*****************/


/*** Tree Functions ***/


Tree_Node * tree_insert( Tree_Node * tree_node , Date date , Patient_Node * this_patient )
{	

	if(tree_node == NULL)
	{
		Tree_Node * new_node = createTreeNode(date);
		tree_node = new_node;
		insertNewDate(&tree_node->date_list , this_patient);
		return tree_node;
	}
	else
	{
		if (date_cmp(date,tree_node->date) == 2) // equal dates
		{
			Date_Node *temp = tree_node->date_list.head;
			while(temp != NULL)
			{	
				if(temp->patient_node->patient.recordID != this_patient->patient.recordID)
					insertNewDate(&tree_node->date_list , this_patient);
				temp = temp->next;
			}
		}


		/**** Recursive Insert ****/
		if (date_cmp(date, tree_node->date) == 0) //date 2 is bigger
		{
			tree_node->left = tree_insert(tree_node->left, date, this_patient);
		}
		else if(date_cmp(date, tree_node->date) == 1) //date 1 is bigger
		{
			tree_node->right = tree_insert(tree_node->right, date, this_patient);
		}

		tree_node->height = max(height(tree_node->left), height(tree_node->right)) + 1;

		int balance = getBalance(tree_node);

		/*** Rotate Nodes in the right way in order to keep Properties of AVL Trees ***/
		if(balance > 1 && (date_cmp(date , tree_node->date)==0))
			return rightRotate(tree_node);

		if (balance < -1 && (date_cmp(date, tree_node->date)==1))
			return leftRotate(tree_node);

		if(balance > 1 && (date_cmp(date , tree_node->date)==1))
		{
			tree_node->left = leftRotate(tree_node->left);
			return rightRotate(tree_node);
		}

		if (balance < -1 && (date_cmp(date, tree_node->date)==0))
		{
			tree_node->right = rightRotate(tree_node->right);
			return leftRotate(tree_node);
		}
		return tree_node;
	}
	
}



Tree_Node *rightRotate(Tree_Node * this_node) 
{ 
    Tree_Node *left_child = this_node->left; 
    Tree_Node *lefts_child_right_child = left_child->right; 
  
    // Perform rotation 
    left_child->right = this_node; 
    this_node->left = lefts_child_right_child; 
  
    // Update heights 
    this_node->height = max(height(this_node->left), height(this_node->right))+1; 
    left_child ->height = max(height(left_child->left), height(left_child->right))+1; 
  
    // Return new root 
    return left_child; 
}


Tree_Node *leftRotate(Tree_Node * this_node) 
{
	Tree_Node *right_child = this_node->right; 
    Tree_Node *rights_child_left_child = right_child->left; 
  
    // Perform rotation 
    right_child->left = this_node; 
    this_node->right = rights_child_left_child; 
  
    // Update heights 
    this_node->height = max(height(this_node->left), height(this_node->right))+1; 
    right_child->height = max(height(right_child->left), height(right_child->right))+1; 
  
    // Return new root 
    return right_child; 
}

void tree_search_dateRange (Tree_Node * this, Date d1, Date d2, int *counter)
{
	/* base case */
	if(this == NULL)
		return;

	/* Since the desired o/p is sorted, recurse for left subtree first 
    If root->data is greater than k1, then only we can get o/p keys 
    in left subtree */
	if (date_cmp(d1 , this->date) == 0)
		tree_search_dateRange(this->left, d1, d2, counter);

	/* if root's data lies in range, then prints root's data */
	if(((date_cmp(d1, this->date) == 0) || (date_cmp(d1, this->date) == 2)) && ((date_cmp(d2, this->date) ==1) || (date_cmp(d2, this->date) == 2)))
	{
		*counter = *counter + 1;
	}

  	/* If root->data is smaller than k2, then only we can get o/p keys 
      in right subtree */
	if(date_cmp(d2 , this->date) == 1)
		tree_search_dateRange(this->right, d1, d2, counter);
}

void tree_search_Country_dateRange (Tree_Node * this, Date d1, Date d2, char * country, int *counter)
{

	/* base case */
	if(this == NULL)
		return;

	/* Since the desired o/p is sorted, recurse for left subtree first 
    If root->data is greater than k1, then only we can get o/p keys 
    in left subtree */
	if (date_cmp(d1 , this->date) == 0)
		tree_search_dateRange(this->left, d1, d2, counter);

	/* if root's data lies in range, then prints root's data */
	if(((date_cmp(d1, this->date) == 0) || (date_cmp(d1, this->date) == 2)) && ((date_cmp(d2, this->date) ==1) || (date_cmp(d2, this->date) == 2)))
	{	
		Date_Node *temp = this->date_list.head;
		while(temp != NULL)
		{
			if(strcmp(temp->patient_node->patient.country, country)== 0)
				*counter = *counter + 1;
			temp = temp->next;
		}
	}
	
  	/* If root->data is smaller than k2, then only we can get o/p keys 
      in right subtree */
	if(date_cmp(d2 , this->date) == 1)
		tree_search_dateRange(this->right, d1, d2, counter);
}
 
void tree_country_search( Tree_Node * this , char * country, int * counter)
{
	if (this != NULL)
	{
		Date_Node *temp = this->date_list.head;
		while(temp != NULL)
		{	
			if (strcmp(temp->patient_node->patient.country, country) == 0)
				*counter = *counter+1;
			temp = temp->next;			
		}
		tree_country_search(this->left, country, counter);
		tree_country_search(this->right, country, counter);
	}
}
 
void tree_preorder_print(Tree_Node* this) 
{ 
    if (this != NULL) 
    {    	
        print_date(this->date);
		printDatetList(&this->date_list );
        tree_preorder_print(this->left); 
        tree_preorder_print(this->right); 
    } 
} 


void tree_destroy(Tree_Node *tree)
{
	if(tree == NULL)
		return;	

	tree_destroy(tree->left);
	tree_destroy(tree->right);
	freeDateList(&tree->date_list);
	free(tree);
}
/*******************/

/*** Other Functions ***/

int date_cmp(Date d1, Date d2)
{
	if (d1.day == d2.day && d1.month == d2.month && d1.year ==d2.year) 
       return 2; //if two dates are equal

   	else if (d1.year > d2.year || d1.year == d2.year && d1.month > d2.month
   	 		|| d1.year == d2.year && d1.month == d2.month && d1.day > d2.day) 
   		return 1; //if date 1 is bigger than date 2

   	else
   		return 0; //if date 2 is bigger than date 1
}

void print_date(Date date)
{
	printf(" %d-%d-%d", date.day, date.month, date.year);
}

int max(int height1, int height2) 
{ 
    return (height1 > height2) ? height1 : height2; 
} 

int height(Tree_Node *this_node) 
{ 
    if (this_node == NULL) 
        return 0; 

    return this_node->height; 
} 


int getBalance(Tree_Node * this_node) 
{ 
    if (this_node == NULL) 
        return 0; 

    return height(this_node->left) - height(this_node->right); 
}

/******************/
