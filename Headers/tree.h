#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "../Headers/patient_list.h"


typedef struct Tree_Node
{
	Date date;
	int height;
	struct Tree_Node *left;
	struct Tree_Node *right;
	Patient_Node * patient;
} Tree_Node;

typedef struct 
{
	Tree_Node * root;
	int counter;
} Tree;

/**** Tree Constructors ****/

void TreeInitialize( Tree * );

Tree_Node * createTreeNode();

 /*****************/


/*** Tree Functions ***/


Tree_Node *rightRotate( Tree_Node * );

Tree_Node *leftRotate( Tree_Node * ); 

/*******************/



/*** Other Functions ***/

int date_cpm( Date , Date );

int height( Tree_Node * );

int getBalance(Tree_Node * );

int max( int , int );

/******************/

#endif