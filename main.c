/* Aalborg University
 * Bachelor Software
 * 1. Semester
 *
 * Project: P1 - Baggage handling
 * Group: B2-1a
 * Group Members:   Rasmus E. Nielsen
 *                  Lukas Rønsholt
 *                  Mickey Svendsen
 *                  Lasse Lundbo
 *                  Alexander Vassiliev
 *                  Gustav Brodersen
 *                  Magnus T. Boisen
 *
 * Description:
 *  Program to make a week schedule, in a effort to eliminate understaffing.
 *  The progam will decide how many employees that are need, depending on how many flights that are
 */

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
#define PASSENGERS_PER_HOUR 310
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

/* Prototypes */
int count_lines(char *file);
int sort_by_hrs(const void *a, const void *b);
int sort_by_name(const void *a, const void *b);
int flights_in_interval(int total_flights, char *interval, flight_type *flights, flight_type *flights_in_interval);
void print_flights(int length, flight_type *flights);
void print_employees(int length, employee_type *employees);
void free_array(flight_array_type *flights);
void print_weekschedule(int total_employees, employee_type *employee);
void set_array_size(flight_array_type *flights, int total_flights[week]);
void get_total_flights(int total_flights[week], char flight_files[week][40]);
void scan_for_flight_data(char *file, int total_flights, flight_type *flights);
void scan_for_employee_data(char *file, int total_employees, employee_type *employees);
void assign_worktime(int total_employees, employee_type *emplyees, double shift_employees[week][3]);
void get_all_flights(char flight_files[week][40], int total_flights[week], flight_array_type *flights);
void get_required_empolyees(int total_flights[week], double shift_employees[week][3], flight_array_type *flights);
void employees_in_time_intervals(int total_flights, flight_type *flights, double *morning_employees, double *day_employees, double *night_employees);
double basic_employees_shift(int total_flights, flight_type *flights);
double find_max_flights_hour_interval(int total_flight_in_shift, flight_type *flights);
double find_employees_in_shifts(int total_flights_in_shift, flight_type *flights_in_shift);
flight_type read_flights(FILE *fp);
employee_type read_employees(FILE *fp);
/* End of prototypes */

int main(void)
{
    srand(time(NULL));                                                                                                  /* Generates a random seed */
    int total_flights[week], total_employees;
    double shift_employees[week][3];
    char flight_files[week][40] = {"Flights/flight-05-12-2016-monday.txt",
                                   "Flights/flight-06-12-2016-tuesday.txt",
                                   "Flights/flight-07-12-2016-wednesday.txt",
                                   "Flights/flight-08-12-2016-thursday.txt",
                                   "Flights/flight-09-12-2016-friday.txt",
                                   "Flights/flight-10-12-2016-saturday.txt",
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
    
    free_array(flights);
}

/*
 * @Description: Puts the total amount of flights in a period of a
 *  day and puts it over to an array with the use of the Count_lines function.
 *
 * @Input: Array with various weekdays including weekday files.
 * @Output: Number of lines in the various weekdays.
 */
void get_total_flights(int total_flights[week], char flight_files[week][40])
{
    for (int i = 0; i < week; ++i)
    {
        total_flights[i] = count_lines(flight_files[i]);
        printf("%d\n",total_flights[i]);
    }
}


/*
 * @Description: Counts the number of lines in the given file until it reaches End of File.
 *
 * @Input: File.
 * @Returns: The number of lines.
 */ 
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
/*
 * @Description: Allocate the needed memory to the array, using calloc to remove any data stored
 *  in the array before resizing.
 *
 * @Input: The flight_array_type array and the array containing the total flight on each day.
 * @Output: The resized array in flight_array_type
 */ 
void set_array_size(flight_array_type *flights, int total_flights[week])
{
    for (int i = 0; i < week; ++i)
    {
        flights[i].flights = (flight_type*)calloc(total_flights[i], sizeof(flight_type));
    }
}
  
/*
 * @Description: Frees the array for a week
 *
 * @Input: The array containing all flights
 * @Output: None.
 */
void free_array(flight_array_type *flights)
{
    for (int i = 0; i < week; ++i)
    {
        free(flights[i].flights);
    }
}

/*
 * @Description: Scans data for a week
 *
 * @Input: File, number of flights in the weekday, and the array where the data can be stored.
 * @Output: Array with flight data
 */    
void get_all_flights(char flight_files[week][40], int total_flights[week], flight_array_type *flights)
{
    for (int i = 0; i < week; ++i)
    {
        scan_for_flight_data(flight_files[i],total_flights[i],flights[i].flights);
    }
}

/*
 * Description: Scans every line in the file.
 *
 * Input: File, number of flights, and the array where the data can be stored.
 * Output: Array with flight data
 */    
void scan_for_flight_data(char *file, int total_flights, flight_type *flights)
{
    FILE *fp = fopen(file, "r");
    
    for (int i = 0; i < total_flights; ++i)
    {
        flights[i] = read_flights(fp);
    }
    fclose(fp);
}
  
/*
 * Description: Scans for the data of the number of employees available.
 *
 * Input: File, number of employees, employee array
 * Output: Employee array with data
*/  
void scan_for_employee_data(char *file, int total_employees, employee_type *employees)
{
    FILE *fp = fopen(file, "r");
    
    for (int i = 0; i < total_employees; ++i)
    {
        employees[i] = read_employees(fp);
    }
    fclose(fp);
}

/*
 * Description: Reads the line that the that the pointer is curently pointing too,
 * and stores the data in a temporary struct Flight_type, and returns it.
 *
 * Input: File. 
 * Returns: Flight data from line.
*/    
  
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

/*
 * Description: Reads the line that the that the pointer is curently pointing too,
 * and stores the data in a temporary struct Flight_type, and returns it.
 *
 * Input: File.
 * Returns: Employee data.
*/
employee_type read_employees(FILE *fp)
{
    employee_type employee;
    fscanf(fp, " %s %s %d",
           employee.first_name,
           employee.last_name,
           &employee.max_hrs);
    
    return employee;
}

/*
 * Description: Prints flight data from an array up to a given length.
 *
 * Input: length, array containing flights.
 * Returns: None.
*/
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

/*
 * Description: Prints employee data from an array up to a given length.
 *
 * Input: length, array containing the employees.
 * Returns: None.
*/
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

/*
 * Description: Get the needed employees for the 3 shift on every day in the week
 *
 * Input: The total number of flight though out the week, the array to hold the employees, the flight array.
 * Returns: The filled out array with the needed employees.
*/
void get_required_empolyees(int total_flights[week], double shift_employees[week][3], flight_array_type *flights)
{
    for (int i = 0; i < week; ++i)
    {
        printf("Day: %d\n",i+1);
        employees_in_time_intervals(total_flights[i], flights[i].flights, &shift_employees[i][0], &shift_employees[i][1], &shift_employees[i][2]);
    }
}

/*
 * Description: Calculates the number of employees needed for the different shifts on a day.
 *
 * Input: The total number of flight, the array containing the flights, and pointers to the 3 shifts
 * Output: The 3 pointers to the shifts.
 */  
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
    
    *morning_employees = find_employees_in_shifts(total_morning_flights, morning_flights);
    *day_employees = find_employees_in_shifts(total_day_flights, day_flights);
    *night_employees = find_employees_in_shifts(total_night_flights, night_flights);
}
  
/*
 * Description: Find, Stores and Calculates the number of flights in the shift interval.
 *
 * Input: The total flights, the interval, the array containing the flights,
 *  the array to hold the flights in the interval
 * Output: Array with the flights in the the shift interval
 * Return: The number of flights found
 */ 
int flights_in_interval(int total_flights, char *interval, flight_type *flights, flight_type *flights_in_interval)
{
    char interval_start[MAX_TIME], interval_end[MAX_TIME];
    int j = 0;
    sscanf(interval, " %s - %s", interval_start, interval_end);                                                         /* Convert the interval to a staring time and a ending time */
    
    for (int i = 0; i < total_flights; ++i)
    {
        if (strcmp(flights[i].time, interval_start) >= 0 && strcmp(flights[i].time, interval_end) <= 0)
        {
            flights_in_interval[j] = flights[i];
            j++;
        }
    }
    
    return j;
}
  
/*
 * Description: Calculates the number of employees needed depending on the flights.
 *
 * Input: The the total flights, the array containing the flights.
 * Returns: Number of employees needed
 */ 
double find_employees_in_shifts(int total_flights_in_shift, flight_type *flights_in_shift)
{
    double basic_employees_pr_shift = basic_employees_shift(total_flights_in_shift, flights_in_shift);
    double employees_in_hour_interval = find_max_flights_hour_interval(total_flights_in_shift, flights_in_shift);
    double employees;
    
    employees = (basic_employees_pr_shift) + (employees_in_hour_interval);
    
    return employees;
}
  
/*
 * Description: Calculates the number of employees needed in a period with most flights in any given shift.
 *
 * Input: The total number of flights, The array containing the flights
 * Returns: Conversion of data which is number of needed employees
 */ 
double find_max_flights_hour_interval(int total_flight_in_shift, flight_type *flights)
{
    int flight_hour, flight_minute, flight_time, flight_hour_cmp, flight_minute_cmp, flight_time_cmp;
    int max_flights_hour_interval = 0, flights_hour_interval = 0;
    int time_flight_hour_interval, total_passengers_hour_interval, passengers;
    
    for (int i = 0; i < total_flight_in_shift ; ++i)
    {
        if (max_flights_hour_interval < flights_hour_interval)                                                          /*If a new max is found save the data*/
        {
            max_flights_hour_interval = flights_hour_interval;
            time_flight_hour_interval = flight_time;
            total_passengers_hour_interval = passengers;
            
        }
        flights_hour_interval = 0;
        passengers = 0;
        sscanf(flights[i].time, "%d:%d", &flight_hour, &flight_minute);                                                 /*Convert the time from char to 2 ints*/
        flight_time = flight_hour * 100 + flight_minute;                                                                /*Convert the 2 ints to one int, holing time in militarytime*/
        for (int j = i; j < total_flight_in_shift; ++j)
        {
            sscanf(flights[j].time, "%d:%d", &flight_hour_cmp, &flight_minute_cmp);                                     /*Convert the time from char to 2 ints*/
            flight_time_cmp = flight_hour_cmp * 100 + flight_minute_cmp;                                                /*Convert the 2 ints to one int, holing time in militarytime*/
            
            if (flight_time <= flight_time_cmp && (flight_time + HOUR_MILITARYTIME) >= flight_time_cmp)                 /* Check if the time is within an hour of the flight from the first loop*/
            {
                ++flights_hour_interval;
                passengers += flights[j].passengers;
            }
        }
    }
    
    return (max_flights_hour_interval/2) + (total_passengers_hour_interval/PASSENGERS_PER_HOUR);                        /* The number of flights halved returns needed employees plus
                                                                                                                         * the total passengers in those flights and the total employees already calculated
                                                                                                                         * is expected to handle 310 passengers in that one busy hour */
}

/*
 * Description: Calculates the number of employees needed to handle the total passengers in the shift.
 *
 * Input: The total flights, the array containing the flights.
 * Returns: Conversion to number of needed employees
 */ 
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
            avg_passengers += flights[i].passengers * 1.5;                                                              /* Departures is expected to need more employees, passengers is increased by 50% */
        }
        
    }
    
    return (avg_passengers/total_flights)/PASSENGERS_PER_EMPLOYEE;                                                      /* Average passengers divided by the amount of passenger expected to be handle by one employee*/
}

/*
 * Description: Assigning worktime to all employeees for all days any shifts.
 *  By sorting the array after the employees working hours, and using the first employee in the
 *  array, the hours need will be divided equal out to the employees, and all get as close the their
 *  max working hours. in case of one or more employees have the same amount of hours, the order need to
 *  be random.
 *
 * Input: The total employees, the array containing the employees, and the array containing the need employees per shift
 * Output: The filled out worktime for the employees.
 */ 
void assign_worktime(int total_employees, employee_type *emplyees, double shift_employees[week][3])
{
    for (int n = 0; n < total_employees; n++)                                                                           /* Initialize the worktime for each employee, to an empty sting*/
    {
        emplyees[n].hrs = 0;
        for (int j = 0; j < week; j++)
        {
            strcpy(emplyees[n].worktime[j],"");
        }
    }

    for (int j = 0; j < week; ++j)
    {
        qsort(emplyees,total_employees, sizeof(employee_type),sort_by_hrs);
        
        for (int k = 0; k < shift_employees[j][0]; ++k)
        {
            if (emplyees[0].hrs <= (emplyees[0].max_hrs - 6))
            {
                strcpy(emplyees[0].worktime[j],"04:00 - 12:00");
                emplyees[0].hrs += 8;
                qsort(emplyees,total_employees, sizeof(employee_type),sort_by_hrs);
            } else
                printf("NO MORE EMPLOYEES\n");
        }
        for (int l = 0; l < shift_employees[j][1]; ++l)
        {
            if (emplyees[0].hrs <= (emplyees[0].max_hrs - 6))
            {
                strcpy(emplyees[0].worktime[j],"11:30 - 19:30");
                emplyees[0].hrs += 8;
                qsort(emplyees,total_employees, sizeof(employee_type),sort_by_hrs);
            } else
                printf("NO MORE EMPLOYEES\n");
        }
        for (int m = 0; m < shift_employees[j][2]; ++m)
        {
            if (emplyees[0].hrs <= (emplyees[0].max_hrs - 6))
            {
                strcpy(emplyees[0].worktime[j],"19:00 - 01:00");
                emplyees[0].hrs += 6;
                qsort(emplyees,total_employees, sizeof(employee_type),sort_by_hrs);
            } else
                printf("NO MORE EMPLOYEES\n");
        }
    }
    qsort(emplyees,total_employees, sizeof(employee_type),sort_by_name);
}

/*
 * Description: Function to be used by qsort, to sort after the employees hours,
 *  and if the hours is equal, sort random.
 *
 * Input: 2 const void.
 * Output: < 0 or > 0
 */
int sort_by_hrs(const void *a, const void *b)
{
    employee_type *pa = (employee_type*)a;
    employee_type *pb = (employee_type*)b;

    if(pa->hrs == pb->hrs)
    {
        int c = rand()%10;
        return (c > 5? -1:1);
    }

    return (pa->hrs - pb->hrs);
}

/*
 * Description: Function to be used by qsort, to sort after the employees first name.
 *
 * Input: 2 const void.
 * Output: < 0, > 0 or 0
 */
int sort_by_name(const void *a, const void *b)
{
    employee_type *pa = (employee_type*)a;
    employee_type *pb = (employee_type*)b;

    return (strcmp(pa->first_name, pb->first_name));
}

/*
 * Description: Print the week schedule to the standard output
 *
 * Input: the total employees, the array containing the employees.
 * Output: None
 */
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