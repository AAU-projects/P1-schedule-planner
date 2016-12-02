#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRAVEL 4
#define MAX_FLIGHT_NR 10
#define MAX_DATE 11
#define MAX_WEEKDAY 4
#define MAX_TIME 6
#define MAX_FLIGHT_MODEL 15

int count_travel_types(char *type);

typedef struct
{
    char travel_type[MAX_TRAVEL];
    char flight_number[MAX_FLIGHT_NR];
    char date[MAX_DATE];
    char weekday[MAX_WEEKDAY];
    char time[MAX_TIME];
    char flight_model[MAX_FLIGHT_MODEL];

}flight_data;

typedef struct
{
    flight_data *arrivals;
    flight_data *departure;
} flight_type;

int main(void)
{

    int number_arrivals, number_departures;

    number_arrivals = count_travel_types("ARR");
    number_departures = count_travel_types("DPT");

    flight_type flights[number_arrivals + number_departures];

    flights->arrivals = (flight_data*)calloc(number_arrivals, sizeof(flight_data));
    flights->departure = (flight_data*)calloc(number_departures, sizeof(flight_data));

    return 0;
}

int count_travel_types(char *type)
{
    FILE *fp = fopen("Flight_information", "r");
    int count = 0;
    int c;
    char travel_type[MAX_TRAVEL];

    while ((c = fgetc(fp)) != EOF)
    {
        fseek(fp,-1,SEEK_CUR);
        fscanf(fp," %s %*s %*s %*s %*s %*s",
                travel_type);

        if (!strcmp(travel_type,type))
        {
            count += 1;
            /*printf("%s \n",travel_type);*/
        }
    }

    fclose(fp);

    return count;
}