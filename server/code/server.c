#include "../headers/server.h"
/*** Global Variables ***/
    
// conditional variable, signals a put operation (receiver waits on this)
pthread_cond_t msg_in = PTHREAD_COND_INITIALIZER;
// conditional variable, signals a get operation (sender waits on this)
pthread_cond_t msg_out = PTHREAD_COND_INITIALIZER;
// mutex protecting shared memory resources
pthread_mutex_t job_mutex = PTHREAD_MUTEX_INITIALIZER;

/*************************/

Cycle_Buffer * circular_buffer;

Params params; //parameters of the programm

int main(int argc, char *argv[])
{
	params = inputValidate(argc, argv);

    printf("queryportnum %d, statisticsPortNum %d, numThreads %d, bufferSize %d \n",params.queryPortNum, params.statisticsPortNum, params.numThreads, params.bufferSize );
    int query_sock;
    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    struct hostent *rem;

    if ((query_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(params.queryPortNum);

    /* Bind socket to address */
    if (bind(query_sock, serverptr, sizeof(server)) < 0)
        perror_exit("bind");

    getsockname(query_sock, (struct sockaddr *)&server, &clientlen); //takes the first available port and give it to server
    params.queryPortNum = ntohs(server.sin_port);
    /* Listen for connections */
    if (listen(query_sock, 5) < 0) 
        perror_exit("listen");

    printf("Listening for connections to port %d\n", params.queryPortNum);

    pthread_t threads[params.numThreads];

    init_cycle_buffer();

    printf("Circular buffer State is : job_index = %d count = %d, size = %d\n", circular_buffer->job_index, circular_buffer->count, circular_buffer->size); 
   
    for (int i = 0; i < params.numThreads; ++i)
    {
        pthread_create(&threads[i],NULL,handle_request,NULL);
    }

    int new_sock;
    Job new_job;
    while (1) 
    {
        /* accept connection */
        if ((new_sock = accept(query_sock, clientptr, &clientlen)) < 0) 
            perror("accept");
        // convert to client ip
        struct sockaddr_in *addr_in = (struct sockaddr_in *)clientptr;
        char *s = inet_ntoa(addr_in->sin_addr);

        printf("Accepted connection from %s\n", s);

        new_job.fd = new_sock;

        if(put_job(new_job))
            printf("The new job inserted succesfully\n");

        //close(new_sock); /* parent closes socket to client */
    }

    for (int i = 0; i < params.numThreads; ++i)
    {
        pthread_join(threads[i],NULL);
    }



    return 0;
}


void * handle_request()
{
    Job job;
    while(1)
    {
        job = get_job();
        printf("I took a job with fd = %d\n", job.fd);
        char buffer[100000];
        read(job.fd,buffer,100000);

        printf("Buffer is %s\n",buffer);

        close(job.fd);
    }
    exit(0);
}

Job get_job() 
{
    // lock mutex
    pthread_mutex_lock(&job_mutex);

    while (buffer_isEmpty())   
        pthread_cond_wait(&msg_out,&job_mutex); //while buffer is empty this thread sleeps

    // receive message
    Job job = circular_buffer->job_array[--circular_buffer->job_index];
    if(buffer_isFull())
        circular_buffer->job_index = 0;

    circular_buffer->count--;
    
    // signal the sender that something was removed from buffer
    pthread_cond_signal(&msg_in);
    //printf("new job fd is %d\n", job.fd);
    pthread_mutex_unlock(&job_mutex);

    return(job);
}



/**** Cycle Buffer Functions ***/

int put_job(Job job) //puts a job in the first available position
{

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
        params.numThreads = 2;
        params.bufferSize = 50;
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
