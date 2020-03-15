#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "../Headers/date_list.h"


typedef struct Tree_Node
{
	Date date;
	Date_List  date_list;
	int height;
	struct Tree_Node *left;
	struct Tree_Node *right;
} Tree_Node;


/**** Tree Constructors ****/


Tree_Node * createTreeNode( Date );

 /*****************/


/*** Tree Functions ***/

Tree_Node * tree_insert( Tree_Node * , Date , Patient_Node * );

Tree_Node *rightRotate( Tree_Node * );

Tree_Node *leftRotate( Tree_Node * ); 

void tree_preorder_print(Tree_Node* root);


/*******************/

/*** Other Functions ***/

int date_cmp( Date , Date );

void print_date(Date date);

int height( Tree_Node * );

int getBalance(Tree_Node * );

int max( int , int );

/******************/

#endif