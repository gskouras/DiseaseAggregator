#include "../Headers/tree.h"

/**** Tree Constructors ****/

void TreeInitialize( Tree * tree)
{
	tree->root = NULL;
	int counter = 0;
}


Tree_Node * createTreeNode()
{
	Tree_Node * new_node = malloc(sizeof(Tree_Node));
	int height = 0;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->patient = NULL;
	return new_node;
}


/*****************/


/*** Tree Functions ***/


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
