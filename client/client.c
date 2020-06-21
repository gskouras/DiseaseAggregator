#include "client.h"


// conditional variable, signals a send operation (sender waits on this)
pthread_cond_t send_message = PTHREAD_COND_INITIALIZER;
// mutex protecting shared memory resources
pthread_mutex_t query_mutex = PTHREAD_MUTEX_INITIALIZER;

int flag = 0;

Params params;

Query_array *q_array;

int main(int argc, char *argv[])
{
	params = inputValidate(argc, argv);

    int queries = count_queries(params.queryFile);

    readQueryFile(params);

    int thread_id = 0, counter = 0;
    
    int remaining_queries = q_array->total;
    pthread_t  threads[params.numThreads];

    printf("Number of Queries is %d and number of Threads is %d\n\n", queries, params.numThreads);
    printf("Queries will be sent to the Server in %d Batches of %d\n", queries/params.numThreads, params.numThreads);

    printf("\nProccedure Starts in 3\n");sleep(1);printf("2\n");sleep(1);printf("1\n");sleep(1);printf("\nGO!\n\n");

    for (int i = 0; i < q_array->total; ++i)
    {
        flag = 0;       
        while(params.numThreads > counter)
        {
            if(remaining_queries >= params.numThreads)
            {
                // printf("||||%s|||\n",  q_array->queries[i]);
               pthread_create(&threads[thread_id], NULL, send_query, q_array->queries[i]);

                thread_id++;
                i++; 
                counter++;

                if(params.numThreads == counter)
                {   
                    // printf("\n");
                    flag = 1;
                    pthread_cond_broadcast(&send_message);
                    printf("\n\n");
                    if(remaining_queries >= params.numThreads)
                    {
                        // printf("remaining_queries are %d\n",remaining_queries );
                        for (int i = 0; i < params.numThreads; ++i)
                        {
                            pthread_join(threads[i], NULL);
                            remaining_queries--;
                            //printf("Perimeno na teliosei to %d thread me id %d\n",i, thread_id );
                        }                   
                    }
                    thread_id = 0;
                }
            } 
            else
            {      
                if(remaining_queries == 0)
                    client_exit();

                for (int j = i; j < q_array->total; j++)
                {
                    pthread_create(&threads[thread_id], NULL, send_query, q_array->queries[j]);  
                }
                flag = 1;
                pthread_cond_broadcast(&send_message);
                printf("\n\n");
                for (int i = 0; i < remaining_queries; ++i)
                {
                    pthread_join(threads[i], NULL);
                    remaining_queries--;
                }
            }      
        }
        i--;
        counter = 0;

    }

	return 0;
}


void * send_query(void * args)
{
    //printf("Irtha na perimeno me thread id %ld ::: %s\n", pthread_self(), (char*)args);
    // printf("Flag is %d\n", flag );
    pthread_mutex_lock(&query_mutex);
    while(flag == 0)
    {
        pthread_cond_wait(&send_message, &query_mutex); //while buffer is empty this thread sleeps
    }
    pthread_mutex_unlock(&query_mutex);


    int sock;
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;

    // /* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("socket");
    /* Find server address */
    if ((rem = gethostbyname(params.servIP)) == NULL) 
    { 
       perror("gethostbyname"); 
       exit(1);
    }
    server.sin_family = AF_INET;  /*Internet domain*/ 
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(params.servPort); /* Server port */
    if (connect(sock, serverptr, sizeof(server)) < 0)
        perror("connect");

    // printf("Connecting to %s port %d\n", params.servIP, params.servPort);
    write_to_socket(sock, (char *)args);
    char response[200];
    
    pthread_mutex_lock(&query_mutex);
    read(sock, response, 200);
    printf("%s\n", (char*)args);
    printf("%s\n", response);
    // printf("%ld : %s\n", pthread_self(), (char *)args );
    close(sock);
    pthread_mutex_unlock(&query_mutex);
    pthread_exit(NULL);
}




void readQueryFile(Params params)
{
    int queries = count_queries(params.queryFile);

    FILE *fp = fopen(params.queryFile, "r");

    if (fp == NULL) //error handling when opening the file
    {
        perror(" Requested file failed to open");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    pthread_t thread[params.numThreads];

    initQueryArray(queries);
    int i = 0;
    int count = 0;
    while ((nread = getline(&line, &len, fp)) != -1) 
    {   
        char * query = strtok(line, "\r\n");
        query[strlen(query) + 1] = '\0';
        strcpy(q_array->queries[count] , query);
        count++;
    }  

    free(line);
    fclose(fp);
}

void write_to_socket(int socket_fd, char * message)
{
    int message_len = strlen(message);
    char temp[11];
    memset(temp, '\0', sizeof(temp));

    sprintf(temp, "c$"); //w stands for worker
    //printf("message_len is %d\n", message_len);
    write(socket_fd, temp, 10);
    //printf("message is %s\n",message );
    write(socket_fd, message, 100);
}

void initQueryArray(int size)
{
    q_array = malloc(sizeof(Query_array));
    q_array->queries = malloc(sizeof(char*) * size);
    for(int i = 0; i < size; ++i)
    {
        q_array->queries[i] = calloc(sizeof(char) , 100);
    }
    q_array->total = size;
    q_array->current = 0;
}

int count_queries(char *queryFile)
{
    int count = 0;
    FILE *fp = fopen(queryFile, "r");

    if (fp == NULL) //error handling when opening the file
    {
        perror(" Requested file failed to open");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&line, &len, fp)) != -1) 
    {   
        count++;
    }

    free(line);
    fclose(fp);
    return count;
}





Params inputValidate (int argc, char *argv[])
{
    Params params;
    
    if(argc==1)
    {
        params.queryFile = malloc(sizeof(char) *25);
        strcpy(params.queryFile, "./resources/queryFile");
        params.numThreads = 5;
        params.servPort = 8000;
        params.servIP = malloc(sizeof(char) * 20);
        strcpy(params.servIP, "127.0.0.1");
        return params;
    }

    else
    { 
    	if ( argc != 9 )
    	{
    		printf("Error. Arguement related error: Got %d, expectetd 7\n", argc);
    		exit(0);
    	}

    	if ( (strcmp(argv[1], "-q") == 0 && strcmp(argv[3], "-w") == 0 && strcmp(argv[5], "-sp") == 0 && strcmp(argv[7], "-sip") == 0)
    	&& !digitValidate(argv[4]) && !digitValidate(argv[6]) )
    	{
        	params.queryFile = malloc(sizeof(char) *strlen(argv[2])+1);
        	strcpy(params.queryFile, argv[2]);
    		params.numThreads = atoi(argv[4]);
        	params.servPort = atoi(argv[6]);
        	params.servIP = malloc(sizeof(char) *strlen(argv[8])+1);
        	strcpy(params.servIP, argv[8]);
        	return params;
    	}
    	else 
    	{
    		printf("Error. Arguement related error.\n");
    		exit(0);
    	}
    }
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


void freeQueriesArray()
{
    for (int i = 0; i < q_array->total; ++i)
    {
        free(q_array->queries[i]);
    }
    free(q_array->queries);
    free(q_array);
}


void client_exit()
{
    freeQueriesArray();
    free(params.queryFile);
    free(params.servIP);
    exit(0);
}