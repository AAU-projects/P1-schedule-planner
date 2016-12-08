#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRAVEL 4
#define MAX_FLIGHT_NR 10
#define MAX_DATE 11
#define MAX_WEEKDAY 4
#define MAX_TIME 6
#define MAX_FLIGHT_MODEL 15
#define MAX_NAME 15
#define MAX_WORKTIME 14
#define PASSENGERS_PER_EMPLOYEE 38
#define HOUR_MILITARYTIME 100

typedef struct {
    char travel_type[MAX_TRAVEL];
    char flight_number[MAX_FLIGHT_NR];
    char date[MAX_DATE];
    char weekday[MAX_WEEKDAY];
    char time[MAX_TIME];
    char flight_model[MAX_FLIGHT_MODEL];
    int passengers;
} flight_type;

typedef struct {
    char first_name[MAX_NAME];
    char last_name[MAX_NAME];
    int max_hrs;
    int hrs;
    struct worktime {
        char monday[MAX_WORKTIME];
        char tuesday[MAX_WORKTIME];
        char wednesday[MAX_WORKTIME];
        char thursday[MAX_WORKTIME];
        char friday[MAX_WORKTIME];
        char saturday[MAX_WORKTIME];
        char sunday[MAX_WORKTIME];
    }worktime;
} employee_type;

/* prototypes */
int count_lines(char *file);
void scan_for_flight_data(char *file, int total_flights, flight_type *flights);
void scan_for_employee_data(char *file, int total_employees, employee_type *employees);
flight_type read_flights(FILE *fp);
employee_type read_employees(FILE *fp);
void print_flights(int length, flight_type *flights);
void print_employees(int length, employee_type *employees);
void employees_in_time_intervals(int total_flights, int employees_length, flight_type *flights, employee_type *empolyees);
int flights_in_interval(int length, char *interval, flight_type *flights, flight_type *flights_in_interval);
double find_empolyees_in_shifts(int length, flight_type *flights);
double basic_employees_shift(int total_flights, flight_type *flights);
void print_weekschedule(int total_employees, employee_type *employee);
/* end of prototypes */

int main(void)
{
    char flight_file[] = "Flight_information.txt", employee_file[] = "Employees.txt";
    int total_flights = count_lines(flight_file);
    int total_employees = count_lines(employee_file);
    flight_type flights[total_flights];
    employee_type employees[total_employees];
    
    scan_for_flight_data(flight_file, total_flights, flights);
    scan_for_employee_data(employee_file, total_employees, employees);
    //print_weekschedule(total_employees, employees);
    //print_flights(total_flights, flights);
    //print_employees(total_employees, employees);
    
    employees_in_time_intervals(total_flights, total_employees, flights, employees);
}

int count_lines(char *file)
{
    FILE *fp = fopen(file, "r");
    int c, counter = 1;
    
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
        {
            ++counter;
        }
    }
    
    fclose(fp);
    
    return counter;
}

void scan_for_flight_data(char *file, int total_flights, flight_type *flights)
{
    FILE *fp = fopen(file, "r");
    
    for (int i = 0; i < total_flights; ++i)
    {
        flights[i] = read_flights(fp);
    }
    fclose(fp);
}

void scan_for_employee_data(char *file, int total_employees, employee_type *employees)
{
    FILE *fp = fopen(file, "r");
    
    for (int i = 0; i < total_employees; ++i)
    {
        employees[i] = read_employees(fp);
    }
    fclose(fp);
}

flight_type read_flights(FILE *fp)
{
    flight_type flight;
    fscanf(fp, " %s %s %s %s %s %s %d",
           flight.travel_type,
           flight.flight_number,
           flight.date,
           flight.weekday,
           flight.time,
           flight.flight_model,
           &flight.passengers);
    
    return flight;
}

employee_type read_employees(FILE *fp)
{
    employee_type employee;
    fscanf(fp, " %s %s %d",
           employee.first_name,
           employee.last_name,
           &employee.max_hrs);
    
    return employee;
}

void print_flights(int length, flight_type *flights)
{
    for (int i = 0; i < length; ++i)
    {
        printf("%s %s %s %s %s %s %d\n",
               flights[i].travel_type,
               flights[i].flight_number,
               flights[i].date,
               flights[i].weekday,
               flights[i].time,
               flights[i].flight_model,
               flights[i].passengers);
    }
}

void print_employees(int length, employee_type *employees)
{
    for (int i = 0; i < length; ++i)
    {
        printf("%s %s %d\n",
               employees[i].first_name,
               employees[i].last_name,
               employees[i].max_hrs);
    }
}

void employees_in_time_intervals(int total_flights, int employees_length, flight_type *flights, employee_type *empolyees)
{
    char morning_shift[] = "04:00 - 12:00", day_shift[] = "11:30 - 19:30", night_shift[] = "19:00 - 25:00";
    int total_morning_flights, total_day_flights, total_night_flights;
    double morning_employees, day_employees, night_employees;
    
    flight_type morning_flights[total_flights];
    flight_type day_flights[total_flights];
    flight_type night_flights[total_flights];
    
    total_morning_flights = flights_in_interval(total_flights, morning_shift, flights, morning_flights);
    total_day_flights = flights_in_interval(total_flights, day_shift, flights, day_flights);
    total_night_flights = flights_in_interval(total_flights, night_shift, flights, night_flights);
    
    morning_employees = find_empolyees_in_shifts(total_morning_flights, morning_flights);
    day_employees = find_empolyees_in_shifts(total_day_flights, day_flights);
    night_employees = find_empolyees_in_shifts(total_night_flights, night_flights);
    
    printf("%f %f %f",morning_employees, day_employees, night_employees);
}

int flights_in_interval(int length, char *interval, flight_type *flights, flight_type *flights_in_interval)
{
    char interval_start[MAX_TIME], interval_end[MAX_TIME];
    int j = 0;
    sscanf(interval, " %s - %s", interval_start, interval_end);
    
    for (int i = 0; i < length; ++i)
    {
        if (strcmp(flights[i].time, interval_start) >= 0 && strcmp(flights[i].time, interval_end) <= 0)
        {
            flights_in_interval[j] = flights[i];
            j++;
        }
    }
    
    
    return j;
}

double find_empolyees_in_shifts(int length, flight_type *flights)
{
    double basic_employees_pr_shift = basic_employees_shift(length, flights);
    printf("basic: %lf\n",basic_employees_pr_shift);
    int flight_hour, flight_minute, flight_time, flight_hour_cmp, flight_minute_cmp, flight_time_cmp;
    int max_flights_hour_interval = 0, flights_hour_interval = 0;
    int time_flight_hour_interval, total_passengers_hour_interval, passengers;
    double employees;
    
	//printf("start: %.4d end: %.4d\n", start_time, end_time);

    for (int i = 0; i < length ; ++i) 
    {
        //printf("%d flights for %s at %s and 1 hour forward\n\n", flights_hour_interval, flights[i - 1].flight_model, flights[i - 1].time);
        if (max_flights_hour_interval < flights_hour_interval)
        {
            max_flights_hour_interval = flights_hour_interval;
            time_flight_hour_interval = flight_time;
            total_passengers_hour_interval = passengers;
        
        }
        flights_hour_interval = 0;
        passengers = 0;
        sscanf(flights[i].time, "%d:%d", &flight_hour, &flight_minute);
        flight_time = flight_hour * 100 + flight_minute;
        for (int j = i; j < length; ++j)
        {
            sscanf(flights[j].time, "%d:%d", &flight_hour_cmp, &flight_minute_cmp);
            flight_time_cmp = flight_hour_cmp * 100 + flight_minute_cmp;

            //printf("Flight forward 1 hour %d\n", flight_time + HOUR_MILITARYTIME);
            //printf("flight_cmp is this lower?: %d\n", flight_time_cmp);

            if (flight_time <= flight_time_cmp && (flight_time + HOUR_MILITARYTIME) >= flight_time_cmp)
            {
                ++flights_hour_interval;
                passengers += flights[j].passengers;
                
                //printf("YES\n");
            }
        }
    }

    printf("Max flights at %.4d - %.4d is %d with %d\n", time_flight_hour_interval, time_flight_hour_interval+HOUR_MILITARYTIME, max_flights_hour_interval, total_passengers_hour_interval);
    //printf("%lf\n",basic_employees_pr_shift);
    
    employees = (basic_employees_pr_shift) + (max_flights_hour_interval/2) + (total_passengers_hour_interval/PASSENGERS_PER_EMPLOYEE/8);
    
    return employees;
}

double basic_employees_shift(int total_flights, flight_type *flights)
{
    double avg_passengers = 0;
    
    for (int i = 0; i < total_flights; ++i)
    {
        if (!strcmp(flights[i].travel_type, "ARR"))
        {
            avg_passengers += flights[i].passengers;
        } else if (!strcmp(flights[i].travel_type, "DPT"))
        {
            avg_passengers += flights[i].passengers * 1.5;
        }
        
    }
    
    return (avg_passengers/total_flights)/PASSENGERS_PER_EMPLOYEE;
}

void print_weekschedule(int total_employees, employee_type *employee)
{
    printf("\n");
    printf("\t\t\t Monday \t Tuesday \t Wednesday \t Thursday \t Friday \t Saturday \t Sunday\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i < total_employees ; i++)
    {
        printf("%s %3s, %3d | \t %s \t %s \t %s \t %s \t %s \t %s \t %s\n"
                , employee[i].first_name, employee[i].last_name, employee[i].max_hrs, employee[i].worktime.monday, employee[i].worktime.tuesday, employee[i].worktime.wednesday, employee[i].worktime.thursday, employee[i].worktime.friday, employee[i].worktime.saturday, employee[i].worktime.sunday);
    }
    printf("\n");
}