#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "../Headers/patient_list.h"


typedef struct TreeNode
{
	Date date;
	struct TreeNode *left;
	struct TreeNode *right;
	Patient_Node * patient;
} TreeNode;

typedef struct 
{
	TreeNode * root;
	int counter;
} Tree;


#endif