/* 
Group 909
1. Myungsoo Son 20417585
2. Dharmendra A/L Saravanakumar 20321437
3. Muhammad Fadil Amin Bin Arsani 20515959
4. Ivan Chan Jia Zen 20409322
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for representing a process
struct Process {
    int pid; // Process ID
    float arrivalTime; // moment when a process enters the ready queue and is awaiting execution by the CPU
    float waitingTime; // process's duration in the ready queue before it begins executing
    float burstTime; // amount of CPU time the process requires to complete its execution.
    float turnaroundTime; // the time difference between the arrival of a process and its completion
    float completionTime; // the time when a process finishes execution and is no longer being processed by the CPU.
    float startTime; // the time when a process starts the execution
    float remainingTime; // Remaining time out of burst time
    float responseTime; // Duration between the arrival of a process and the first time it runs
    int queue; // Queue identifier for MLFQ(0 for nothing, 1 for RR, 2 for FCFS)
};

// Structure for storing the results of a scheduling algorithm
struct Result {
    char name[20]; //name of the scheduling algorithm
    float avgResponseTime; //average of response time
    float avgWaitingTime; //average of waiting time
    float avgTurnaroundTime; //average of turnaround time
    float completionTime; //average of completion time
    float throughput; //CPU Performance
};

// Function prototypes
void inputProcessInformation(struct Process *p, int n);
void sortByArrivalTime(struct Process *p, int n);
void FCFS(struct Process *p, int n);
void RR(struct Process *p, int n, float timeQuantum);
void MLFQ(struct Process *p, int n, float timeQuantum);
void displayTable(struct Process p[], int n);
void getAverage(struct Process p[], struct Result *r, int n);
void findBestAlgorithm(struct Result results[], int n);

// Main function
int main() {
    // Initialize variables
    int n;
    char a;
    float timeQuantum;

    // Initialize individual elements of the Result structure
    struct Result r[3];
    strcpy(r[0].name, "FCFS");
    strcpy(r[1].name, "RR");
    strcpy(r[2].name, "MLFQ");

    printf("\n========== PROCESS SCHEDULING ==========\n\n");

    // Infinite loop for processing the user input
    while (1) {
        // Prompt user to enter the total number of processes
        printf("Enter total number of processes : ");
        scanf("%d", &n);

        // Validate user input for the number of processes
        if (n <= 0) {
            printf("INVALID NUMBER\n\n");
            continue; // Ask user to input a valid number
        }

        // Array to store process information
        struct Process p[n];

        // Function to input process information from the user
        inputProcessInformation(p, n);

        // Sort processes by arrival time
        sortByArrivalTime(p, n);

        // Prompt user to enter time quantum for RR and MLFQ algorithms with validation for positivity
        do {
            printf("\nEnter Time Quantum : ");
            scanf("%f", &timeQuantum);
            if (timeQuantum <= 0) {
                printf("Error: Time quantum must be a positive number.\n");
            }
        } while (timeQuantum <= 0);

        // Copy the process for each algorithm
        struct Process p1[n];
        struct Process p2[n];
        struct Process p3[n];
        for (int i = 0; i < n; i++) {
            p1[i] = p[i];
            p2[i] = p[i];
            p3[i] = p[i];
        }

        // Apply First Come First Served (FCFS) algorithm
        FCFS(p1, n);

        // Apply Round Robin (RR) algorithm
        RR(p2, n, timeQuantum);

        // Apply Multi-level Feedback Queue (MLFQ) algorithm
        MLFQ(p3, n, timeQuantum);

        // Display results for FCFS algorithm
        printf("\n\n===== First Come First Served =====\n");
        displayTable(p1, n);
        getAverage(p1, &r[0], n);

        // Display results for RR algorithm
        printf("\n\n===== Round Robin =====\n");
        displayTable(p2, n);
        getAverage(p2, &r[1], n);

        // Display results for MLFQ algorithm
        printf("\n\n===== Multi-level Feedback Queue =====\n");
        displayTable(p3, n);
        getAverage(p3, &r[2], n);

        // Display overall results
        printf("\n\n========== Result ==========\n\n");
        printf("Name\tAverage Waiting Time\tAverage Response Time\tAverage Turnaround Time\tCompletion Time\tThroughput\n");
        for (int i = 0; i < 3; i++) {
            printf("%s\t\t%f\t\t%f\t\t%f\t\t%f\t%f\n", r[i].name, r[i].avgWaitingTime, r[i].avgResponseTime, r[i].avgTurnaroundTime, r[i].completionTime, r[i].throughput);
        }

        // Find the best algorithm and print its name
        findBestAlgorithm(r, n);

        // Prompt user to continue the simulation
        printf("Do you wish to continue the simulation? (Y/N) : ");
        scanf(" %c", &a);  // Note the space before %c to consume any whitespace characters

        // Exit the loop if the user enters 'N' or 'n'
        if (a == 'N' || a == 'n') {
            break;
        }
    }

    return 0;
}



// Sort processes by arrival time using Bubble Sort algorithm
void sortByArrivalTime(struct Process *p, int n) {
    struct Process temp;
    // Implement Bubble Sort
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Compare arrival times of adjacent processes
            if (p[j].arrivalTime > p[j + 1].arrivalTime) {
                // Swap processes if they are out of order
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}


// Display the table of processes
void displayTable(struct Process p[], int n) {
    // Check the queue type to determine the display format
    if (p[0].queue == 0) {
        // Displaying the result of MLFQ
        printf("\nProcess ID\tArrival Time\tResponse Time\tStart Time\tBurst Time\tRemaining Time\tWaiting Time\tCompletion Time\tTurnaround Time\n");
        for (int i = 0; i < n; i++) {
            printf("Process[%d]\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", p[i].pid, p[i].arrivalTime, p[i].responseTime, p[i].startTime, p[i].burstTime, p[i].remainingTime, p[i].waitingTime, p[i].completionTime, p[i].turnaroundTime);
        }
    } else {
        // Displaying the result of FCFS or RR
        printf("\nProcess ID\tQueue\tArrival Time\tResponse Time\tStart Time\tBurst Time\tRemaining Time\tWaiting Time\tCompletion Time\tTurnaround Time\n");
        for (int i = 0; i < n; i++) {
            printf("Process[%d]\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", p[i].pid, p[i].queue, p[i].arrivalTime, p[i].responseTime, p[i].startTime, p[i].burstTime, p[i].remainingTime, p[i].waitingTime, p[i].completionTime, p[i].turnaroundTime);
        }
    }
    printf("\n");
}


// Input information of the processes
void inputProcessInformation(struct Process *p, int n) {
    for (int i = 0; i < n; i++) {
        // Assign a unique process ID
        p[i].pid = i + 1;

        // Prompt user to enter details for each process
        printf("\nEnter Details of Process[%d]\n", i + 1);
        
        // Input arrival time with validation for non-negativity
        do {
            printf("Arrival Time : ");
            scanf("%f", &p[i].arrivalTime);
            if (p[i].arrivalTime < 0) {
                printf("Error: Arrival time must be a non-negative number.\n");
            }
        } while (p[i].arrivalTime < 0);

        // Input burst time with validation for positivity
        do {
            printf("Burst Time : ");
            scanf("%f", &p[i].burstTime);
            if (p[i].burstTime <= 0) {
                printf("Error: Burst time must be a positive number.\n");
            }
        } while (p[i].burstTime <= 0);

        // Initialize the remaining time to burst time (at first)
        p[i].remainingTime = p[i].burstTime;

        // Initialize start time to -1 (not started)
        p[i].startTime = -1;

        // Initialize completion time to -1 (not completed)
        p[i].completionTime = -1;

        // Set queue type (0 for normal FCFS and RR, 1 and 2 for MLFQ)
        p[i].queue = 0;

        // Initialize waiting time to -1 (not set)
        p[i].waitingTime = -1;

        // Initialize response time to -1 (not set)
        p[i].responseTime = -1;

        // Initialize turnaround time to -1 (not set)
        p[i].turnaroundTime = -1;
    }
}


// Calculate and print average waiting time, turnaround time, response time, completion time, and throughput
void getAverage(struct Process p[], struct Result *r, int n) {
    float avgWaitingTime = 0.0;
    float avgTurnaroundTime = 0.0;
    float avgResponseTime = 0.0;
    float completionTime = p[n - 1].completionTime;
    float throughput = 0.0;

    // Calculate total waiting time, turnaround time, and response time
    for (int i = 0; i < n; i++) {
        avgWaitingTime += p[i].waitingTime;
        avgTurnaroundTime += p[i].turnaroundTime;
        avgResponseTime += p[i].responseTime;
    }

    // Calculate averages
    avgWaitingTime /= n;
    avgTurnaroundTime /= n;
    avgResponseTime /= n;

    // Calculate throughput
    throughput = n / completionTime;

    // Update the Result structure with calculated averages and metrics
    r->avgResponseTime = avgResponseTime;
    r->avgWaitingTime = avgWaitingTime;
    r->avgTurnaroundTime = avgTurnaroundTime;
    r->completionTime = completionTime;
    r->throughput = throughput;

    // Print the calculated averages and metrics
    printf("Average Response Time : %.3f\n", avgTurnaroundTime);
    printf("Average Waiting Time : %.3f\n", avgWaitingTime);
    printf("Average Turnaround Time : %.3f\n", avgTurnaroundTime);
    printf("Completion Time: %.3f\n", completionTime);
    printf("Throughput : %.3f\n", throughput);
    printf("\n");
}


// Function to find the best algorithm based on specified criteria and print the name
void findBestAlgorithm(struct Result results[], int n) {
    // Assume the first algorithm is the best initially
    struct Result bestAlgorithm = results[0];

    // Iterate through the rest of the algorithms to find the best one
    for (int i = 1; i < n; i++) {
        // Compare based on criteria: throughput, avgTurnaroundTime, avgWaitingTime, avgResponseTime
        if (results[i].throughput > bestAlgorithm.throughput ||
            (results[i].throughput == bestAlgorithm.throughput &&
             results[i].avgTurnaroundTime < bestAlgorithm.avgTurnaroundTime) ||
            (results[i].throughput == bestAlgorithm.throughput &&
             results[i].avgTurnaroundTime == bestAlgorithm.avgTurnaroundTime &&
             results[i].avgWaitingTime < bestAlgorithm.avgWaitingTime) ||
            (results[i].throughput == bestAlgorithm.throughput &&
             results[i].avgTurnaroundTime == bestAlgorithm.avgTurnaroundTime &&
             results[i].avgWaitingTime == bestAlgorithm.avgWaitingTime &&
             results[i].avgResponseTime < bestAlgorithm.avgResponseTime)) {
            // Update bestAlgorithm if the current one is better
            bestAlgorithm = results[i];
        }
    }

    // Print the name of the best CPU scheduling algorithm
    printf("\nThe best CPU scheduling algorithm is: %s\n\n", bestAlgorithm.name);
}



// First Come First Serve (FCFS) Scheduling Algorithm
void FCFS(struct Process *p, int n) {
    float currentTime = 0;

    // In the MLFQ, set the current time after the demotion
    if (p[0].queue != 0) {
        for (int i = 0; i < n; i++) {
            // Current time can be calculated as the completion time of the last arrived process
            currentTime = p[i].startTime + p[i].burstTime - p[i].remainingTime;
        }
    }

    for (int i = 0; i < n; i++) {
        // If FCFS in MLFQ and the process is in the first queue, then continue to the next iteration
        if (p[i].queue == 1) continue;

        // If the process hasn't arrived yet, update the current time
        if (p[i].arrivalTime > currentTime) {
            currentTime = p[i].arrivalTime;
        }

        // Set start time if not already set
        p[i].startTime = (p[i].startTime == -1) ? currentTime : p[i].startTime;

        // Calculate response time, completion time, turnaround time, waiting time
        p[i].responseTime = p[i].startTime - p[i].arrivalTime;
        p[i].completionTime = currentTime + p[i].remainingTime;
        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;
        p[i].remainingTime = 0; // All remaining time is used

        currentTime = p[i].completionTime;
    }
}


// Round Robin (RR) Scheduling Algorithm
void RR(struct Process *p, int n, float timeQuantum) {
    float currentTime = p[0].arrivalTime; // current time is set to the arrival time of the first process
    int completed = 0;

    // If all the processes are completed then end the loop
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            // Check if the process has arrived and has remaining time
            if (p[i].arrivalTime <= currentTime && p[i].remainingTime > 0) {

                // Not started yet
                if (p[i].startTime == -1) {
                    p[i].startTime = currentTime;
                    p[i].responseTime = p[i].startTime - p[i].arrivalTime;
                }

                // Calculate the time to execute for this round
                float executeTime = (p[i].remainingTime < timeQuantum) ? p[i].remainingTime : timeQuantum;

                // Update remaining time and current time
                p[i].remainingTime -= executeTime;
                currentTime += executeTime;

                // Check if the process is completed
                if (p[i].remainingTime == 0) {
                    completed++;
                    p[i].completionTime = currentTime;
                    p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
                    p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;
                }
            }
        }

        // Check if it is MLFQ by checking the first process is assigned any queue, then end the loop
        if (p[0].queue != 0) return;
    }
}


// Multi-Level Feedback Queue (MLFQ) Scheduling Algorithm
void MLFQ(struct Process *p, int n, float timeQuantum) {
    // Assign all processes to the first queue (RR queue) initially
    for (int i = 0; i < n; i++) {
        p[i].queue = 1; // First starts in RR queue
    }

    // Apply Round Robin (RR) algorithm for the first queue
    RR(p, n, timeQuantum);

    // Demote processes from RR to FCFS
    for (int i = 0; i < n; i++) {
        if (p[i].remainingTime > 0) {
            p[i].queue = 2; // Demote the process to FCFS
        }
    }

    // Apply First Come First Serve (FCFS) algorithm for the second queue
    FCFS(p, n);
}





