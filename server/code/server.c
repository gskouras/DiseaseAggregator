#include "../headers/server.h"
/*** Global Variables ***/
    
// conditional variable, signals a put operation (receiver waits on this)
pthread_cond_t msg_in = PTHREAD_COND_INITIALIZER;
// conditional variable, signals a get operation (sender waits on this)
pthread_cond_t msg_out = PTHREAD_COND_INITIALIZER;
// mutex protecting shared memory resources
pthread_mutex_t job_mutex = PTHREAD_MUTEX_INITIALIZER;
// mutex for print messages in the right order
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

Cycle_Buffer * circular_buffer;

Params params; //parameters of the programm

Worker_list w_list;

/*************************/
int main(int argc, char *argv[])
{
	params = inputValidate(argc, argv);

    initWorkertList(&w_list);

    printf("queryportnum %d, statisticsPortNum %d, numThreads %d, bufferSize %d \n",params.queryPortNum, params.statisticsPortNum, params.numThreads, params.bufferSize );
    int worker_sock, client_sock;
    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    struct hostent *rem;

    //create_passive_endpoints(&serverptr,&clientptr,&server, &client,&worker_sock, &client_sock, &clientlen, params);
    if ((worker_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 || (client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        perror_exit("socket");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(params.statisticsPortNum);

    /* Bind socket to address */
    if (bind(worker_sock, serverptr, sizeof(server)) < 0) 
        perror_exit("bind");

    server.sin_port = htons(params.queryPortNum);

    if (bind(client_sock, serverptr, sizeof(server)) < 0) 
        perror_exit("bind");

    getsockname(worker_sock, (struct sockaddr *)&server, &clientlen); //takes the first available port and give it to server
    params.statisticsPortNum = ntohs(server.sin_port);

    getsockname(client_sock, (struct sockaddr *)&server, &clientlen); //takes the first available port and give it to server
    params.queryPortNum = ntohs(server.sin_port);
    /* Listen for connections */
    if (listen(worker_sock, 10) < 0 || listen(client_sock, 10) < 0) 
        perror_exit("listen");

    printf("Listening for worker connections to port %d\n", params.statisticsPortNum);
    printf("Listening for client connections to port %d\n", params.queryPortNum);

    pthread_t threads[params.numThreads];

    init_cycle_buffer();
   
    for (int i = 0; i < params.numThreads; ++i)
    {
        pthread_create(&threads[i],NULL,handle_request,NULL);
    }

    int new_sock;
    Job new_job;

    Socket_fd socket_fds[FD_POOL];

    while (1) 
    {
        new_sock = accept_connection(&clientptr, &clientlen, socket_fds,  worker_sock, client_sock);

        new_job.fd = new_sock;

        if(!put_job(new_job))
            printf("Error Occured while inserting a new job to the Cycle_Buffer\n");
    }

    for (int i = 0; i < params.numThreads; ++i)
    {
        pthread_join(threads[i],NULL);
    }

    return 0;
}


int accept_connection(struct sockaddr** clientptr,  socklen_t *clientlen, Socket_fd * socket_fds, int worker_sock, int client_sock)
{
    struct pollfd fdarray[FD_POOL];
    initSocketFd( socket_fds, worker_sock, client_sock );
    int new_sock;
    int i;
    while(checkAllFlags(socket_fds))
    {
        for (i = 0; i < FD_POOL; i++)
        {
            fdarray[i].fd = socket_fds[i].fd;
            fdarray[i].events = POLLIN;
        }

        poll(fdarray, FD_POOL, -1);
        //printf("Infinite\n");
        for (i = 0; i < FD_POOL; i++)
        {
            
            if((fdarray[i].revents & POLLIN))
            {
                if(fdarray[i].fd ==  socket_fds[i].fd)
                {   
                    /* accept connection */
                    if ((new_sock = accept(socket_fds[i].fd, *clientptr, clientlen)) < 0)
                            perror("accept");
                    // convert to client ip
                    struct sockaddr_in *addr_in = (struct sockaddr_in *)clientptr;
                    char *s = inet_ntoa(addr_in->sin_addr);

                    //printf("Accepted connection from %s\n", s);
                    socket_fds[i].flag = 0;
                    return new_sock;                    
                }               
            }
            else if((fdarray[i].revents & POLLHUP)) 
                socket_fds[i].flag = 1;
        }
    }
}


int create_passive_endpoints(struct sockaddr** serverptr, struct sockaddr** clientptr, struct sockaddr_in *server, struct sockaddr_in *client, int * worker_sock, int *client_sock, socklen_t *clientlen, Params params)
{
 if ((*worker_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 || (*client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        perror_exit("socket");

    server->sin_family = AF_INET;
    server->sin_addr.s_addr = htonl(INADDR_ANY);
    server->sin_port = htons(params.statisticsPortNum);

    /* Bind socket to address */
    if (bind(*worker_sock, *serverptr, sizeof(*server)) < 0) 
        perror_exit("bind");

    getsockname(*worker_sock, (struct sockaddr *)&server, clientlen); //takes the first available port and give it to server
    params.statisticsPortNum = ntohs(server->sin_port);

    server->sin_port = htons(params.queryPortNum);

    if (bind(*client_sock, *serverptr, sizeof(*server)) < 0) 
        perror_exit("bind");

    getsockname(*client_sock, (struct sockaddr *)&server, clientlen); //takes the first available port and give it to server
    params.queryPortNum = ntohs(server->sin_port);
    /* Listen for connections */
    if (listen(*worker_sock, 10) < 0 || listen(*client_sock, 10) < 0) 
        perror_exit("listen");

    printf("Listening for worker connections to port %d\n", params.statisticsPortNum);
    printf("Listening for client connections to port %d\n", params.queryPortNum);

    return 1;
}

/*** Request Handling Functions ***/

void * handle_request()
{
    Job job;
    while(1)
    {
        job = get_job();

        if(job.flag == 1) //it means that the job came from a worker
            handle_worker(job);

        else if(job.flag == 0) //it means tha the job came from a client
            handle_client(job);

        //close(job.fd);
    }
    exit(0);
    //close(job.fd);
}



void handle_client(Job job)
{
    // pthread_mutex_lock(&print_mutex);
    static int i = 1;
    char mess[100];
    read(job.fd, mess, 100);
    //printf("i is %d ### %s\n",i, mess );
    char * response = write_to_worker(job.fd, mess, &w_list);
    // pthread_mutex_unlock(&print_mutex);
    write(job.fd, response, 50);
   //free(response);
    close(job.fd);
    i++;
}


void handle_worker(Job job)
{
    char message[100000];
    read(job.fd,message,100000);

    char ** countries_array = NULL;
    int count = 0;

    char * stats = strtok(message, "$");
    // char *countries = strtok(message, "$");
    char *countries = strtok(NULL, "@");
    char *temp_countries = malloc(sizeof(char) * strlen(countries));
    strcpy(temp_countries, countries);

    char *port = strtok(NULL, "\n");
    int portnum = atoi(port);

    int size = count_countries(countries);

    countries_array = malloc(sizeof(char *) * size);
    for (int i = 0; i < size; ++i)
    {
        countries_array[i] = malloc(sizeof(char) * 50);
    }

    countries = strtok(temp_countries, "$");
    while(countries != NULL)
    {
        //printf("country is %s\n", countries);
        countries_array[count] = countries;
        count++;
        countries = strtok(NULL, "$");
    }

    pthread_mutex_lock(&print_mutex);
    //printf("\n\nI took a job with fd = %d MY ID IS %ld\n", job.fd, pthread_self());
    //printf("%s\n",stats);

    insertNewWorker(&w_list, countries_array, portnum, size);

    pthread_mutex_unlock(&print_mutex);

    free(temp_countries);
}


int count_countries( char* string)
{
    //printf("countries are %s\n", string);
    int count = 0;
    int i;

    int length = strlen(string);

    for (i = 0; i < length; i++)
    {
        if (string[i] == '$')
        {
            count++;
        }
    }

    return count + 1;
}


/*****************************/

/**** Cycle Buffer Functions ***/

Job get_job() 
{

    pthread_mutex_lock(&job_mutex);

    while (buffer_isEmpty())   
        pthread_cond_wait(&msg_out,&job_mutex); //while buffer is empty this thread sleeps

    // receive message
    Job job = circular_buffer->job_array[--circular_buffer->job_index];

    if(buffer_isFull())
        circular_buffer->job_index = 0;

    circular_buffer->count--;

    char temp[11];

    read(job.fd, temp, 10);

    //printf("\n\ntemp is %s\n", temp);

    char * token = strtok(temp, "$");

    if(strcmp(token, "w") == 0)
        job.flag = 1;

    if(strcmp(token, "c") == 0)
        job.flag = 0;
    
    // signal the sender that something was removed from buffer
    pthread_cond_signal(&msg_in);
    pthread_mutex_unlock(&job_mutex);

    return(job);
}

int put_job(Job job) //puts a job in the first available position
{
    usleep(10);
    pthread_mutex_lock(&job_mutex);

    while (buffer_isFull())  //while buffer is full this thread sleeps
        pthread_cond_wait(&msg_in,&job_mutex);   

    circular_buffer->job_array[circular_buffer->job_index++] = job;
    circular_buffer->count++;
    pthread_cond_signal(&msg_out);
    pthread_mutex_unlock(&job_mutex);
    
    return 1;
}

void init_cycle_buffer()
{
    circular_buffer = malloc(sizeof(Cycle_Buffer));
    circular_buffer->job_array = malloc(sizeof(Job) * params.bufferSize);
    circular_buffer->job_index = 0;
    circular_buffer->count = 0;
    circular_buffer->size = params.bufferSize;
}

int buffer_isEmpty()
{
    if(circular_buffer->count == 0)
        return 1;

    return 0;
}

int buffer_isFull()
{
    if(circular_buffer->count == circular_buffer->size)
        return 1;

    return 0;
}

/*************************/

/*** Utillity Functions ***/

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
        params.queryPortNum = 8000;
        params.statisticsPortNum = 9000;
        params.numThreads = 1;
        params.bufferSize = 4;
        return params;
    }

    else
    { 
    	if ( argc != 9 )
    	{
    		printf("Error. Arguement related error: Got %d, expectetd 7\n", argc);
    		exit(0);
    	}

    	if ( (strcmp(argv[1], "-q") == 0 && strcmp(argv[3], "-s") == 0 && strcmp(argv[5], "-w") == 0  && strcmp(argv[7], "-b") == 0)
    	&& !digitValidate(argv[2]) && !digitValidate(argv[4])  && !digitValidate(argv[6])  && !digitValidate(argv[8]))
    	{
            params.queryPortNum = atoi(argv[2]);
            params.statisticsPortNum = atoi(argv[4]);
            params.numThreads = atoi(argv[6]);
            params.bufferSize = atoi(argv[8]);
        	return params;
    	}
    	else 
    	{
    		printf("Error. Arguement related error.\n");
    		exit(0);
    	}
    }
}


void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}


int checkAllFlags(Socket_fd * socket_fds) //return 1 oso kati den exei diavastei
{
    for (int i = 0; i < FD_POOL; ++i)
    {
        if(socket_fds[i].flag != 1)
            return 0;
    }
    return 1;
}

void initSocketFd(Socket_fd * socket_fds, int worker_sock, int client_sock)
{
    socket_fds[0].fd = worker_sock;
    socket_fds[1].fd = client_sock;
    for (int i = 0; i < FD_POOL; ++i)
    {
        socket_fds[i].flag = 1; //1 simainei oti den exei diavastei
    }
}

/*************************************/

