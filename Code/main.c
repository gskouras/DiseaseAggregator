#include "../headers/main.h"


int main(int argc, char* argv[])
{

	signal(SIGCHLD, worker_handler);

	Params params = inputValidate(argc, argv);

	Directory_list d_list;

	readInputDirectory(&d_list, params.input_dir);
	//printDirectoryList(&d_list);
	
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
			if(mkfifo(parentPipes[i], PERMISSIONS)<0){
				perror("Error creating the named pipe \n");
				exit(1);
			}
		}
		if(stat(workerPipes[i],&st)==-1)
		{
			if(mkfifo(workerPipes[i], PERMISSIONS)<0){
				perror("Error creating the named pipe \n");
				exit(1);
			}
		}
	}

	Worker_info workers_array[params.numWorkers];	

	int id_counter = 0;


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
			//printf("%s %s\n", parentPipes[i], workerPipes[i]);
			workers_array[i].write_fifo = malloc(strlen(parentPipes[i])+1);
			workers_array[i].read_fifo = malloc(strlen(workerPipes[i])+1);
			initDirectorytList(&workers_array[i].country_list); 
			strcpy( workers_array[i].write_fifo, parentPipes[i]);
			strcpy (workers_array[i].read_fifo, workerPipes[i]);					 
		}
	}

	//printf("Worker arrays info data..\n");

	initialize_dirPaths(&d_list, workers_array, parentPipes, workerPipes, params.numWorkers);

	read_from_workers(workers_array, params);

	printf("\nParse of file Completed Succesfully!\n");
	printf("\nWelcome to diseaseAggregator CLI\n");

    cli(workers_array, params);

	//1.getline
	//2.grapse sta fifos tin entoli(na tsekaro an iparxei country. an iparxei tha stelno se 1)
	//3. kalo tin read from workers // i tin sketi read an esteila se 1

	//4.auto ginetai mexri na lavo exit.

	for (int i = 0; i < params.numWorkers; ++i)
	{
	 	unlink(parentPipes[i]);
		unlink(workerPipes[i]);
	}
    for (int i = 0; i < params.numWorkers; ++i)
    {
    	pid = wait(&status);
    	//printf("%d\n", status);
    }
	
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
        params.numWorkers = 8;
        params.bufferSize = 512;
        params.input_dir = malloc(sizeof(50));
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
	char *argv[]={"./workerz", 
    read_pipe, write_pipe, buffer , NULL}; 
    execvp(argv[0],argv);
}


void worker_handler( int sig)
{
	printf("I caught a signal!\n");
}





