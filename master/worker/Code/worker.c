#include "../Headers/worker.h"
#include "../../code/input_list.c"

/*** Global Variables that allow signal handlers to retrive usefull Informations ***/ 

Logfile_Info log_info;

Directory_list d_list;

Patient_list patient_list;
HashTable disease_HT;
HashTable country_HT;


/**********************************************************************************/

int main(int argc, char *argv[])
{
    // signal(SIGINT, signal_handler);
    // signal(SIGQUIT, signal_handler);

    char  *countries = NULL , *connect_info = NULL, *summary_stats = NULL;

    Params params = init_params( argv[1], argv[2], &countries, &connect_info, atoi(argv[3]));

    if(readInputDirs(params, &disease_HT, &patient_list, &d_list, params.write_fd, &log_info, &summary_stats, &countries)==0)
    {
        printf("\nAn Error Occured While Proccesing Input\n\n");
        exit(0);            
    }

    int sig_flag = 0;

    int worker_sock, new_sock;
    struct sockaddr_in server, client;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    struct hostent *rem;

    int worker_port = create_socket(&serverptr,&clientptr,&server, &client,&worker_sock,&clientlen);

    send_summary_stats(summary_stats, &d_list, worker_port, params);

    char * result = NULL;
    while(1) //worker is ready to receive queries
    {
        //printf("beno sto aenao loop\n");
        if ((new_sock = accept(worker_sock, clientptr, &clientlen)) < 0) 
            perror("accept");
        // convert to client ip
        struct sockaddr_in *addr_in = (struct sockaddr_in *)clientptr;
        char *address = inet_ntoa(addr_in->sin_addr);

        //printf("Accepted connection from %s and new sock is %d\n", address, new_sock);

        char query[200];
        read(new_sock,query,200);

        //printf("Query received from worker with pid %d is %s\n",getpid() , query);

        result = query_handler(query, &disease_HT, &country_HT, &patient_list, &d_list, params.write_fd);
        //printf("result is %s\n",result);
        
        write(new_sock, result, 200);
        //free(result);
        close(new_sock); 
    }

    destroyHashTable(&disease_HT);
    destroyHashTable(&country_HT);
    freePatientList(&patient_list);
    freeDirList(&d_list);

    return 0;

}

int create_socket(struct sockaddr** serverptr, struct sockaddr** clientptr, struct sockaddr_in *server, struct sockaddr_in *client, int * worker_sock, socklen_t *clientlen)
{
    if ((*worker_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");

    server->sin_family = AF_INET;
    server->sin_addr.s_addr = htonl(INADDR_ANY);
    server->sin_port = htons(INADDR_ANY);

    // /* Bind socket to address */
    if (bind(*worker_sock, *serverptr, sizeof(*server)) < 0)
        perror_exit("bind");

    getsockname(*worker_sock, *serverptr, clientlen); //takes the first available port and give it to server
    int worker_port = ntohs(server->sin_port);

    printf("worker_port is %d\n",worker_port );
    
    if (listen(*worker_sock, 5) < 0) 
        perror_exit("listen");

    return worker_port;
}

int readInputDirs(Params params, HashTable *disease_HT, Patient_list * patient_list, Directory_list * d_list, int write_fd, Logfile_Info *log_info, char ** summary_stats, char ** countries)
{

    initHashTable(disease_HT, params.disHashSize, params.bucketsize); 
    initPatientList(patient_list);
    initDirectorytList(d_list);

    *summary_stats = calloc(sizeof(char) , MAX_STATS);

    char * token = NULL;
    int buffer_counter = 0;
    token = strtok(*countries , "$");
    int prev_token_len = strlen(token);
    
    char *p = *countries;
 
    while (token)
    {   
        strcpy(params.fileName, token);

        insertNewDirectory(d_list, params.fileName);
        //printf("params filename of process id %u is %s\n", getpid() ,params.fileName );
        if(strlen(params.fileName)> 10)
        {
            if(readPatientRecordsFile ( params, disease_HT, patient_list, params.write_fd, log_info, summary_stats)==0)
            {
                printf("\nAn Error Occured While Proccesing Input\n\n");
                exit(0);
            }
        }

        p = p + (strlen(token) +1);
        token = strtok(p, "\n$");
        if (token)
            prev_token_len += strlen(token);
    }

    //printDirectoryList(d_list);
    free(*countries);
    return 1;
}


int readPatientRecordsFile ( Params params, HashTable * disease_HT, Patient_list *patient_list, int write_fd, Logfile_Info *log_info, char ** stats)
{

    struct dirent *de;

    DIR *dr = opendir(params.fileName);

    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        perror("Error ");
        exit(0);
    } 

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int line_pos;
    char file_path[200];
    char date[50];
    // memset(*stats, '\0', sizeof(stats));

    strcpy(file_path, params.fileName);

    Patient patient_attributes; 

    Patient_Node * new_patient_node = NULL;

    log_info->total = 0;
    log_info->success = 0;
    log_info->fail = 0;

    while ((de = readdir(dr)) != NULL)
    {
        if(strcmp(de->d_name, ".") && strcmp(de->d_name, ".."))
        {
            //printf("%s\n",de->d_name );
            strcat(file_path, "/");
            strcat(file_path, de->d_name);
    
            //printf("file path is %s\n",file_path );
            FILE *fp = fopen(file_path, "r");

            if (fp == NULL) //error handling when opening the file
            {
                perror(" Requested file failed to open");
                return 0;
            }   

            for (int i = 0; i < disease_HT->size; ++i)
            {   
                if( disease_HT->lists_of_buckets[i].head != NULL)
                {
                    Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
                    while(temp !=NULL)
                    {
                        for (int j = 0; j < temp->slot_counter; ++j)
                        {
                            for (int k = 0; k < 4; ++k)
                            {
                                temp->bucket_item[j].age_ranges[k] = 0;
                            }
                        }
                        temp = temp->next;
                    }
                }
            }
            
            while ((nread = getline(&line, &len, fp)) != -1) 
            {   
                strcpy(date, de->d_name);
                //printf("Date is %s\n", date);
                char temp_line[100] = "";

                for (int i = 0; line[i] != '\0'; ++i) 
                {
                    temp_line[i] = line[i];
                }

                //printf("temp_lineis %s\n", temp_line);

                patient_attributes = line_tokenize(line, patient_attributes, date, file_path);
                
                if(strcmp(patient_attributes.status, "EXIT") == 0 && !id_exist(patient_list, patient_attributes.recordID))
                {
                    //printf("Provlimatiki eggrafi me stoixeia");
                    //printPatientData(patient_attributes);
                    free(patient_attributes.recordID);
                    free(patient_attributes.firstName);
                    free(patient_attributes.lastName);
                    free(patient_attributes.diseaseID);
                    free(patient_attributes.country);
                    //printf("Error\n");
                    log_info->fail++;
           
                }
                else if(strcmp(patient_attributes.status, "EXIT") == 0 && id_exist(patient_list, patient_attributes.recordID))
                {    
                    updatePatientRecord( patient_attributes.recordID,  patient_attributes.exitDate, patient_list);
                    log_info->success++;
                }
                else if(!id_exist(patient_list, patient_attributes.recordID) && (strcmp(patient_attributes.status, "ENTER") == 0))
                {      
       
                    new_patient_node =  insertNewPatient(patient_list, patient_attributes);
                    insert_to_hash_table(disease_HT, patient_attributes.diseaseID, new_patient_node);
                    log_info->success++;
                }

                log_info->total++;
            }
            //printf("Pao na grapso\n");
            char *temp_stats = NULL;
            temp_stats = calculate_summary_stats(disease_HT, patient_list->tail->patient.country, patient_list->tail->patient.entryDate, write_fd, params);
            fclose(fp);
            strcpy(file_path, params.fileName);
           //printf("file_path is %s\n", file_path);
            sprintf(*stats,"%s%s\n", *stats, temp_stats);
            free(temp_stats);
        }
    }
    //print_hash_table(disease_HT);
    //printf("%s\n", *stats);
    free(line);
    closedir(dr);   
    return 1;
}

void send_summary_stats(char *summary_stats, Directory_list *d_list, int port, Params params)
{
    char *token = NULL;
    int sock;
    int length;
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;

    /* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("socket");
    /* Find server address */
    if ((rem = gethostbyname(params.ipAddress)) == NULL) 
    { 
       perror("gethostbyname"); 
       exit(1);
    }

    server.sin_family = AF_INET; /* Internet domain */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(params.server_portNum); /* Server port */
    if (connect(sock, serverptr, sizeof(server)) < 0)
        perror("connect");

    //printf("Connecting to %s port %d\n", params.ipAddress, params.portNum);
    CountryPath_Node* temp_node = NULL;

    int counter = d_list->counter;
    int index = 0;
    //printf("counter is %d\n", counter);
    char temp_path[50];
    while(counter > 0)
    {
        temp_node = get_country(d_list, index);
        strcpy(temp_path, temp_node->country_path);
        // printf("temp->country path is %s\n", temp->country_path);
        token = strtok(temp_path, "/");
        token = strtok(NULL, "/");
        token = strtok(NULL, "/");
        token = strtok(NULL, "/");

        // printf("token is %s\n", token );
        sprintf(summary_stats, "%s$%s", summary_stats, token);
        index++;
        counter--;
    }
    //printf("port is %d\n", port);
    sprintf(summary_stats, "%s@%d", summary_stats, port);
    write_to_socket(sock, summary_stats);
}


char *calculate_summary_stats( HashTable * disease_HT,  char * country, Date date, int write_fd, Params params)
{
    char *stats = malloc(sizeof(char) * 10000);
    // print_date(date);
    // printf("\n");
    // printf("%s\n",country);
    sprintf(stats, "%d-%d-%d", date.day, date.month, date.year);
    sprintf(stats, "%s\n%s", stats, country);

    //printf("%s\n", stats);

    for (int i = 0; i < disease_HT->size; ++i)
    {       
        if( disease_HT->lists_of_buckets[i].head != NULL)
        {
            //printf("eimai diaforo tou NULL\n");
            Bucket_Node *temp = disease_HT->lists_of_buckets[i].head;
            while(temp !=NULL)
            {                    
                for (int j = 0; j < temp->slot_counter; ++j)
                {   
                    // printf("%s\n", temp->bucket_item[j].string);
                    sprintf(stats, "%s\n%s", stats, temp->bucket_item[j].string);
                    sprintf(stats, "%s\nAge range 0-20 years: %d", stats, temp->bucket_item[j].age_ranges[0]);
                    sprintf(stats, "%s\nAge range 20-40 years: %d", stats, temp->bucket_item[j].age_ranges[1]);
                    sprintf(stats, "%s\nAge range 41-60 years: %d", stats, temp->bucket_item[j].age_ranges[2]);
                    sprintf(stats, "%s\nAge range 60+ years: %d", stats, temp->bucket_item[j].age_ranges[3]);
                }
                temp = temp->next;
            }
        }
    }

    return stats;
}


Params init_params( char * read_fifo, char *write_fifo, char ** countries, char **info, int buffersize)
{
    Params params;
    params.read_fd = open(read_fifo, O_RDONLY);
    //printf("read_fd is %d\n",params.read_fd );
    params.write_fd = open(write_fifo, O_WRONLY);

    //printf("write_fd is %d\n", params.write_fd);
    *countries = read_from_fifo(params.read_fd, buffersize);

    //printf("Message is %s\n", *countries);

    *info = read_from_fifo(params.read_fd, buffersize);

    // printf("Connection info are %s\n", *info);
    
    params.disHashSize = 15;
    params.bucketsize = 512;

    char *token  = strtok(*info, "$");
    params.ipAddress = malloc(sizeof(char) * strlen(token) + 1);
    strcpy(params.ipAddress, token);
    token = strtok(NULL, "\n");
    params.server_portNum = atoi(token);

    //printf("Write_fd is %d, read_fd is %d, ip_address is %s, portNum is %d\n", params.write_fd, params.read_fd, params.ipAddress, params.portNum);
    return params;
}

char * query_handler(char * message, HashTable * disease_HT, HashTable * country_HT, Patient_list *list, Directory_list * dir_list, int write_fd)
{

    char *input = NULL, *cmd = NULL;

    input = message;
    cmd = strtok_r(input, " \n", &input);
    input = strtok(input, "\n");

    if (cmd != NULL) 
    {

        if (strcmp(cmd, "/diseaseFrequency") == 0 || strcmp(cmd, "/df") == 0) 
        {
            return diseaseFrequency(input, disease_HT, list); //done
        } 
        else if (strcmp(cmd, "/topk-AgeRanges") == 0 || strcmp(cmd, "/tka") == 0 ) 
        {
            return topAgeRanges(input, disease_HT, list);
        } 
        else if (strcmp(cmd, "/searchPatientRecord") == 0 || strcmp(cmd, "/spr") == 0)
        {
            Patient patient = searchPatientRecord(input, list);
            if (patient.recordID == 0)
            {
                char * result = malloc(sizeof(char)* 2);
                strcpy(result, "0");
                return result;
            }
            else
            {
                char * result = patient_stringify(patient); 
                //printf("result is %s\n", result);
                return  result;
            }   
            //printf("result before return is %s\n", result);           
        } 
        else if (strcmp(cmd, "/numPatientAdmissions") == 0 || strcmp(cmd, "/npa") == 0) 
        {
            int flag = 1;
            char *token = NULL;
            //printf("strlen of input is %ld\n",strlen(input));
            if (check_for_country(input))
            {
                int index = 0;
                char * response = malloc(sizeof(char)* 200);
                strcpy(response, "");
            
                CountryPath_Node* temp_node = NULL;
                //printf("Input is %s\n", input);
                int counter = dir_list->counter;
                //printf("counter is %d\n", counter);
                //printf("o xristis den exei dosei xora!!!\n");
                char temp_country[50];
                while(counter > 0)
                {
                    temp_node = get_country(dir_list, index);
                    strcpy(temp_country, temp_node->country_path);
                    //printf("temp->country path is %s\n", temp->country_path);
                    token = strtok(temp_country, "/");
                    token = strtok(NULL, "/");
                    token = strtok(NULL, "/");
                    token = strtok(NULL, "/");

                    //printf("token is %s\n", token );
                    char * result = numPatientAdmissions(input, disease_HT, list, token, flag);
                    sprintf(response, "%s\n%s", response, result);
                    free(result);
                    index++;
                    counter--;
                }
                return response;
            }
            else
            {
                return numPatientAdmissions(input, disease_HT, list, token, flag);
            }
        } 
        else if (strcmp(cmd, "/numPatientDischarges") == 0 || strcmp(cmd, "/npd")==0 ) 
        {
            int flag = 0;
            char *token = NULL;
            if (check_for_country(input))
            {
                int index = 0;
                char * response = malloc(sizeof(char)* 200);
                strcpy(response, "");
            
                CountryPath_Node* temp_node = NULL;
                //printf("i am in patient dishargers\n");
                int counter = dir_list->counter;
                //printf("counter is %d\n", counter);
                char temp_country[50];
                while(counter > 0)
                {
                    temp_node = get_country(dir_list, index);
                    strcpy(temp_country,temp_node->country_path);
                    // printf("temp->country path is %s\n", temp->country_path);
                    token = strtok(temp_node->country_path, "/");
                    token = strtok(NULL, "/");
                    token = strtok(NULL, "/");
                    token = strtok(NULL, "/");

                    // printf("token is %s\n", token );
                    char * result = numPatientAdmissions(input, disease_HT, list, token, flag);
                    sprintf(response, "%s\n%s", response, result);
                    free(result);
                    index++;
                    counter--;
                }
                return response;
            }
            else // query had a country name
            {
                return numPatientAdmissions(input, disease_HT, list, token, flag);
            }
        } 
    }
}


char * read_from_fifo( int read_fd, int buffersize)
{
    int bytes_in; //posa byte diavastikan apo tin read
    int buffer_counter = 0 , input_size = 0;
    char * token = NULL , *p = NULL, *buffer = NULL;

    char temp[11];
    memset(temp, '\0', sizeof(temp));

    if(read(read_fd, temp, 10)==0)
        perror("Error");

    token = strtok(temp, "$");
    input_size = atoi(token); //tora ksero posa byte tha mou steilei
    buffer = calloc(sizeof(char) , (input_size) + 1);
    memset(buffer, '\0', sizeof(buffer));
    p = buffer;

    while(buffer_counter < input_size)
    {
        bytes_in = read( read_fd, p, buffersize );//printf("bytes_in = %d\n", bytes_in);

        if(buffer_counter + bytes_in > input_size)
            bytes_in = input_size-buffer_counter;
        
        p += bytes_in;
        buffer_counter += bytes_in;
    }

    buffer[input_size]='\0';
    return buffer;
}

void write_to_socket(int  write_fd, char * message)
{
    int message_len = strlen(message);
    char temp[11];
    memset(temp, '\0', sizeof(temp));

    sprintf(temp, "w$"); //w stands for worker
    //printf("message_len is %d\n", message_len);
    write(write_fd, temp, 10);
    //printf("message is %s\n",message );
    write(write_fd, message, message_len);
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


Patient line_tokenize(char *line, Patient patient, char * date, char * country )
{
        char * token;
        char temp [100] = " ";
        char temp_line[100]= " ";
        strcpy(temp, country);
        strcpy(temp_line, line );

        //printf("line is %s\n", temp_line );
        token = strtok(temp_line, " ");
        patient.recordID = malloc(sizeof(char)* strlen(token)+1);
        strcpy(patient.recordID, token);
        //printf("patient record id is %s\n", patient.recordID);

        token = strtok(NULL, " ");
        strcpy( patient.status, token);
        //printf("patient record status is %s\n", patient.status);

        token = strtok(NULL, " ");
        patient.firstName = malloc(sizeof(char)*strlen(token)+1);
        strcpy( patient.firstName, token);
        //printf("patient record firstName is %s\n", patient.firstName);

        token = strtok(NULL, " ");
        patient.lastName = malloc(sizeof(char)*strlen(token)+1);
        strcpy( patient.lastName, token);
        //printf("patient record lastName is %s\n", patient.lastName);

        token = strtok(NULL, " ");
        patient.diseaseID = malloc(sizeof(char)*strlen(token)+1);
        strcpy( patient.diseaseID, token);
        //printf("patient record disease is %s\n", patient.diseaseID);

        token = strtok(NULL, " ");
        patient.age = atoi(token);
        //printf("patient record age is %d\n", patient.age);

        if(strcmp(patient.status, "ENTER") == 0)
        {
            token = strtok(date, "-");
            patient.entryDate.day = atoi (token);

            token = strtok(NULL, "-");
            patient.entryDate.month = atoi (token);

            token = strtok(NULL, "\n");
            patient.entryDate.year = atoi (token);
            
            patient.exitDate.day = 0;
            patient.exitDate.month = 0;
            patient.exitDate.year = 0;

        }
        else
        {
            token = strtok(date, "-");
            patient.exitDate.day = atoi (token);

            token = strtok(NULL, "-");
            patient.exitDate.month = atoi (token);

            token = strtok(NULL, "\n");
            patient.exitDate.year = atoi (token);

            patient.entryDate.day = 0;
            patient.entryDate.month = 0;
            patient.entryDate.year = 0;

        } 
        //printf("country is %s\n",country );
        token = strtok(temp, "/");
        token = strtok(NULL, "/");
        token = strtok(NULL, "/");
        token = strtok(NULL, "/");

        patient.country = malloc(sizeof(char)*strlen(token)+1);
        strcpy( patient.country, token); 

    return patient;
}


void signal_handler(int sig)
{
    sleep(1); 
    char buffer[30] = "";

    sprintf(buffer, "log_file_worker.");
    sprintf(buffer + strlen(buffer), "%u", getpid());

    FILE *fp = fopen(buffer, "w");

    CountryPath_Node* temp = NULL;
    int counter = d_list.counter;
    char *token = NULL;
    int index = 0;
        
    while(counter > 0)
    {
        temp = get_country(&d_list, index);
        //printf("temp->country path is %s\n", temp->country_path);
        token = strtok(temp->country_path, "/");
        token = strtok(NULL, "/");
        token = strtok(NULL, "/");
        token = strtok(NULL, "/");

        //printf("token is %s\n", token );
        fprintf(fp, "%s\n", token);
        index++;
        counter--;
    }

    fprintf(fp, "Total: %d\n", log_info.total);
    fprintf(fp, "Success: %d\n", log_info.success);
    fprintf(fp, "Fail: %d\n" , log_info.fail);

    fclose(fp);

    destroyHashTable(&disease_HT);
    destroyHashTable(&country_HT);
    freePatientList(&patient_list);
    freeDirList(&d_list);

}


void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}











