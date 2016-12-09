#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

enum weekdays
{
    monday,
    tuesday,
    wednesday,
    thursday,
    friday,
    saturday,
    sunday,
    week
};

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
    flight_type *flights;
} flight_array_type;

typedef struct {
    char first_name[MAX_NAME];
    char last_name[MAX_NAME];
    int max_hrs;
    int hrs;
    char worktime[week][MAX_WORKTIME];
} employee_type;

/* prototypes */
void get_total_flights(int total_flights[week], char flight_files[week][40]);
int count_lines(char *file);
void set_array_size(flight_array_type *flights, int total_flights[week]);
void free_array(flight_array_type *flights, int total_flights[week]);
void get_all_flights(char flight_files[week][40], int total_flights[week], flight_array_type *flights);
void scan_for_flight_data(char *file, int total_flights, flight_type *flights);
void scan_for_employee_data(char *file, int total_employees, employee_type *employees);
flight_type read_flights(FILE *fp);
employee_type read_employees(FILE *fp);
void print_flights(int length, flight_type *flights);
void print_employees(int length, employee_type *employees);
void get_required_empolyees(int total_flights[week], double shift_employees[week][3], flight_array_type *flights);
void employees_in_time_intervals(int total_flights, flight_type *flights, double *morning_employees, double *day_employees, double *night_employees);
int flights_in_interval(int length, char *interval, flight_type *flights, flight_type *flights_in_interval);
double find_empolyees_in_shifts(int length, flight_type *flights);
double find_max_flights_hour_interval(int length, flight_type *flights);
double basic_employees_shift(int total_flights, flight_type *flights);
void assign_worktime(int total_employees, employee_type *emplyees, double shift_employees[week][3]);
int get_new_number(employee_type *employees, int j);
int myRandom (int size);
void print_weekschedule(int total_employees, employee_type *employee);
/* end of prototypes */

int main(void)
{
    srand (time(NULL));
    int total_flights[week], total_employees;
    double shift_employees[week][3];
    char flight_files[week][40] = {"Flights/flight-05-12-2016-monday.txt","Flights/flight-06-12-2016-tuesday.txt",
                                   "Flights/flight-07-12-2016-wednesday.txt","Flights/flight-08-12-2016-thursday.txt",
                                   "Flights/flight-09-12-2016-friday.txt","Flights/flight-10-12-2016-saturday.txt",
                                   "Flights/flight-11-12-2016-sunday.txt"};
    char employee_file[] = "Employees.txt";
    
    get_total_flights(total_flights,flight_files);
    total_employees = count_lines(employee_file);
    
    flight_array_type flights[week];
    set_array_size(flights, total_flights);
    get_all_flights(flight_files, total_flights, flights);

    employee_type employees[total_employees];
    scan_for_employee_data(employee_file, total_employees, employees);

    get_required_empolyees(total_flights, shift_employees, flights);

    assign_worktime(total_employees, employees, shift_employees);

    //print_employees(total_employees, employees);
    print_weekschedule(total_employees, employees);
    
    free_array(flights, total_flights);
}

void get_total_flights(int total_flights[week], char flight_files[week][40])
{
    for (int i = 0; i < week; ++i)
    {
        total_flights[i] = count_lines(flight_files[i]);
        printf("%d\n",total_flights[i]);
    }
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

void set_array_size(flight_array_type *flights, int total_flights[week])
{
    for (int i = 0; i < week; ++i)
    {
        flights[i].flights = (flight_type*)calloc(total_flights[i], sizeof(flight_type));
    }
}

void free_array(flight_array_type *flights, int total_flights[week])
{
    for (int i = 0; i < week; ++i)
    {
        free(flights[i].flights);
    }
}

void get_all_flights(char flight_files[week][40], int total_flights[week], flight_array_type *flights)
{
    for (int i = 0; i < week; ++i)
    {
        scan_for_flight_data(flight_files[i],total_flights[i],flights[i].flights);
    }
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
        printf("%s %s %d %s %s %s %s %s %s %s\n",
               employees[i].first_name,
               employees[i].last_name,
               employees[i].max_hrs,
               employees[i].worktime[monday],
               employees[i].worktime[tuesday],
               employees[i].worktime[wednesday],
               employees[i].worktime[thursday],
               employees[i].worktime[friday],
               employees[i].worktime[saturday],
               employees[i].worktime[sunday]);
    }
}

void get_required_empolyees(int total_flights[week], double shift_employees[week][3], flight_array_type *flights)
{
    for (int i = 0; i < week; ++i)
    {
        printf("Day: %d\n",i+1);
        employees_in_time_intervals(total_flights[i], flights[i].flights, &shift_employees[i][0], &shift_employees[i][1], &shift_employees[i][2]);
    }
}

void employees_in_time_intervals(int total_flights, flight_type *flights, double *morning_employees, double *day_employees, double *night_employees)
{
    char morning_shift[] = "04:00 - 12:00", day_shift[] = "11:30 - 19:30", night_shift[] = "19:00 - 25:00";
    int total_morning_flights, total_day_flights, total_night_flights;
    
    flight_type morning_flights[total_flights];
    flight_type day_flights[total_flights];
    flight_type night_flights[total_flights];
    
    total_morning_flights = flights_in_interval(total_flights, morning_shift, flights, morning_flights);
    total_day_flights = flights_in_interval(total_flights, day_shift, flights, day_flights);
    total_night_flights = flights_in_interval(total_flights, night_shift, flights, night_flights);
    
    *morning_employees = find_empolyees_in_shifts(total_morning_flights, morning_flights);
    *day_employees = find_empolyees_in_shifts(total_day_flights, day_flights);
    *night_employees = find_empolyees_in_shifts(total_night_flights, night_flights);
    
    printf("%f %f %f\n\n\n",*morning_employees, *day_employees, *night_employees);
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

double find_max_flights_hour_interval(int length, flight_type *flights)
{
    int flight_hour, flight_minute, flight_time, flight_hour_cmp, flight_minute_cmp, flight_time_cmp;
    int max_flights_hour_interval = 0, flights_hour_interval = 0;
    int time_flight_hour_interval, total_passengers_hour_interval, passengers;

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
    return (max_flights_hour_interval/2) + (total_passengers_hour_interval/PASSENGERS_PER_EMPLOYEE/8);
}

double find_empolyees_in_shifts(int length, flight_type *flights)
{
    double basic_employees_pr_shift = basic_employees_shift(length, flights);
    double max_flights_hour_interval = find_max_flights_hour_interval(length, flights);
    double employees;
    
    printf("basic: %lf\n",basic_employees_pr_shift);
	//printf("start: %.4d end: %.4d\n", start_time, end_time);
    //printf("%lf\n",basic_employees_pr_shift);
    
    employees = 5; //(basic_employees_pr_shift) + (max_flights_hour_interval);
    
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

void assign_worktime(int total_employees, employee_type *emplyees, double shift_employees[week][3])
{
    int i;
    for (int n = 0; n < total_employees; ++n)
    {
        emplyees[n].hrs = 0;
        for (int j = 0; j < week; ++j)
        {
            strcpy(emplyees[n].worktime[j],"");
        }
    }

    for (int j = 0; j < week; ++j)
    {
        i = myRandom(total_employees);
        for (int k = 0; k < shift_employees[j][0]; ++k)
        {
            strcpy(emplyees[i].worktime[j],"04:00 - 12:00");
            emplyees[i].hrs += 8;
            i = get_new_number(emplyees,j);
        }
        for (int l = 0; l < shift_employees[j][1]; ++l)
        {
            strcpy(emplyees[i].worktime[j],"11:30 - 19:30");
            emplyees[i].hrs += 8;
            i = get_new_number(emplyees,j);;
        }
        for (int m = 0; m < shift_employees[j][2]; ++m)
        {
            strcpy(emplyees[i].worktime[j],"19:00 - 01:00");
            emplyees[i].hrs += 6;
            i = get_new_number(emplyees,j);
        }
        myRandom(-2);
    }
}

int get_new_number(employee_type *employees, int j)
{
    int i = myRandom(-1);
    
    if (employees[i].hrs > employees[i].max_hrs-8)
    {
        do {
            i = myRandom(-1);
            if (i == -1 || i == -2)
            {
                printf("error: %d j = %d\n",i,j);
                if (i == -1)
                {
                    printf("No more available employees\n");
                } else
                {
                    printf("No more memory\n");
                }
                exit(-1);
            }
        } while (employees[i].hrs > employees[i].max_hrs-8);
    }
    return i;
}

int myRandom (int size)
{
    int i, n;
    static int numNums = 0;
    static int *numArr = NULL;

    // Initialize with a specific size.
    
    if (size == -2)
    {
        free(numArr);
        numNums = 0;
        return -3;
    }
    
    if (size >= 0) {
        if (numArr != NULL)
            free (numArr);
        if ((numArr = malloc (sizeof(int) * size)) == NULL)
            return -2;
        for (i = 0; i  < size; i++)
            numArr[i] = i;
        numNums = size;
    }

    // Error if no numbers left in pool.

    if (numNums == 0)
        return -1;

    // Get random number from pool and remove it (rnd in this
    //   case returns a number between 0 and numNums-1 inclusive).

    n = rand() % numNums;
    i = numArr[n];
    numArr[n] = numArr[numNums-1];
    numNums--;
    if (numNums == 0) {
        free (numArr);
        numArr = 0;
    }

    return i;
}

void print_weekschedule(int total_employees, employee_type *employee)
{
    printf("\n");
    printf("Employee \t        |     Monday    |    Tuesday    |   Wednesday   |   Thursday    |    Friday     |   Saturday    |     Sunday    |\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------| \n");
    for(int i = 0 ; i < total_employees ; i++)
    {
        printf("%-10s %-12s | %13s | %13s | %13s | %13s | %13s | %13s | %13s | %d\n",
               employee[i].first_name,
               employee[i].last_name,
               employee[i].worktime[monday],
               employee[i].worktime[tuesday],
               employee[i].worktime[wednesday],
               employee[i].worktime[thursday],
               employee[i].worktime[friday],
               employee[i].worktime[saturday],
               employee[i].worktime[sunday],
               employee[i].hrs);
    }
    printf("\n");
}