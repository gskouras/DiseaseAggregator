#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "../Headers/date_list.h"


typedef struct Tree_Node
{
	Date date; /* Date of a Tree */
	Date_List  date_list; /* a List that each node of the List point to the correct Patient Node */
	int height; /* Height of the TreeNode */
	struct Tree_Node *left; /* Pointer to the left leaf of the TreeNode */
	struct Tree_Node *right; /* Pointer to the right leaf of the TreeNode */
} Tree_Node;


/**** Tree Constructors ****/

Tree_Node * createTreeNode( Date );

 /*****************/


/*** Tree Functions ***/

Tree_Node * tree_insert( Tree_Node * , Date , Patient_Node * ); 

Tree_Node * rightRotate( Tree_Node * );

Tree_Node * leftRotate( Tree_Node * ); 

void tree_preorder_print(Tree_Node* );

void tree_search_dateRange( Tree_Node * , Date , Date, int * ); //counts how many incidents of a disease exist in a given range of Dates

void tree_search_Country_dateRange( Tree_Node * , Date , Date, char *, int * ); // counts how many incidents of a diseases exist in a country in a given range

void tree_country_search( Tree_Node * , char *, int * ); //counts how many incidents of a disease exist in a given country

void tree_search_Disease_dateRange ( Tree_Node * , Date , Date , char * , int *);

void tree_disease_search( Tree_Node * this , char * , int * );

void tree_destroy(Tree_Node *tree);

/*******************/

/*** Other Functions ***/

int date_cmp( Date , Date ); /*Compare two given dates. Return 2 if the given Dates are equal, return 1 if date 1 is bigger than date 2*/

void print_date(Date date);

int get_height( Tree_Node * ); /*Get the height of a given TreeNode */

int getBalance(Tree_Node * ); /* Get the Balance of a TreeNode */

int max( int , int ); /* // A utility function to get maximum of two integers */


/******************/

#endif