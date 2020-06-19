#ifndef QUERY_HANDLER_H_
#define QUERY_HANDLER_H_

#include <stdio.h>
#include <ctype.h> 
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h> 
#include <dirent.h> 
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/socket.h>              
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>	
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include "./server.h"



void write_to_worker(int , char * , Worker_list *);

int find_worker_country(Worker_list * , char * );

int get_worker_port(Worker_list *, int );

char * get_worker_ip( Worker_list *, int );

int connect_to_worker( int , char * );

int check_for_country( char * );


#endif