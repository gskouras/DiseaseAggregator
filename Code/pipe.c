#include "../headers/pipe.h"
#include "../headers/main.h"


void cli( Worker_info * workers_array, Params params )
{

    char *input = NULL, *line = NULL, *cmd = NULL;
    size_t len = 0;
    ssize_t read;

    // printf("\t\t\t##########  WELCOME TO DISEASE MONITOR COMMAND LINE INTERFACE  ##########\n\n"); 
    // printf("You can use the available commands listed in the manual page. Type /man to see the manual page or type /exit to quit\n\n");
    printf("\n");
    putchar('>');

    while ((read = getline(&line, &len, stdin)) != EOF) 
    {
        input = line;
        cmd = strtok_r(input, " \n", &input);
        input = strtok(input, "\n");
        char query[50]="";

        if (cmd != NULL) 
        {
            if (strcmp(cmd, "/listCountries") == 0 || strcmp(cmd, "/lc") == 0) 
            {
                int list_counter;
                int index = 0;
                char *token = NULL;
                char country[50];
                for (int i = 0; i < params.numWorkers; ++i)
                {
                    list_counter = workers_array[i].country_list.counter;
                    //list_reverse(&workers_array[i].country_list);
                    //printDirectoryList(&workers_array[i].country_list);
                    while(list_counter > 0)
                    {
                        CountryPath_Node *temp = get_country(&workers_array[i].country_list, index); 
                        strcpy(country, temp->country_path);
                        //printf("country is %s\n", country);
                        token = strtok(country, "/");
                        token = strtok(NULL, "/");
                        token = strtok(NULL, "/");
                        token = strtok(NULL, "/");
                        strcpy(country, token);
                        printf("%s %u\n",country, workers_array[i].pid );
                        index++;
                        list_counter--;
                    }
                    index = 0;
                }
            } 
            else if (strcmp(cmd, "/diseaseFrequency") == 0 || strcmp(cmd, "/df") == 0) 
            {
                if (input != NULL && strlen(input) < 20  || input == NULL)
                {
                    printf("Please Insert valid Data\n");
                    putchar('>');
                    continue;
                }
                else 
                {
                    strcpy(query, cmd);
                    sprintf(query, "%s ", query);
                    strcat(query, input);
                    //printf("query is %s\n", query);

                    if (strlen(query) <= 35) // Sent Query to All Workers
                    {
                        for (int i = 0; i < params.numWorkers; ++i)
                        {
                            //printf("stelno se pollous worker\n");
                            write_to_fifo(workers_array[i].write_fd, query);
                            read_from_workers(workers_array, params);
                        }                        
                    }
                    else // Sent Query to the Worker with the specific country
                    {
                        // printf("query is %s\n",query );
                        char temp[50];
                        strcpy(temp, query);
                        char * token = strtok(temp," ");
                        token = strtok(NULL, " ");
                        token = strtok(NULL, " ");
                        token = strtok(NULL, " ");
                        token = strtok(NULL, " ");

                        char * country = malloc(sizeof(char) * strlen(token) + 1);
                        strcpy(country, token);
                        // printf("Requested country is %s\n", country);
                        int pos = find_worker_country(workers_array, params.numWorkers, country);
                        // printf("Position is %d\n",pos );
                        if(pos != -1)
                        {
                            // printf("query to be written is %s \n", query);
                            printf("stelno se ena worker\n");
                            write_to_fifo(workers_array[pos].write_fd, query);
                            read_from_workers(workers_array, params);
                        }
                        else
                        {
                            printf("Error: Requested Country Doesnt Exist\n");
                        }
                    }
                }

            } 
            else if (strcmp(cmd, "/topk-AgeRanges") == 0 || strcmp(cmd, "/tka") == 0) 
            {
                if (input != NULL && (strlen(input) < 24  && strlen(input) > 12))
                {
                    printf("If you Enter a Date you must also enter another \n");
                    putchar('>');
                    continue;
                }
                else
                {
                    for (int i = 0; i < params.numWorkers; ++i)
                    {
                        strcpy(query, cmd);
                        sprintf(query, "%s ", query);
                        strcat(query, input);
                        write_to_fifo(workers_array[i].write_fd, query);
                        read_from_workers(workers_array, params);
                    }             
                }

            } 
            else if (strcmp(cmd, "/searchPatientRecord") == 0 || strcmp(cmd, "/spr") == 0 ) 
            {

                if (input == NULL ||  digitValidate(input))
                {
                    printf("Error :  \n");
                    putchar('>');
                    continue;
                }
                else
                {
                    for (int i = 0; i < params.numWorkers; ++i)
                    {
                        strcpy(query, cmd);
                        sprintf(query, "%s ", query);
                        strcat(query, input);
                        write_to_fifo(workers_array[i].write_fd, query);
                        read_from_workers(workers_array, params);
                    }   
                }


            } 
            else if ((strcmp(cmd, "/numPatientAdmissions") == 0 || strcmp(cmd, "/npa") == 0)  ||  (strcmp(cmd, "/numPatientDischarges") == 0 || strcmp(cmd, "/npd") == 0))
            {
                if ((input != NULL && strlen(input) < 20 || input == NULL))
                {
                    printf("Please Insert valid Data\n");
                    putchar('>');
                    continue;
                }
                else 
                {
                    strcpy(query, cmd);
                    sprintf(query, "%s ", query);
                    strcat(query, input);
                    //printf("query is %s\n", query);

                    if (strlen(query) <= 35) // Sent Query to All Workers
                    {
                        for (int i = 0; i < params.numWorkers; ++i)
                        {
                            //printf("Parent : stelno se pollous worker to query:: %s\n", query);
                            write_to_fifo(workers_array[i].write_fd, query);
                            read_from_workers(workers_array, params);
                        }                        
                    }
                    else // Sent Query to the Worker with the specific country
                    {
                        // printf("query is %s\n",query );
                        char temp[50];
                        strcpy(temp, query);
                        char * token = strtok(temp," ");
                        token = strtok(NULL, " ");
                        token = strtok(NULL, " ");
                        token = strtok(NULL, " ");
                        token = strtok(NULL, " ");

                        char * country = malloc(sizeof(char) * strlen(token) + 1);
                        strcpy(country, token);
                        // printf("Requested country is %s\n", country);
                        int pos = find_worker_country(workers_array, params.numWorkers, country);
                        // printf("Position is %d\n",pos );
                        if(pos != -1)
                        {
                            // printf("query to be written is %s \n", query);
                            printf("stelno se ena worker\n");
                            write_to_fifo(workers_array[pos].write_fd, query);
                            read_from_workers(workers_array, params);
                        }
                        else
                        {
                            printf("Error: Requested Country Doesnt Exist\n");
                        }
                    }
                }
            } 
            else if (strcmp(cmd, "/man") == 0)
            {
                open_manual();
            }
            else if (strcmp(cmd, "/exit") == 0) 
            {
                printf("\nExiting Disease Aggregator..\n\n"); //Done
                for (int i = 0; i < params.numWorkers; ++i)
                {
                    kill(workers_array[i].pid, SIGKILL);
                }
                free(line);
                return;
            } 
            else 
            {
                fprintf(stdout, "~ error: %s: command not found!\n", cmd);
            }
            
        }
        
        putchar('>');
        putchar(' ');
    }
}

/*** CLI commands ***/

void open_manual()
{
    FILE *fptr; 
  
    char filename[100], c; 
  
    // Open file 
    fptr = fopen("./Resources/manual.txt", "r"); 
    if (fptr == NULL) 
    { 
        printf("Cannot open file manual \n"); 
        return;
    } 
  
    // Read contents from file 
    c = fgetc(fptr); 
    while (c != EOF) 
    { 
      /* no timeout */
        printf ("%c", c); 
        c = fgetc(fptr); 
    } 

    printf("\n\n");
    printf("\t\t\t\t\t\tPress ENTER to Continue\n");  
    getchar(); 
    return;  
}

int initialize_dirPaths(Directory_list* d_list, Worker_info* workers_array, char ** parentPipes, char ** workerPipes, int numWorkers)
{
    sleep(2);

    for (int i = 0; i < numWorkers; ++i)
    {   
        workers_array[i].write_fd = open(parentPipes[i], O_WRONLY | O_NONBLOCK);
        workers_array[i].read_fd =  open(workerPipes[i], O_RDONLY | O_NONBLOCK);  
        //printf("i opened %s for readonly\n",workerPipes[i]);            
    }
    
    //printf("write_fd is %d\n", workers_array[0].write_fd);

    int counter = 0;
    int country_index = 0;
    int list_counter = d_list->counter;
    if(numWorkers >= d_list->counter)
    {
	    while (list_counter > 0)
	    {
    		CountryPath_Node *temp = get_country(d_list, country_index);
            //printf("counter is %d temp country path is %s\n",country_index , temp->country_path);
    		insertNewDirectory(&workers_array[counter].country_list, temp->country_path);
    		counter++;
            country_index++;
            list_counter--;
	 	}
    }
    else
    {
    	while (list_counter  > 0)
	    {
			CountryPath_Node *temp =  get_country(d_list, country_index);
            //printf("counter is %d temp country path is %s\n",country_index,  temp->country_path);
	    	insertNewDirectory(&workers_array[counter].country_list, temp->country_path);
    		counter++;
            country_index++;
            list_counter--;

    		if( counter == numWorkers)
    		{
    			counter = 0;
    		}
    	}
    }

    //apo do kai pano o kodikas einai tsekarismenos trexei sosta.

    for (int i = 0; i < numWorkers; ++i)
    {
        if(workers_array[i].country_list.counter > 1) // an o sigekrimenos worker exei parapano apo mia xores
    	{
            char message_buffer[1000]="";
            char temp_buffer[1000]="";

            memset( (void *)message_buffer, '\0', sizeof(message_buffer));
            memset( (void *)temp_buffer, '\0', sizeof(temp_buffer));

            list_counter = workers_array[i].country_list.counter;
            int index = 0;
    		while (list_counter > 0) //oso i lista den einai keni
    		{
    			CountryPath_Node *temp =  get_country(&workers_array[i].country_list, index); //pairno apo to lista to proto path
                //printf("tenp country is %s\n", temp->country_path);
                if(strlen(temp_buffer) > 1 )
                    sprintf(temp_buffer, "%s$", temp_buffer);//vazo dolario gia na ta ksexwriso meta me strtok

				strcat(temp_buffer, temp->country_path); //vazo tin epomeni xwra pou exei i lista
                index++;
                list_counter--;
    		}

            strcat(message_buffer, temp_buffer); //ston synoliko message buffer vazo ton temp
            //printf("message_buffer is %s\n", message_buffer );
            //printf("i am writing to %s\n",workers_array[i].write_fifo );
            write_to_fifo(workers_array[i].write_fd, message_buffer); //grafo se olous tous worker ta path me tis xores
    		
            //printf("writed from ifto %s countries %s\n",workers_array[0].write_fifo, message_buffer );
    	}
    	else //an exeis mono mia xora apla partin apo ti lista kai grapstin sto fifo
    	{
            char message_buffer[1000] = "";
            char temp_buffer[1000] = "";
    		CountryPath_Node *temp =  get_country(&workers_array[i].country_list, 0);
			strcat(message_buffer, temp->country_path);
			write_to_fifo(workers_array[i].write_fd, message_buffer);
    	}	
    }
    sleep(1);
}


int read_from_fifo( int read_fd, int buffersize)
{
    int bytes_in = 0, input_size; //posa byte diavastikan apo tin read
    int buffer_counter = 0;
    char *token;
    char *p;
    char temp[100];


    //printf("read fd is %d\n",read_fd );
    read(read_fd, temp, 10);
    //printf("temp is %s\n",temp );
    token = strtok(temp, "$");//printf("token = %s\n", token);
    //printf("token is %s\n", token);
    input_size = atoi(token); //tora ksero posa byte tha mou steilei
    
    char buffer[10000]; //(char *)malloc(sizeof(char) * (input_size +1));
    memset(buffer, 0, input_size +1);

    p = buffer;

    if(input_size > buffersize)
        buffersize = input_size + 1;

    // printf("input size is %d\n",input_size );
    bytes_in = read( read_fd, p, input_size );//printf("bytes_in = %d\n", bytes_in);
    // printf("I read %d bytes\n", bytes_in);
    if (bytes_in == 0)
        return 0;

    buffer[input_size]='\0';
    if (strcmp(buffer, "0") !=0)
        printf("%s\n",buffer);
    //printf("strlen of buffer = %ld\n", strlen(buffer));
    if (strlen(buffer) == 0)
        return 0;

    // printf("strlen of buffer is  %ld\n", strlen(buffer));
    return 1;
}

void write_to_fifo(int  write_fd, char * message)
{
    int message_len = strlen(message);
    char temp[11];

    sprintf(temp, "%d$", message_len);
    //printf("temp is %s\n", temp);
    write(write_fd, temp, 10);
    //printf("message is %s \n", message);
    write(write_fd, message, message_len);

}


///////******* Utillity Fucntions ******///////



int find_worker_country(Worker_info * workers_array, int workers, char * country)
{
   //printf("country is(in find worker country) ::: %s\n", country);
    char temp_path[50];
    for (int i = 0; i < workers; ++i)
    {
        CountryPath_Node * temp = workers_array[i].country_list.head;
        char * token = NULL;
        while( temp != NULL)
        {
            strcpy(temp_path, temp->country_path);
            token = strtok(temp_path, "/");
            token = strtok(NULL, "/");
            token = strtok(NULL, "/");
            token = strtok(NULL, "/");
            //printf("token inside while is ::: %s\n",token );

            if(strcmp(country, token) == 0)
                return i;
            temp = temp->next;
        }
    }
    return -1;
}


void df_tokenize (char *input, char ** disease_holder, char ** country_holder, Date *d1, Date *d2)  //printf(" disease holder is%s\n", *disease_holder );
{   
    if (strlen(input) <= 30) //tokenize without country name
    {
        char *token = strtok( input, " ");
    
        *disease_holder = malloc(sizeof(char)*strlen(token)+1);
        strcpy(*disease_holder, token);
        token = strtok( NULL, "\n");    
        char *date_holder = malloc(sizeof(char)*strlen(token)+1);
        strcpy(date_holder, token);
        dateTokenize(date_holder , d1, d2); 
        *country_holder = NULL;
    }
    else
    {
        char *token = strtok( input, " ");
    
        *disease_holder = malloc(sizeof(char)*strlen(token)+1);
        strcpy(*disease_holder, token);
        token = strtok(NULL, "-");
        d1->day = atoi (token);

        token = strtok(NULL, "-");
        d1->month = atoi (token);

        token = strtok(NULL, " ");
        d1->year = atoi (token);

        token = strtok(NULL, "-");
        d2->day = atoi (token);

        token = strtok(NULL, "-");
        d2->month = atoi (token);

        token = strtok(NULL, " ");
        d2->year = atoi (token);

        token = strtok(NULL, "\n");
        *country_holder = malloc(sizeof(char)*strlen(token)+1);
        strcpy(*country_holder, token);
    }
}


void dateTokenize( char * input, Date *d1, Date *d2)
{   
    char * token = NULL;

    token = strtok(input, "-");
    d1->day = atoi (token);

    token = strtok(NULL, "-");
    d1->month = atoi (token);


    token = strtok(NULL, " ");
    d1->year = atoi (token);


    token = strtok(NULL, "-");
    d2->day = atoi (token);


    token = strtok(NULL, "-");
    d2->month = atoi (token);


    token = strtok(NULL, "\n");
    d2->year = atoi (token);  
}