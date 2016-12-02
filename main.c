#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRAVEL 4
#define MAX_FLIGHT_NR 10
#define MAX_DATE 11
#define MAX_WEEKDAY 4
#define MAX_TIME 6
#define MAX_FLIGHT_MODEL 15

typedef struct
{
    char travel_type[MAX_TRAVEL];
    char flight_number[MAX_FLIGHT_NR];
    char date[MAX_DATE];
    char weekday[MAX_WEEKDAY];
    char time[MAX_TIME];
    char flight_model[MAX_FLIGHT_MODEL];

}flight_data;

int main(void)
{

    count_travel_types("ARR");

    flight_data arrivals[1];
    flight_data departures[1];
}

int count_travel_types(char *type)
{
    FILE *fp = fopen("Flight_information", "r");

    if(!strcmp(type,"DPT"))
    {

    }

    fclose(fp);
}