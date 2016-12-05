#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRAVEL 4
#define MAX_FLIGHT_NR 10
#define MAX_DATE 11
#define MAX_WEEKDAY 4
#define MAX_TIME 6
#define MAX_FLIGHT_MODEL 15

typedef struct {
    char travel_type[MAX_TRAVEL];
    char flight_number[MAX_FLIGHT_NR];
    char date[MAX_DATE];
    char weekday[MAX_WEEKDAY];
    char time[MAX_TIME];
    char flight_model[MAX_FLIGHT_MODEL];

} flight_data;

int count_lines();

void scan_for_data(int total_flights, flight_data *flights);

flight_data read_line(FILE *fp);

void print_array(int length, flight_data *flights);

int main(void)
{
    int total_flights = count_lines();

    flight_data flights[total_flights];
    scan_for_data(total_flights, flights);

    printf("%d\n", total_flights);
    print_array(total_flights, flights);
}

int count_lines()
{
    FILE *fp = fopen("Flight_information.txt", "r");
    int c, counter = 1;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
            ++counter;
    }

    fclose(fp);

    return counter;
}

void scan_for_data(int total_flights, flight_data *flights)
{
    FILE *fp = fopen("Flight_information.txt", "r");

    for (int i = 0; i < total_flights; ++i)
    {
        flights[i] = read_line(fp);
    }
    fclose(fp);
}

flight_data read_line(FILE *fp)
{
    flight_data flight;
    fscanf(fp, " %s %s %s %s %s %s",
           flight.travel_type,
           flight.flight_number,
           flight.date,
           flight.weekday,
           flight.time,
           flight.flight_model);

    return flight;
}

void print_array(int length, flight_data *flights)
{
    for (int i = 0; i < length; ++i)
    {
        printf("%s %s %s %s %s %s\n",
               flights[i].travel_type,
               flights[i].flight_number,
               flights[i].date,
               flights[i].weekday,
               flights[i].time,
               flights[i].flight_model);
    }
}