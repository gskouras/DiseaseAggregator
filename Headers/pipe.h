#ifndef PIPE_H_
#define PIPE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <fcntl.h> 
#include <unistd.h>
#include <signal.h>
#include "../headers/input_list.h"

#define PERMISSIONS 0666

typedef struct 
{
	pid_t pid;
	Directory_list country_list;
	char *write_fifo;
	char *read_fifo;
	int read_fd;
	int write_fd;
    int flag;
} Worker_info;


void create_named_pipes( char **, char **, int);

void open_write_pipes(int *, char **, int );

int initialize_dirPaths( Directory_list *, Worker_info *, char ** , char ** , int );

int read_from_fifo( int, int);

void write_to_fifo(int, char *);

int worker( char *, char * , int );

void open_manual();

#endif


// create_filenames () {
//   string='';
//   name_len=$(($RANDOM % 8 + 1));
//   for (( j=0; j<name_len; j++));
//   do
//     random_char=$(($RANDOM % 2 + 1));
//     if [ $random_char -eq 1 ]
//     then
//       string+=$(printf \\$(printf '%03o' $(( ( RANDOM % 10 )  + 48 ))));
//     fi
//     if [ $random_char -eq 2 ]
//     then
//       string+=$(printf \\$(printf '%03o' $(( ( RANDOM % 26 )  + 97 ))));
//     fi
//   done;
//   echo $string
// }


// arr[0]="ENTER"
// arr[1]="EXIT"

// rand=$[ $RANDOM % 2
//echo ${arr[$rand]}



    // for (int i = 0; i < params.numWorkers; ++i)
    // {   
    //     workers_array[i].read_fd = open(parentPipes[i], O_RDONLY | O_NONBLOCK);       
    //     write_fds[i] = open(parentPipes[i], O_WRONLY | O_NONBLOCK);

    //     read_fds[i] = open(workerPipes[i], O_RDONLY | O_NONBLOCK);
    //     workers_array[i].write_fd = open(workerPipes[i], O_WRONLY |O_NONBLOCK);


    //     printf("!!!!!!PARENT!!!!!\n");
	   //  printf("Read file is %s and read fd of parent is %d\n",parentPipes[i], workers_array[i].read_fd);
    //     printf("Write file is %s and write_fd of parent is %d\n", workerPipes[i], workers_array[i].write_fd);           
    //  	printf("write_fd of %d is %d\n",i, write_fds[i]);
    //  	printf("read_fd of %d is %d\n",i, read_fds[i]);
    // }
