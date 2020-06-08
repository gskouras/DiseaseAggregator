#include "../headers/server.h"
/*** Global Variables ***/
    
// conditional variable, signals a put operation (receiver waits on this)
pthread_cond_t put_in = PTHREAD_COND_INITIALIZER;
// conditional variable, signals a get operation (sender waits on this)
pthread_cond_t get_out = PTHREAD_COND_INITIALIZER;
// mutex protecting shared memory resources
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*************************/

int main(int argc, char *argv[])
{
	Params params = inputValidate(argc, argv);

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

    int new_sock;
    while (1) 
    {
        /* accept connection */
        if ((new_sock = accept(query_sock, clientptr, &clientlen)) < 0) 
            perror("accept");
        // convert to client ip
        struct sockaddr_in *addr_in = (struct sockaddr_in *)clientptr;
        char *s = inet_ntoa(addr_in->sin_addr);

        printf("Accepted connection from %s\n", s);

        char buffer[10000];
        read(new_sock,buffer,10000);

        printf("Buffer is %s\n",buffer);

        close(new_sock); /* parent closes socket to client */
    }

    return 0;
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
        params.queryPortNum = 8000;
        params.statisticsPortNum = 9000;
        params.numThreads = 2;
        params.bufferSize = 5;
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
