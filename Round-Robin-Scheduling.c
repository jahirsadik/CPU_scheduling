#include <stdio.h>
#include <stdlib.h>

// Struct named Process with members arrival, burst, identifier and remaining_time
struct Process
{
    int arrival;
    int burst;
    int identifier;
    int remaining_time;
};

// Linked list Struct to store the processes
struct Node
{
    struct Process process;
    struct Node *next;
};

// Function to insert a process at the end of the linked list
void insert(struct Node **head, struct Process process)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->process = process;
    new_node->next = NULL;
    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    struct Node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_node;
}

// Function to insert a process before a given process
void insert_before(struct Node **head, struct Process process, struct Process before)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->process = process;
    new_node->next = NULL;
    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    struct Node *temp = *head;
    if (temp->process.identifier == before.identifier)
    {
        new_node->next = temp;
        *head = new_node;
        return;
    }
    while (temp->next != NULL)
    {
        if (temp->next->process.identifier == before.identifier)
        {
            new_node->next = temp->next;
            temp->next = new_node;
            return;
        }
        temp = temp->next;
    }
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

// Function to sort the linked list based on burst time
void sort_burst(struct Node **head)
{
    struct Node *i, *j;
    for (i = *head; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
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

// Function to print the linked list
void print(struct Node *head)
{
    // Printing the linked list
    printf("Printing the Linked List:\n");
    int i = 1;
    while (head != NULL)
    {
        printf("Process: %d, Arrival: %d, Burst: %d, identifier: %d Remaining: %d\n", head->process.identifier, head->process.arrival, head->process.burst,
               head->process.identifier, head->process.remaining_time);
        head = head->next;
    }
    printf("\n");
}

// Function to initialize the linked list
void initProcessesArray(struct Node *head, struct Process processes[])
{
    int i = 0;
    while (head != NULL)
    {
        processes[i++] = head->process;
        head = head->next;
    }
}

// Function to implement round robin scheduling
void round_robin(struct Node **head, int time_quantum, struct Process processes[], int noOfProcesses)
{
    int total_waiting_time = 0, total_turnaround_time = 0;
    struct Node *temp = *head;
    int time = 0, i = 0, total = 0;
    // Calculating the total burst time
    for (int j = 0; j < noOfProcesses; j++)
    {
        total += processes[j].burst;
    }
    // find size of linked list
    int k = 0;
    while (temp != NULL)
    {
        k++;
        temp = temp->next;
    }
    temp = *head;
    printf("--------------------------SIMULATION OF ROUND ROBIN--------------------------\n");
    // Looping until the total burst time is reached
    while (time < total)
    {
        // If the process has remaining time & the arrival time is less than equal to the current time
        if (temp->process.remaining_time > 0 && temp->process.arrival <= time)
        {
            printf("Time %d-%d: Process %d\n", time, time + time_quantum, temp->process.identifier);
            // If the remaining time is greater than the time quantum
            if (temp->process.remaining_time > time_quantum)
            {
                // Reducing the remaining time by the time quantum
                temp->process.remaining_time -= time_quantum;
                // Increasing the time by the time quantum
                time += time_quantum;
            }
            else
            {
                // Increasing the time by the remaining time
                time += temp->process.remaining_time;
                // Setting the remaining time to 0
                temp->process.remaining_time = 0;
            }
            while (processes[k].arrival <= time && k < noOfProcesses)
            {
                insert_before(head, processes[k], temp->process);
                k++;
            }
            if (temp->process.remaining_time == 0)
            {
                // print the processes finishing time, turn around time and waiting time
                printf("Process %d finished at time %d\n", temp->process.identifier, time);
                printf("Turn around time: %d\n", time - temp->process.arrival);
                printf("Waiting time: %d\n", time - temp->process.arrival - temp->process.burst);
                total_waiting_time = total_waiting_time + (time - temp->process.arrival - temp->process.burst);
                total_turnaround_time = total_turnaround_time + (time - temp->process.arrival);
            }
        }

        // If the temp is not the last node
        if (temp->next != NULL)
        {
            // Setting the temp to the next node
            temp = temp->next;
        }
        // If the temp is the last node
        else
        {
            // Setting the temp to the head of the linked list
            temp = *head;
        }
    }
    printf("--------------------------ANALYSIS OF ROUND ROBIN--------------------------\n");
    // print average waiting time and average turn around time
    printf("Average waiting time: %f, Average turnaround time: %f\n\n\n", (float)total_waiting_time / noOfProcesses,
           (float)total_turnaround_time / noOfProcesses);
}

// Function to implement round robin scheduling
void SJF(struct Node **head, struct Process processes[], int noOfProcesses)
{
    int total_waiting_time = 0, total_turnaround_time = 0;
    struct Node *temp = *head;
    int time = 0, i = 0, total = 0;
    // Calculating the total burst time
    for (int j = 0; j < noOfProcesses; j++)
    {
        total += processes[j].burst;
    }
    // find size of linked list
    int k = 0;
    while (temp != NULL)
    {
        k++;
        temp = temp->next;
    }
    temp = *head;
    printf("--------------------------SIMULATION OF SJF(Pre-emptive)--------------------------\n");
    // Looping until the total burst time is reached
    while (time < total)
    {
        // If the process has remaining time & the arrival time is less than equal to the current time
        if (temp->process.remaining_time > 0 && temp->process.arrival <= time)
        {
            int endEstimate = time + temp->process.remaining_time;
            int minBurst = temp->process.burst, minBurstArrival = temp->process.arrival;
            while (processes[k].arrival <= endEstimate && k < noOfProcesses)
            {
                insert(head, processes[k]);
                if (processes[k].burst < minBurst)
                {
                    minBurst = processes[k].burst;
                    minBurstArrival = processes[k].arrival;
                    k++;
                    break;
                }
                k++;
            }
            if (temp->process.burst != minBurst)
            {
                // pre-emp
                printf("Time %d-%d: Process %d\n", time, minBurstArrival, temp->process.identifier);
                temp->process.remaining_time -= (minBurstArrival - time);
                time = minBurstArrival;
                sort_burst(head);
                temp = *head;
            }
            else
            {
                temp->process.remaining_time = 0;
                // run till end
                printf("Time %d-%d: Process %d\n", time, endEstimate, temp->process.identifier);
                time = endEstimate;
                // print the processes finishing time, turn around time and waiting time
                printf("Process %d finished at time %d\n", temp->process.identifier, time);
                printf("Turn around time: %d\n", time - temp->process.arrival);
                printf("Waiting time: %d\n", time - temp->process.arrival - temp->process.burst);
                total_waiting_time = total_waiting_time + (time - temp->process.arrival - temp->process.burst);
                total_turnaround_time = total_turnaround_time + (time - temp->process.arrival);
                sort_burst(head);
                temp = *head;
            }
        }
        else
        {
            // If the temp is not the last node
            if (temp->next != NULL)
            {
                // Setting the temp to the next node
                temp = temp->next;
            }
            // If the temp is the last node
            else
            {
                // Setting the temp to the head of the linked list
                temp = *head;
            }
        }
    }

    printf("\n--------------------------ANALYSIS OF SJF(Pre-emptive)--------------------------\n");
    // print average waiting time and average turn around time
    printf("Average waiting time: %f, Average turnaround time: %f\n", (float)total_waiting_time / noOfProcesses,
           (float)total_turnaround_time / noOfProcesses);
}

int main()
{
    // Number of processes
    int number_of_processes;
    printf("Enter the number of processes: ");
    scanf("%d", &number_of_processes);
    // Process array
    struct Process processes[number_of_processes];
    struct Node *head = NULL;
    // Taking input for the processes
    for (int i = 0; i < number_of_processes; i++)
    {
        struct Process process;
        printf("Enter the arrival time for process %d: ", i);
        scanf("%d", &process.arrival);
        printf("Enter the burst time for process %d: ", i);
        scanf("%d", &process.burst);
        printf("Enter the Identifier for process %d: ", i);
        scanf("%d", &process.identifier);
        process.remaining_time = process.burst;
        insert(&head, process);
    }

    // ROUND ROBIN

    sort(&head);
    initProcessesArray(head, processes);
    // initial process
    struct Process initial_process = processes[0];
    head = NULL;
    insert(&head, initial_process);
    int i = 1;
    while (processes[i].arrival == initial_process.arrival)
    {
        insert(&head, processes[i]);
        i++;
    }
    round_robin(&head, 4, processes, number_of_processes);

    // SJF
    head = NULL;
    insert(&head, initial_process);
    i = 1;
    while (processes[i].arrival == initial_process.arrival)
    {
        insert(&head, processes[i]);
        i++;
    }
    SJF(&head, processes, number_of_processes);
    return 0;
}