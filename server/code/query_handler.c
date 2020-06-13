#include "../headers/query_handler.h"


char * write_to_worker(int fd, char * query, Worker_list *w_list)
{

	char *input = NULL, *line = NULL, *cmd = NULL;
    size_t len = 0, pos = 0;
    ssize_t reads;

    input = query;
    cmd = strtok_r(input, " \n", &input);
    input = strtok(input, "\n");
    char temp_query[100];

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
                strcpy(temp_query, cmd);
                sprintf(temp_query, "%s ", query);
                strcat(temp_query, input);
                printf("query is %s\n", temp_query);
                //printf("input is %s\n", input);

                if (strlen(temp_query) < 100) // Sent Query to All Workers
                {
                    int sum = 0;
                    int list_counter = w_list->counter;
                    while(list_counter > 0)
                    {
                        //printf("stelno se pollous worker\n");

                        int portNum = get_worker_port(w_list, pos);

                        //printf("portnum is %d\n",portNum );

                     	int sock;
                     	struct sockaddr_in server;
    					struct sockaddr *serverptr = (struct sockaddr*)&server;
    					struct hostent *rem;

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
						    perror("connect");

						write(sock, temp_query, 200);
						char *response = malloc(sizeof(char) * 50);
						read(sock,response, 50);

						printf("response is %s\n", response);

                        // usleep(1000);

                        // char *temp = malloc(sizeof(char) * 20);
                        //     // //printf("read fd is %d\n", );
                        // read(workers_array[i].read_fd, temp, 10);

                        // char *token = strtok(temp, "$");//printf("token = %s\n", token);
                        //                                 //printf("token is %s\n", token);
                        // int input_size = atoi(token); //tora ksero posa byte tha mou steilei
                                                           
                        // char buffer[10000]; //(char *)malloc(sizeof(char) * (input_size +1));
                        // memset(buffer, 0, input_size +1);

                        // char *p = buffer;

                        // if( input_size > params.bufferSize)
                        //         params.bufferSize = input_size + 1;

                        // read( workers_array[i].read_fd, p, input_size);//printf("bytes_in = %d\n", bytes_in);

                        // buffer[input_size]='\0';
                        // sum += atoi(buffer);
                        list_counter--;
                        pos++;
                    }
                    
                    sum = 0;                  
                }
                else // Sent Query to the Worker with the specific country
                {
                    // printf("query is %s\n",query );
                    // char temp[50];
                    // strcpy(temp, query);
                    // char * token = strtok(temp," ");
                    // token = strtok(NULL, " ");
                    // token = strtok(NULL, " ");
                    // token = strtok(NULL, " ");
                    // token = strtok(NULL, " ");

                    // char * country = malloc(sizeof(char) * strlen(token) + 1);
                    // strcpy(country, token);
                    // // printf("Requested country is %s\n", country);
                    // int pos = find_worker_country(workers_array, params.numWorkers, country);
                    // // printf("Position is %d\n",pos );
                    // if(pos != -1)
                    // {
                    //     // printf("query to be written is %s \n", query);
                    //     //printf("stelno se ena worker\n");
                    //     //write_to_fifo(workers_array[pos].write_fd, query);
                    // }
                    // else
                    // {
                    //     printf("Error: Requested Country Doesnt Exist\n");
                    // }
                }
            }

        } 
        // else if (strcmp(cmd, "/topk-AgeRanges") == 0 || strcmp(cmd, "/tka") == 0) 
        // {
        //     if (input != NULL && (strlen(input) < 30) || input == NULL)
        //     {
        //         printf("Please Insert valid Data \n");
        //         log_info->fail++;
        //         log_info->total++;
        //         putchar('>');
        //         continue;
        //     }
        //     else
        //     {
        //         strcpy(query, cmd);
        //         sprintf(query, "%s ", query);
        //         strcat(query, input);
        //         //printf("query is %s\n", query);

        //         // printf("query is %s\n",query );
        //         char temp[50];
        //         strcpy(temp, query);
        //         char * token = strtok(temp," ");
        //         token = strtok(NULL, " ");
        //         token = strtok(NULL, " ");

        //         //printf("Country is %s\n",token );

        //         char * country = malloc(sizeof(char) * strlen(token) + 1);
        //         strcpy(country, token);
        //         // printf("Requested country is %s\n", country);
        //         int pos = find_worker_country(workers_array, params.numWorkers, country);
        //         // printf("Position is %d\n",pos );
        //         if(pos != -1)
        //         {
        //             // printf("query to be written is %s \n", query);
        //             //printf("stelno se ena worker\n");
        //             write_to_fifo(workers_array[pos].write_fd, query);
        //             read_from_workers(workers_array, params);
        //             log_info->success++;
        //             log_info->total++;
        //         }
        //         else
        //         {
        //             printf("Error: Requested Country Doesnt Exist\n");
        //             log_info->fail++;
        //             log_info->total++;
        //         }                    
        //     }
        // } 
        // else if (strcmp(cmd, "/searchPatientRecord") == 0 || strcmp(cmd, "/spr") == 0 ) 
        // {

        //     if (input == NULL ||  digitValidate(input))
        //     {
        //         printf("Error\n");
        //         putchar('>');
        //         log_info->fail++;
        //         log_info->total++;
        //         continue;
        //     }
        //     else
        //     {
        //         for (int i = 0; i < params.numWorkers; ++i)
        //         {
        //             strcpy(query, cmd);
        //             sprintf(query, "%s ", query);
        //             strcat(query, input);
        //             write_to_fifo(workers_array[i].write_fd, query);
        //             read_from_workers(workers_array, params);
        //             log_info->success++;
        //             log_info->total++;
        //         }   
        //     }
        // } 
        // else if ((strcmp(cmd, "/numPatientAdmissions") == 0 || strcmp(cmd, "/npa") == 0)  ||  (strcmp(cmd, "/numPatientDischarges") == 0 || strcmp(cmd, "/npd") == 0))
        // {
        //     if ((input != NULL && strlen(input) < 20 || input == NULL))
        //     {
        //         printf("Please Insert valid Data\n");
        //         log_info->fail++;
        //         log_info->total++;
        //         putchar('>');
        //         continue;
        //     }
        //     else 
        //     {
        //         strcpy(query, cmd);
        //         sprintf(query, "%s ", query);
        //         strcat(query, input);
        //         //printf("query is %s\n", query);

        //         if (strlen(query) < 30) // Sent Query to All Workers
        //         {
        //             for (int i = 0; i < params.numWorkers; ++i)
        //             {
        //                 //printf("Parent : stelno se pollous worker to query:: %s\n", query);
        //                 write_to_fifo(workers_array[i].write_fd, query);
        //                 read_from_workers(workers_array, params);
        //                 log_info->success++;
        //                 log_info->total++;
        //             }                        
        //         }
        //         else // Sent Query to the Worker with the specific country
        //         {
        //             // printf("query is %s\n",query );
        //             char temp[50];
        //             strcpy(temp, query);
        //             char * token = strtok(temp," ");
        //             token = strtok(NULL, " ");
        //             token = strtok(NULL, " ");
        //             token = strtok(NULL, " ");
        //             token = strtok(NULL, " ");

        //             char * country = malloc(sizeof(char) * strlen(token) + 1);
        //             strcpy(country, token);
        //             // printf("Requested country is %s\n", country);
        //             int pos = find_worker_country(workers_array, params.numWorkers, country);
        //             // printf("Position is %d\n",pos );
        //             if(pos != -1)
        //             {
        //                 // printf("query to be written is %s \n", query);
        //                 //printf("stelno se ena worker\n");
        //                 write_to_fifo(workers_array[pos].write_fd, query);
        //                 read_from_workers(workers_array, params);
        //                 log_info->success++;
        //                 log_info->total++;
        //             }
        //             else
        //             {
        //                 log_info->fail++;
        //                 log_info->total++;
        //                 printf("Error: Requested Country Doesnt Exist\n");
        //             }
        //         }
        //     }
        // } 
        // else if (strcmp(cmd, "/man") == 0)
        // {
        //     open_manual();
        // }
        // else if (strcmp(cmd, "/exit") == 0) 
        // {
        //     printf("\nExiting Disease Aggregator..\n\n"); //Done
        //     for (int i = 0; i < params.numWorkers; ++i)
        //     {
        //         *kill_flag = 0;
        //         kill(workers_array[i].pid, SIGKILL);
        //     }
        //     free(line);
        //     return;
        // } 
        // else 
        // {
        //     fprintf(stdout, "~ error: %s: command not found!\n", cmd);
        //     log_info->fail++;
        //     log_info->total++;
        // }
        
    }
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
