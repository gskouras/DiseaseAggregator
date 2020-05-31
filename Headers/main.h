#ifndef MAIN_H_
#define MAIN_H_
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <poll.h>
#include <dirent.h> 
#include <signal.h>
#include "pipe.h"
#include "input_list.h"


typedef struct 
{
	int numWorkers;
	int bufferSize;
	char* input_dir;
} Params;


int digitValidate( char *a );

Params inputValidate ( int argc, char *argv[]);

Directory_list* readInputDirectory( Directory_list * , char * );

void initAllPipes( char **, char **, int);

void read_from_workers( Worker_info * , Params );

int checkAllFlags(Worker_info *, int);

void initAllFlags(Worker_info *, int);

void cli( Worker_info * , Params );


/**** Signal Handlers ****/
void worker_handler( int );

int find_worker_pos( pid_t );





#endif

//Vimata gia ton patera
//1)Anoigo to arxeio me tis xwres kai ta diavazo kai gemizo mia lista opou kathe komvos exei to path tis xwras

//2)Dimourgo ta fifos kai gemizo enan pinaka apo worker infos 

//3)Stelno se olous tous workers tis xores pou exoun analavei

//3)Kano poll gia na paro oles tis apantiseis

//4) Command line

//Vimata gia to paidi

//1) Ftiaxno mia sinartisi pou dexetai san orismata ta onomata ton fifos

//2) Kano exec to tropopoiimeno ektelesimo tis protis ergasias me orismata ta read kai write fifo kai to buffersize 

//To paidi perimenei prota na diavasei tis xwres kai gemizei tis domes tou.