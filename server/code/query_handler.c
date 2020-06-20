#include "../headers/query_handler.h"

pthread_mutex_t handler_mutex = PTHREAD_MUTEX_INITIALIZER;

void write_to_worker(int client_fd, char * query, Worker_list *w_list)
{

	char *input = NULL, *line = NULL, *cmd = NULL;
    size_t len = 0, pos = 0;
    ssize_t reads;
    int new_sock, portNum;
    char *ip = NULL;

    input = query;
    cmd = strtok_r(input, " \n", &input);
    input = strtok(input, "\n");

    if (cmd != NULL) 
    {
        if (strcmp(cmd, "/diseaseFrequency") == 0 || strcmp(cmd, "/df") == 0) 
        {
            if (input != NULL && strlen(input) < 20  || input == NULL)
            {
                printf("Please Insert valid Data\n");
                // continue;
            }
            else 
            {
                char temp_query[100];
                memset(temp_query, '\0', sizeof(temp_query));
                strcpy(temp_query, cmd);
                sprintf(temp_query, "%s ", query);
                strcat(temp_query, input);
                // printf("input is %s\n", input);
                if (check_for_country(temp_query)) // Sent Query to All Workers
                {
                    int sum = 0;
                    int list_counter = w_list->counter;
                    while(list_counter > 0)
                    {
                        // printf("stelno se pollous worker\n");
                        portNum = get_worker_port(w_list, pos);
                        ip = get_worker_ip(w_list, pos);

                        // printf("ip of worker is %s\n",ip );
                        new_sock = connect_to_worker( portNum, ip );

						write(new_sock, temp_query, 200);
						char *response = malloc(sizeof(char) * 200);
                        usleep(100);
						read(new_sock,response, 200);
                        close(new_sock);
						// printf("response is %s\n", response);

						sum += atoi(response);
                        list_counter--;
                        pos++;
                    }
                    char total_sum[10];
                    memset(total_sum, '\0', sizeof(total_sum));
                    sprintf(total_sum, "%d", sum);  
                    
                    // pthread_mutex_lock(&handler_mutex);
                    printf("%s\n",temp_query);
                    printf("%s\n", total_sum); 
					write(client_fd, total_sum, 200 );
                    // pthread_mutex_lock(&handler_mutex);                  
                    // send(client_fd,total_sum ,50, 1);
                    sum = 0;                  
                }
                else // Sent Query to the Worker with the specific country
                {
                    // printf("query is(in else)%s\n",query );
                    char temp[50];
                    memset(temp, '\0', sizeof(temp));
                    strcpy(temp, temp_query);
                    char * token = strtok(temp," ");
                    token = strtok(NULL, " ");
                    token = strtok(NULL, " ");
                    token = strtok(NULL, " ");
                    token = strtok(NULL, " ");

                    char * country = malloc(sizeof(char) * strlen(token) + 1);
                    strcpy(country, token);
                   	// printf("Requested country is %s\n", country);


                    int pos = find_worker_country(w_list, country);
                    portNum = get_worker_port(w_list, pos);
                    ip = get_worker_ip(w_list, pos);
                    // printf("I connect to %d worker with portNum %d\n", pos, portNum);

                    if(pos != -1) // it means that country doesnt exist
                    {
                        new_sock = connect_to_worker( portNum, ip );
                        write(new_sock, temp_query, 200);
                        usleep(100);
                        char *response = malloc(sizeof(char) * 200);
                        read(new_sock,response, 200);
                        close(new_sock);

                        // pthread_mutex_lock(&handler_mutex);
                        printf("%s\n",temp_query );
                        printf("%s\n", response); 
						write(client_fd, response, 200 );
                        // pthread_mutex_lock(&handler_mutex);
                    }
                    else
                    {
                        // printf("Error: Requested Country Doesnt Exist\n");
                        write(client_fd, "Requested Country Doesnt Exist", 200 );
                    }
                }
            }

        } 
        else if (strcmp(cmd, "/topk-AgeRanges") == 0 || strcmp(cmd, "/tka") == 0) 
        {
            if (input != NULL && (strlen(input) < 30) || input == NULL)
            {
                printf("Please Insert valid Data \n");
            }
            else
            {
                char temp_query[100];
                memset(temp_query, '\0', sizeof(temp_query));
                strcpy(temp_query, cmd);
                sprintf(temp_query, "%s ", query);
                strcat(temp_query, input);
                // printf("query is %s\n", query);

                // printf("query is %s\n",query );
                char temp[50];
                memset(temp, '\0', sizeof(temp));
                strcpy(temp, temp_query);
                char * token = strtok(temp," ");
                token = strtok(NULL, " ");
                token = strtok(NULL, " ");

                // printf("Country is %s\n",token );

                char * country = calloc(sizeof(char) , strlen(token) + 1);
                memset(country, '\0', sizeof(country));

                strcpy(country, token);
                // printf("Requested country is %s\n", country);
                int pos = find_worker_country(w_list, country);
                portNum = get_worker_port(w_list, pos);
                ip = get_worker_ip(w_list, pos);

                new_sock = connect_to_worker( portNum, ip );
                write(new_sock, temp_query, 200);
                usleep(100);
                char *response = malloc(sizeof(char) * 200);
                read(new_sock,response, 200);
                close(new_sock);
                // pthread_mutex_lock(&handler_mutex);
                // printf("%s\n",temp_query );
                //printf("%s\n", response); 

                // printf("Position is %d\n",pos );
                if(pos != -1)
                {
                    // pthread_mutex_lock(&handler_mutex);
                    printf("%s\n",temp_query );
                    printf("%s\n", response); 
                    write(client_fd, response, 200 );
                    // pthread_mutex_lock(&handler_mutex);
                }
                else
                {
                    // printf("Error: Requested Country Doesnt Exist\n");
                    write(client_fd, "Requested Country Doesnt Exist", 200);
                }                    
            }
        } 
        else if (strcmp(cmd, "/searchPatientRecord") == 0 || strcmp(cmd, "/spr") == 0 ) 
        {
            int flag = 0;
            if (input == NULL || digitValidate(input))
            {
                //printf("Error\n");

                write(client_fd, "Argument Related Error", 200 );
            }
            else
            {
                char temp_query[100];
                memset(temp_query, '\0', sizeof(temp_query));
                strcpy(temp_query, cmd);
                sprintf(temp_query, "%s %s",temp_query, input);
                int list_counter = w_list->counter;
                int pos = 0;
                printf("%s\n",temp_query);

                while(list_counter > 0)
                {
                    portNum = get_worker_port(w_list, pos);
                    ip = get_worker_ip(w_list, pos);

                        // printf("ip of worker is %s\n",ip );
                    new_sock = connect_to_worker( portNum, ip );

                    write(new_sock, temp_query, 200);
                    char *response = malloc(sizeof(char) * 200);
                    usleep(100);

                    read(new_sock,response, 200);
                    close(new_sock);

                    if(strcmp(response, "0"))
                    {   
                        // pthread_mutex_lock(&handler_mutex);
                        printf("%s\n",temp_query );
                        printf("%s\n", response); 
                        write(client_fd, response, 200 );
                        // pthread_mutex_lock(&handler_mutex);
                        flag = 1;
                    }
                    list_counter--;
                    pos++;
                }
                if(flag == 0)
                    write(client_fd, "Patient Doesnt Exist", 200 );   
            }
        } 
        else if ((strcmp(cmd, "/numPatientAdmissions") == 0 || strcmp(cmd, "/npa") == 0)  ||  (strcmp(cmd, "/numPatientDischarges") == 0 || strcmp(cmd, "/npd") == 0))
        {
            if ((input != NULL && strlen(input) < 20 || input == NULL))
            {
                // printf("Please Insert valid Data\n");
                write(client_fd, "Argument Related Error", 50 );
            }
            else 
            {
                char temp_query[100];
                memset(temp_query, '\0', sizeof(temp_query));
                strcpy(temp_query, cmd);
                sprintf(temp_query, "%s %s",temp_query, input);
                // printf("COMMAND IS %s\n", cmd);
                // printf("QUERY IS %s\n",query );
                // printf("INPUT IS %s\n",input );

                // printf("%s from thread id %ld\n", temp_query, pthread_self());

                if (check_for_country(temp_query)) // Sent Query to All Workers
                {
                    int sum = 0;
                    int list_counter = w_list->counter;
                    int pos = 0;
                    char total_response[200];
                    memset(total_response, '\0', sizeof(total_response));
                    // printf("list counter is %d\n", list_counter );
                    while(list_counter > 0)
                    {
                        // printf("stelno se pollous worker\n");
                        portNum = get_worker_port(w_list, pos);
                        ip = get_worker_ip(w_list, pos);

                        // printf("ip of worker is %s\n",ip );
                        new_sock = connect_to_worker( portNum, ip );

                        write(new_sock, temp_query, 200);
                        char *response = malloc(sizeof(char) * 200);
                        usleep(100);

                        read(new_sock,response, 200);
                        // printf("response is %s\n", response);
                        sprintf(total_response, "%s%s", total_response, response);

                        free(response);

                        list_counter--;
                        pos++;
                    }
                    close(new_sock);

                    // printf("%s\n", total_response);
                    // pthread_mutex_lock(&handler_mutex);
                    printf("%s\n",temp_query );
                    printf("%s\n", total_response); 
                    write(client_fd, total_response, 200);
                    // pthread_mutex_lock(&handler_mutex);
                    memset(total_response, '\0', sizeof(total_response));
                }
                else // Sent Query to the Worker with the specific country
                {
                    char temp[50];
                    memset(temp, '\0', sizeof(temp));
                    strcpy(temp, temp_query);
                    // printf("Query is %s\n",temp_query );
                    char * token = strtok(temp," ");
                    token = strtok(NULL, " ");
                    token = strtok(NULL, " ");
                    token = strtok(NULL, " ");
                    token = strtok(NULL, " ");

                    char * country = malloc(sizeof(char) * strlen(token) + 1);
                    memset(country, '\0', sizeof(country));
                    strcpy(country, token);
                    // printf("Requested country is %s\n", country);
                    int pos = find_worker_country(w_list, country);
                    portNum = get_worker_port(w_list, pos);
                    ip = get_worker_ip(w_list, pos);

                    // pthread_mutex_lock(&handler_mutex);
                    // printf("%s\n",temp_query );
                    // printf("Position is %d\n",pos );
                    if(pos != -1)
                    {
                        new_sock = connect_to_worker( portNum, ip );
                        write(new_sock, temp_query, 200);
                        char *response = malloc(sizeof(char) * 200);
                        usleep(100);
                        read(new_sock,response, 200);
                        close(new_sock);
                        // pthread_mutex_lock(&handler_mutex);
                        printf("%s\n",temp_query );
                        printf("%s\n", response); 
                        write(client_fd, response, 200 );
                        // pthread_mutex_lock(&handler_mutex);
                        free(response);
                    }
                    else
                    {
                        // printf("Error: Requested Country Doesnt Exist\n");
                        write(client_fd, "Requested Country Doesnt Exist", 200);
                    }

                    free(country);
                }
            }
        }         
        else 
        {
            fprintf(stdout, "~ error: %s: command not found!\n", cmd);
        }
        
    }
}


int check_for_country( char * query)
{
    char temp[100];
    // printf("query is %s\n", query);
    strcpy(temp, query);
    char * token = NULL;
    int count = 0;

    token = strtok(temp, " ");
    while(token !=NULL)
    {
        token = strtok(NULL, " ");
        count++;
    }
    // printf("counter is %d\n", count);
    if (count == 4)
        return 1;

    if(count == 5)
        return 0;
}


int find_worker_country(Worker_list * w_list, char * country)
{
   // printf("country is(in find worker country) ::: %s\n", country);

    Worker_Node * temp = w_list->head;
    int count = 0;
    while( temp != NULL)
    {
    	for (int i = 0; i < temp->country_count; ++i)
    	{
        	if(strcmp(country, temp->countries[i]) == 0)
            	return count;
    	}
        temp = temp->next;
        count++;
    }
    return -1;
}



int get_worker_port( Worker_list *w_list, int pos)
{
	Worker_Node *current = w_list->head; 

    int count = 0; 
    while (current != NULL) 
    { 
        if (count == pos) 
            return(current->portNum); 
        count++; 
        current = current->next; 
    } 
  
}


char * get_worker_ip( Worker_list *w_list, int pos)
{
	Worker_Node *current = w_list->head; 

    int count = 0; 
    while (current != NULL) 
    { 
        if (count == pos) 
            return(current->ip); 
        count++; 
        current = current->next; 
    } 
  
}


int connect_to_worker( int portNum, char *ip )
{
 	int sock;
 	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr*)&server;
	struct hostent *rem;
    //printf("ip is %s\n", ip);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("sock");
	/* Find server address */
	if ((rem = gethostbyname("127.0.0.1")) == NULL) 
	{ 
			perror("gethostbyname"); 
			exit(1);
	}
	server.sin_family = AF_INET; /* Internet domain */
	memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
	server.sin_port = htons(portNum); /* Server port */
	if (connect(sock, serverptr, sizeof(server)) < 0)
    {
	    perror("Error when connecting to worker ");
    }
	return sock;
}
