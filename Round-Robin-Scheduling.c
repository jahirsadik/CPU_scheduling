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
                if (i->process.identifier > j->process.identifier)
                {
                    struct Process temp = i->process;
                    i->process = j->process;
                    j->process = temp;
                }
                if (i->process.identifier == j->process.identifier)
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
        printf("Process: %d, Arrival: %d, Burst: %d, identifier: %d\n", i++, head->process.arrival, head->process.burst,
               head->process.identifier);
        head = head->next;
    }
    printf("\n");
}

// Function to implement round robin scheduling
void round_robin(struct Node **head, int time_quantum)
{
    struct Node *temp = *head;
    int time = 0, i = 0, total = 0;
    // Calculating the total burst time
    while (temp != NULL)
    {
        total += temp->process.burst;
        temp = temp->next;
    }
    temp = *head;
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
            if (temp->process.remaining_time == 0)
            {
                printf("Process %d completed at %d\n", temp->process.identifier, time);
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
        printf("Enter the Identifier for process %d: ", i);
        scanf("%d", &process.identifier);
        process.remaining_time = process.burst;
        insert(&head, process);
    }
    sort(&head);
    print(head);
    round_robin(&head, 4);
    return 0;
}