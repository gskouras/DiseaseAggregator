#include "../headers/client.h"

int main(int argc, char *argv[])
{

	Params params = inputValidate(argc, argv);
	return 0;
}


Params inputValidate (int argc, char *argv[])
{
    Params params;
    
    if(argc==1)
    {
        params.queryFile = malloc(sizeof(char) *25);
        strcpy(params.queryFile, "./resources/queryFile");
        params.numThreads = 2;
        params.servPort = 1;
        params.servIP = malloc(sizeof(char) * 20);
        strcpy(params.servIP, "....kati");
        return params;
    }

    else
    { 
    	if ( argc != 9 )
    	{
    		printf("Error. Arguement related error: Got %d, expectetd 7\n", argc);
    		exit(0);
    	}

    	if ( (strcmp(argv[1], "-q") == 0 && strcmp(argv[3], "-w") == 0 && strcmp(argv[5], "-sp") == 0 && strcmp(argv[7], "-sip"))
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
