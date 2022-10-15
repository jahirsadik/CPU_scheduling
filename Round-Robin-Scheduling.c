#include <stdio.h>
#include <stdlib.h>

// Struct named Process with members arrival, burst, priority and remaining_time
struct Process
{
    int arrival;
    int burst;
    int priority;
    int remaining_time;
};

// Linked list Struct to store the processes
struct Node
{
    struct Process process;
    struct Node *next;
};

// Function to insert a process into the linked list
void insert(struct Node **head, struct Process process)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->process = process;
    new_node->next = *head;
    *head = new_node;
}

// Function to remove a process from the linked list
void remove_process(struct Node **head)
{
    if (*head == NULL)
    {
        printf("Empty List");
        return;
    }
    struct Node *temp = *head;
    *head = (*head)->next;
    free(temp);
}

// Function to sort the linked list based on arrival time
void sort(struct Node **head)
{
    struct Node *i, *j;
    for (i = *head; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (i->process.arrival > j->process.arrival)
            {
                struct Process temp = i->process;
                i->process = j->process;
                j->process = temp;
            }
            if (i->process.arrival == j->process.arrival)
            {
                if (i->process.priority > j->process.priority)
                {
                    struct Process temp = i->process;
                    i->process = j->process;
                    j->process = temp;
                }
                if (i->process.priority == j->process.priority)
                {
                    if (i->process.burst > j->process.burst)
                    {
                        struct Process temp = i->process;
                        i->process = j->process;
                        j->process = temp;
                    }
                }
            }
        }
    }
}

// Function to print the linked list
void print(struct Node *head)
{
    // Printing the linked list
    printf("Printing the Linked List:\n");
    int i = 1;
    while (head != NULL)
    {
        printf("Process: %d, Arrival: %d, Burst: %d, Priority: %d\n", i++, head->process.arrival, head->process.burst,
               head->process.priority);
        head = head->next;
    }
    printf("\n");
}

// Function to calculate the waiting time for each process
void calculate_waiting_time(struct Node *head, int *waiting_time)
{
    int time = 0;
    int count = 0;
    while (head != NULL)
    {
        if (head->process.arrival > time)
        {
            time = head->process.arrival;
        }
        waiting_time[count] = time - head->process.arrival;
        time += head->process.burst;
        count++;
        head = head->next;
    }
}

// Function to calculate the turnaround time for each process
void calculate_turnaround_time(struct Node *head, int *turnaround_time, int *waiting_time)
{
    int count = 0;
    while (head != NULL)
    {
        turnaround_time[count] = head->process.burst + waiting_time[count];
        count++;
        head = head->next;
    }
}

// Function to calculate the average waiting time for each process
void calculate_average_waiting_time(int *waiting_time, int number_of_processes)
{
    int sum = 0;
    for (int i = 0; i < number_of_processes; i++)
    {
        sum += waiting_time[i];
    }
    printf("Average Waiting Time: %f\n", (float)sum / number_of_processes);
}

// Function to calculate the average turnaround time for each process
void calculate_average_turnaround_time(int *turnaround_time, int number_of_processes)
{
    int sum = 0;
    for (int i = 0; i < number_of_processes; i++)
    {
        sum += turnaround_time[i];
    }
    printf("Average Turnaround Time: %f\n", (float)sum / number_of_processes);
}

// print the Gantt chart for round robin scheduling
void print_gantt_chart(struct Node *head, int *waiting_time)
{
    printf("Gantt Chart:\n");
    int time = 0;
    while (head != NULL)
    {
        if (head->process.arrival > time)
        {
            time = head->process.arrival;
        }
        printf("%d ", time);
        time += head->process.burst;
        printf("%d ", time);
        printf("P%d ", head->process.priority);
        head = head->next;
    }
    printf("\n");
}

// Round Robin Scheduling Algorithm
void round_robin_scheduling(struct Node *head, int time_quantum)
{
    // Sorting the linked list based on arrival time
    sort(&head);
    // Printing the linked list
    print(head);
    // Calculating the number of processes
    int number_of_processes = 0;
    struct Node *temp = head;
    while (temp != NULL)
    {
        number_of_processes++;
        temp = temp->next;
    }
    // Creating an array to store the waiting time for each process
    int *waiting_time = (int *)malloc(number_of_processes * sizeof(int));
    // Creating an array to store the turnaround time for each process
    int *turnaround_time = (int *)malloc(number_of_processes * sizeof(int));
    // Calculating the waiting time for each process
    calculate_waiting_time(head, waiting_time);
    // Calculating the turnaround time for each process
    calculate_turnaround_time(head, turnaround_time, waiting_time);
    // Calculating the average waiting time
    calculate_average_waiting_time(waiting_time, number_of_processes);
    // Calculating the average turnaround time
    calculate_average_turnaround_time(turnaround_time, number_of_processes);
    // Printing the Gantt Chart
    print_gantt_chart(head, waiting_time);
}

int main()
{
    // Number of processes
    int number_of_processes;
    printf("Enter the number of processes: ");
    scanf("%d", &number_of_processes);

    // Array to store the waiting time for each process
    int waiting_time[number_of_processes];

    // Array to store the turnaround time for each process
    int turnaround_time[number_of_processes];

    // Linked list to store the processes
    struct Node *head = NULL;

    // Taking input for the processes
    for (int i = 0; i < number_of_processes; i++)
    {
        struct Process process;
        printf("Enter the arrival time for process %d: ", i);
        scanf("%d", &process.arrival);
        printf("Enter the burst time for process %d: ", i);
        scanf("%d", &process.burst);
        printf("Enter the priority for process %d: ", i);
        scanf("%d", &process.priority);
        process.remaining_time = process.burst;
        insert(&head, process);
    }
    round_robin_scheduling(head, 1);

    // // Calculating the waiting time for each process
    // calculate_waiting_time(head, waiting_time);
    // print_gantt_chart(head, waiting_time);
    // // Calculating the turnaround time for each process
    // calculate_turnaround_time(head, turnaround_time, waiting_time);

    // // Calculating the average waiting time for each process
    // calculate_average_waiting_time(waiting_time, number_of_processes);

    // // Calculating the average turnaround time for each process
    // calculate_average_turnaround_time(turnaround_time, number_of_processes);
    return 0;
}