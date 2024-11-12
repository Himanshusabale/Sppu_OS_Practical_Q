#include <stdio.h>
#include <stdbool.h>

struct Process {
    int id, arrival, burst, remaining, completion, wait, turnaround;
};

void print_gantt_chart(int gantt_chart[], int gantt_count) {
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < gantt_count; i++) {
        printf(" P%d |", gantt_chart[i]);
    }
    printf("\n");
    printf("0");  // Start time
    for (int i = 1; i <= gantt_count; i++) {
        printf("  %d", i);  // Time labels under each process
    }
    printf("\n");
}

void SJF_Preemptive(struct Process p[], int n) {
    int time = 0, completed = 0, shortest; // Initialize time, completed processes count, and shortest process index
    float total_wait = 0, total_turnaround = 0; // Initialize total wait and turnaround times
    int gantt_chart[1000];  // Gantt chart log for SJF
    int gantt_count = 0; // Counter for Gantt chart entries

    while (completed < n) { // Loop until all processes are completed
        shortest = -1; // Reset shortest process index
        for (int i = 0; i < n; i++) { // Iterate through all processes
            // Check if process has arrived, is remaining, and is shorter than the current shortest
            if (p[i].arrival <= time && p[i].remaining > 0 &&
                (shortest == -1 || p[i].remaining < p[shortest].remaining))
                shortest = i; // Update the shortest process index
        }

        if (shortest == -1) { time++; continue; } // If no process is available, increment time and continue

        p[shortest].remaining--; // Decrement remaining time for the selected process
        gantt_chart[gantt_count++] = p[shortest].id; // Record the process ID in the Gantt chart
        time++; // Increment the current time

        if (p[shortest].remaining == 0) { // Check if the selected process has completed
            completed++; // Increment the completed processes count
            p[shortest].completion = time; // Set completion time for the process
            p[shortest].turnaround = time - p[shortest].arrival; // Calculate turnaround time
            p[shortest].wait = p[shortest].turnaround - p[shortest].burst; // Calculate waiting time
            total_wait += p[shortest].wait; // Accumulate total wait time
            total_turnaround += p[shortest].turnaround; // Accumulate total turnaround time
        }
    }

    // Print results
    printf("\nSJF (Preemptive):\nPID\tArr\tBurst\tComp\tWait\tTurn\n");
    for (int i = 0; i < n; i++) // Iterate through all processes to display their info
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].arrival, p[i].burst, p[i].completion, p[i].wait, p[i].turnaround);

    // Print average wait and turnaround times
    printf("Avg Wait Time: %.2f, Avg Turnaround Time: %.2f\n", total_wait / n, total_turnaround / n);
    print_gantt_chart(gantt_chart, gantt_count); // Call function to print the Gantt chart
}


void RoundRobin(struct Process p[], int n, int quantum) {
    int time = 0, completed = 0;
    float total_wait = 0, total_turnaround = 0;
    int gantt_chart[1000];  // Gantt chart log for Round Robin
    int gantt_count = 0;

    while (completed < n) {
        bool idle = true;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0) {
                idle = false;
                int exec_time = (p[i].remaining > quantum) ? quantum : p[i].remaining;
                

                p[i].remaining -= exec_time;
                for (int j = 0; j < exec_time; j++) {
                    gantt_chart[gantt_count++] = p[i].id; // Record for Gantt chart
                }
                time += exec_time;
                
                if (p[i].remaining == 0) {
                    p[i].completion = time;
                    p[i].turnaround = time - p[i].arrival;
                    p[i].wait = p[i].turnaround - p[i].burst;
                    total_wait += p[i].wait;
                    total_turnaround += p[i].turnaround;
                    completed++;
                }
            }
        }
        if (idle) {
            gantt_chart[gantt_count++] = -1; // Mark idle time in Gantt chart
            time++;
        }
    }

    printf("\nRound Robin:\nPID\tArr\tBurst\tComp\tWait\tTurn\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].arrival, p[i].burst, p[i].completion, p[i].wait, p[i].turnaround);

    printf("Avg Wait Time: %.2f, Avg Turnaround Time: %.2f\n", total_wait / n, total_turnaround / n);
    print_gantt_chart(gantt_chart, gantt_count);
}

int main() {
    int n, quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter arrival and burst time for P%d: ", i + 1);
        scanf("%d %d", &p[i].arrival, &p[i].burst);
        p[i].remaining = p[i].burst;
    }

    printf("Enter time quantum for Round Robin: ");
    scanf("%d", &quantum);

    SJF_Preemptive(p, n);

    // Reset remaining time for Round Robin
    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    RoundRobin(p, n, quantum);
    return 0;
}

// gcc -o scheduler scheduler.c
// ./scheduler



#include <stdio.h>
 #include <limits.h> // For INT_MAX
 typedef struct {
 int pid;
 int arrival_time;
 int burst_time;
 // Process ID
 // Arrival Time
 // Burst Time
 int remaining_time; // Remaining Time for SJF
 int completion_time; // Completion Time
 int waiting_time;
 // Waiting Time
 int turnaround_time; // Turnaround Time
 } Process;
 void calculate_sjf(Process proc[], int n) {
 int completed = 0, time = 0;
 int min_index, shortest_remaining_time;
 int is_completed[n];
 // Initialize remaining times and completion status for all processes
 for (int i = 0; i < n; i++) {
 is_completed[i] = 0;
 proc[i].remaining_time = proc[i].burst_time;
 }
 // Keep running until all processes are completed
 while (completed != n) {
 min_index =-1;
 shortest_remaining_time = INT_MAX;
 // Find the process with the shortest remaining time that has arrived
 for (int i = 0; i < n; i++) {
 if (proc[i].arrival_time <= time && !is_completed[i] && proc[i].remaining_time <
 shortest_remaining_time) {
 shortest_remaining_time = proc[i].remaining_time;
 min_index = i;
 }
 }
 // If no process is ready at the current time, increment time
 if (min_index ==-1) {
 time++;
 } else {
 // Execute the process with the shortest remaining time
 time += proc[min_index].remaining_time; // Execute the process fully
proc[min_index].remaining_time = 0;
 // Process is now completed
 proc[min_index].completion_time = time;
 proc[min_index].turnaround_time = proc[min_index].completion_time
proc[min_index].arrival_time;
 proc[min_index].waiting_time = proc[min_index].turnaround_time
proc[min_index].burst_time;
 is_completed[min_index] = 1;
 completed++;
 }
 }
 }
 void print_table(Process proc[], int n) {
 printf("\nPID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");
 for (int i = 0; i < n; i++) {
 printf("%d\t%d\t%d\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].arrival_time, proc[i].burst_time,
 proc[i].completion_time, proc[i].turnaround_time, proc[i].waiting_time);
 }
 }
 int main() {
 int n;
 printf("Enter the number of processes: ");
 scanf("%d", &n);
 Process proc[n];
 for (int i = 0; i < n; i++) {
 proc[i].pid = i + 1;
 printf("Enter Arrival Time and Burst Time for Process %d: ", i + 1);
 scanf("%d %d", &proc[i].arrival_time, &proc[i].burst_time);
 proc[i].remaining_time = proc[i].burst_time;
 }
 // Call SJF preemptive scheduling
 calculate_sjf(proc, n);
 // Print process table
 print_table(proc, n);
 return 0;
}


new 


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
// Structure to represent a process
struct Process {
int pid; // Process ID
int arrival_time;
int burst_time;
int remaining_time;
int completion_time;
int waiting_time;
int turnaround_time;
};
int total_waiting_time = 0;
int total_turnaround_time = 0;

// Function to implement SJF (Preemptive)
void findWaitingTimeSJF(struct Process proc[], int n) {
int completed = 0, current_time = 0;
int min_time = INT_MAX, shortest = 0;
bool found = false;
while (completed != n) {
for (int i = 0; i < n; i++) {
if (proc[i].arrival_time <= current_time && proc[i].remaining_time < min_time && proc[i].remaining_time > 0) {
min_time = proc[i].remaining_time;
shortest = i;
found = true;
}
}
if (!found) {
current_time++;
continue;
}
proc[shortest].remaining_time--;
min_time = proc[shortest].remaining_time;
if (proc[shortest].remaining_time == 0) {
completed++;
found = false;
min_time = INT_MAX;
proc[shortest].completion_time = current_time + 1;
proc[shortest].turnaround_time = proc[shortest].completion_time - proc[shortest].arrival_time;
proc[shortest].waiting_time = proc[shortest].turnaround_time - proc[shortest].burst_time;
}
current_time++;
}
}

// Function to implement RR
void findWaitingTimeRR(struct Process proc[], int n, int quantum) {
int current_time = 0;
int completed = 0;
while (completed != n) {
for (int i = 0; i < n; i++) {
if (proc[i].remaining_time > 0 && proc[i].arrival_time <= current_time) {
if (proc[i].remaining_time > quantum) {
current_time += quantum;
proc[i].remaining_time -= quantum;
}else{
current_time += proc[i].remaining_time;
proc[i].completion_time = current_time;
proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
proc[i].remaining_time = 0;
completed++;
}
}
}
}
}
// Function to print the results
void printResults(struct Process proc[], int n) {
total_waiting_time = 0;
total_turnaround_time = 0;
printf("PID\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
for (int i = 0; i < n; i++) {
printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
proc[i].pid, proc[i].arrival_time, proc[i].burst_time,
proc[i].completion_time, proc[i].waiting_time, proc[i].turnaround_time);
total_waiting_time += proc[i].waiting_time;
total_turnaround_time += proc[i].turnaround_time;
}
float avg_waiting_time = (float)total_waiting_time / n;
float avg_turnaround_time = (float)total_turnaround_time / n;
printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
}

// Main function
int main() {
int n, quantum;
printf("Enter number of processes: ");
scanf("%d", &n);
struct Process proc[n];
for (int i = 0; i < n; i++) {
proc[i].pid = i + 1;
printf("Enter arrival time for Process %d: ", i + 1);
scanf("%d", &proc[i].arrival_time);
printf("Enter burst time for Process %d: ", i + 1);
scanf("%d", &proc[i].burst_time);
proc[i].remaining_time = proc[i].burst_time;
}
int choice;
printf("Choose Scheduling Algorithm:\n1. Shortest Job First (Preemptive)\n2. Round Robin\nEnter your choice: ");
scanf("%d", &choice);
switch (choice) {
case 1:
findWaitingTimeSJF(proc, n);

break;
case 2:
printf("Enter time quantum for Round Robin: ");
scanf("%d", &quantum);
findWaitingTimeRR(proc, n, quantum);
break;
default:
printf("Invalid choice!\n");
return 1;
}
printResults(proc, n);
return 0;
}
