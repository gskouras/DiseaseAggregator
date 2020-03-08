#include "main.h"



//only main stays at this file
int main(int argc, char *argv[]) {

	//./diseaseMonitor -p patientRecordsFile –h1 diseaseHashtableNumOfEntries  –h2 countryHashtableNumOfEntries –b bucketSize 

	Params params = inputValidate(argc, argv);



	printf("parameters of program are %s, %d, %d, %d\n", params.fileName, params.disHashNum, params.countryHashNum, params.bucketsize);

}



int digitValidate(char *a){
	for (unsigned int i = 0; i < strlen(a); i++){
		if(!isdigit(a[i])){
			return -1;
		}
	}
	return 0;
}

Params inputValidate (int argc, char *argv[])
{
	if ( argc != 9 )
	{
		printf("Error. Arguement related error: Got %d, expectetd 9\n", argc);
		exit(0);
	}

	Params params;

	if ( (strcmp(argv[1], "-p") == 0 && strcmp(argv[3], "-h1") == 0 && strcmp(argv[5], "-h2") == 0 && strcmp(argv[7], "-b") == 0)
	&& !digitValidate(argv[4]) && !digitValidate(argv[6]) && !digitValidate(argv[8]))
	{
		params.fileName = argv[2];
		params.disHashNum = atoi(argv[4]);
		params.countryHashNum = atoi(argv[6]);
		params.bucketsize = atoi(argv[8]);
	} 
	else 
	{
		printf("Error. Arguement related error.\n");
		exit(0);
	}
	return params;
}