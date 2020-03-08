#include "main.h"

typedef struct
{
	int day;
	int monnth;
	int year;
} Date;

typedef struct
{
	int recordID;
	char * firstName;
	char * lastName;
	char * diseaseID;
	char * country;
	Date entryDate;
	Date exitDate;
} Patient;




int  read_PatientRecordsFile ( char * );