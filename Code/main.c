#include "../headers/main.h"

Params params;	
Worker_info * workers_array = NULL;

int main(int argc, char* argv[])
{

	signal(SIGCHLD, worker_handler);

	Params params = inputValidate(argc, argv);

	Directory_list d_list;

	readInputDirectory(&d_list, params.input_dir);

	workers_array = calloc(sizeof(Worker_info) , params.numWorkers);

	pid_t pid;
	int status;
	char ** parentPipes;
	char ** workerPipes;

	parentPipes = malloc(params.numWorkers *sizeof(char*));
	workerPipes = malloc(params.numWorkers *sizeof(char*));

	struct stat st;
	for (int i = 0; i < params.numWorkers; ++i)
	{
		parentPipes[i] = malloc(50* sizeof(char));
		workerPipes[i] = malloc(50*  sizeof(char));
		snprintf(parentPipes[i], strlen("/tmp/parent_fifo%d\0"), "/tmp/parent_fifo%d", i);
		snprintf(workerPipes[i], strlen("/tmp/worker_fifo%d\0"), "/tmp/worker_fifo%d", i);
		if(stat(parentPipes[i],&st)==-1)
		{
			if(mkfifo(parentPipes[i], PERMISSIONS)<0)
			{
				perror("Error creating the named pipe \n");
				exit(1);
			}
		}
		if(stat(workerPipes[i],&st)==-1)
		{
			if(mkfifo(workerPipes[i], PERMISSIONS)<0)
			{
				perror("Error creating the named pipe \n");
				exit(1);
			}
		}
	}

	for (int i = 0; i < params.numWorkers; ++i)
	{
		pid = fork();

		if (pid == 0)
		{
			worker(parentPipes[i], workerPipes[i], params.bufferSize);
		}
		else
		{
			workers_array[i].pid = pid;
			workers_array[i].write_fifo = malloc(sizeof(char) * strlen(parentPipes[i])+1);
			workers_array[i].read_fifo = malloc(sizeof(char) * strlen(workerPipes[i])+1);
			initDirectorytList(&workers_array[i].country_list); 
			strcpy( workers_array[i].write_fifo, parentPipes[i]);
			strcpy (workers_array[i].read_fifo, workerPipes[i]);					 
		}
	}
	printf("Sending Information to Workers...\n");

	initialize_dirPaths(&d_list, workers_array, parentPipes, workerPipes, params.numWorkers);

	read_from_workers(workers_array, params);

	printf("\nWorkers Received and Saved Requested Data Succesfully!\n");
	printf("\nWelcome to Disease Aggregator Command Line Interface\n");

    cli(workers_array, params);

    for (int i = 0; i < params.numWorkers; ++i)
    {
    	pid = wait(&status);
    	unlink(parentPipes[i]);
		unlink(workerPipes[i]);
		free(parentPipes[i]);
		free(workerPipes[i]);
		free(workers_array[i].write_fifo);
		free(workers_array[i].read_fifo);
		freeDirList(&workers_array[i].country_list);
    	//printf("%d\n", status);
    }
    free(parentPipes);
    free(workerPipes);
    free(workers_array);
    freeDirList( &d_list);
    free(params.input_dir);	
	return 0;
}

void read_from_workers( Worker_info * workers, Params params)
{
	struct pollfd fdarray[params.numWorkers];
	int i;
	initAllFlags(workers, params.numWorkers);

	char * message;

    while(checkAllFlags(workers, params.numWorkers))
    {
     	for (i = 0; i < params.numWorkers; i++)
     	{
     		fdarray[i].fd = workers[i].read_fd;
     		fdarray[i].events = POLLIN;
     	}

     	poll(fdarray, params.numWorkers, -1);
     	//printf("Infinite\n");
     	//sleep(1);
     	for (i = 0; i < params.numWorkers; i++)
     	{
     		
     		if((fdarray[i].revents & POLLIN))
     		{
	            if(fdarray[i].fd == workers[i].read_fd)
	            {
	            	//printf("start to print the message with i %d\n", i);
	               	while(read_from_fifo( workers[i].read_fd , params.bufferSize));
	               	//free(message);
	               	// printf("message is over \n");               		
	               	workers[i].flag = 0;	               	
	            }	            
        	}
        	else if((fdarray[i].revents & POLLHUP)) // exei kleisei to pipe apo to allo meros
                workers[i].flag = 1;
     	}
      	//break;
    }
}

int checkAllFlags(Worker_info * workers, int numWorkers) //return 1 oso kati den exei diavastei
{
	for (int i = 0; i < numWorkers; ++i)
	{
		if(workers[i].flag != 1)
			return 0;
	}
	return 1;
}

void initAllFlags(Worker_info * workers, int numWorkers)
{
	for (int i = 0; i < numWorkers; ++i)
	{
		workers[i].flag = 1; //1 simainei oti den exei diavastei
	}
}



Directory_list* readInputDirectory(Directory_list *d_list, char * input)
{
	struct dirent *de;
	char temp[100];
	strcpy(temp, input);

 	initDirectorytList(d_list);

	DIR *dr = opendir(input);

	//printf("To directory einai %s\n",input );

	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        perror("Error");
        exit(0);
    } 
    strcat(input, "/");
    while ((de = readdir(dr)) != NULL)
    {
    	strcpy(temp, input);
    	if(strcmp(de->d_name, ".") && strcmp(de->d_name, ".."))
            insertNewDirectory( d_list, strcat(temp, de->d_name));
    }
  
    closedir(dr);   
    return d_list; 
}


int digitValidate(char *a)
{
	for (unsigned int i = 0; i < strlen(a); i++)
	{
		if(!isdigit(a[i]))
			return -1;
	}
	return 0;
}


Params inputValidate (int argc, char *argv[])
{
    Params params;
    
    if(argc==1)
    {
        params.numWorkers = 1;
        params.bufferSize = 512;
        params.input_dir = malloc(sizeof(char) *25);
        strcpy(params.input_dir, "./resources/input_dir");
        return params;
    }

    else
    { 
    	if ( argc != 7 )
    	{
    		printf("Error. Arguement related error: Got %d, expectetd 7\n", argc);
    		exit(0);
    	}

    	if ( (strcmp(argv[1], "-w") == 0 && strcmp(argv[3], "-b") == 0 && strcmp(argv[5], "-i") == 0)
    	&& !digitValidate(argv[2]) && !digitValidate(argv[4]) )
    	{
    		params.numWorkers = atoi(argv[2]);
        	params.bufferSize = atoi(argv[4]);
        	params.input_dir = malloc(sizeof(char) *strlen(argv[6])+1);
        	strcpy(params.input_dir, argv[6]);
        	return params;
    	}
    	else 
    	{
    		printf("Error. Arguement related error.\n");
    		exit(0);
    	}
    }
}

int worker(char * read_pipe, char * write_pipe, int bufferSize)
{
	
	char buffer[10];//printf("worker will start with procces id %u and %s %s\n", getpid(), read_pipe, write_pipe);
	sprintf(buffer, "%d", bufferSize);
	char *argv[]={"./workers", 
    read_pipe, write_pipe, buffer , NULL}; 
    execvp(argv[0],argv);
}

int find_worker_pos(pid_t pid)
{
	for (int i = 0; i < params.numWorkers; ++i)
	{
		if(workers_array[i].pid == pid)
			return i;
	}
}

void worker_handler( int sig )
{
	pid_t pid, new_pid;
    int status;
    int index;

	pid = waitpid(-1, &status, WNOHANG);
	index = find_worker_pos(pid);

	//printf("index of procces id %d which died is %d\n",pid, index );
	if(sig == SIGCHLD)
	{
		new_pid = fork();

		if (new_pid == 0)
		{
			worker(workers_array[index].write_fifo, workers_array[index].read_fifo, params.bufferSize);
		}
		sleep(1);
		// workers_array[index].write_fd = open(workers_array[index].write_fifo, O_WRONLY | O_NONBLOCK);
	 //    workers_array[index].read_fd =  open(workers_array[index].read_fifo, O_RDONLY | O_NONBLOCK); 
	    if(workers_array[index].country_list.counter > 1) // an o sigekrimenos worker exei parapano apo mia xores
		{
	        char message_buffer[1000]="";
	        char temp_buffer[1000]="";

	        memset( (void *)message_buffer, '\0', sizeof(message_buffer));
	        memset( (void *)temp_buffer, '\0', sizeof(temp_buffer));

	        int list_counter = workers_array[index].country_list.counter;
	        int pos = 0;
			while (list_counter > 0) //oso i lista den einai keni
			{
				CountryPath_Node *temp =  get_country(&workers_array[index].country_list, pos); //pairno apo to lista to proto path
	            //printf("tenp country is %s\n", temp->country_path);
	            if(strlen(temp_buffer) > 1 )
	                sprintf(temp_buffer, "%s$", temp_buffer);//vazo dolario gia na ta ksexwriso meta me strtok

				strcat(temp_buffer, temp->country_path); //vazo tin epomeni xwra pou exei i lista
	            pos++;
	            list_counter--;
			}

	        strcat(message_buffer, temp_buffer); //ston synoliko message buffer vazo ton temp
	        //printf("message_buffer is %s\n", message_buffer );
	        //printf("i am writing to %s\n",workers_array[index].write_fifo );
	        write_to_fifo(workers_array[index].write_fd, message_buffer); //grafo se olous tous worker ta path me tis xores
			
	        //printf("writed from ifto %s countries %s\n",workers_array[index].write_fifo, message_buffer );
		}
		else //an exeis mono mia xora apla partin apo ti lista kai grapstin sto fifo
		{
	        char message_buffer[1000] = "";
	        char temp_buffer[1000] = "";
			CountryPath_Node *temp =  get_country(&workers_array[index].country_list, 0);
			strcat(message_buffer, temp->country_path);
			write_to_fifo(workers_array[index].write_fd, message_buffer);
		}
	}	
}





