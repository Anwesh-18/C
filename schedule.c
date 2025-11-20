#include <stdio.h>
#include <stdlib.h>

// Designing the structure of Process 
typedef struct process{
    int pid;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int start;
    int completion;
    int waiting;
    int turnaround;
}P;
P p[100];
typedef struct GanttChart{
    int pid;
    int start;
    int end;
}G;
G gc[100];
int gc_index = 0;
void printGantt(G gc[],int n){
    for(int i=0;i<n;i++){
        printf("| P%d (%d - %d) ", gc[i].pid, gc[i].start, gc[i].end);
    }
    printf("|\n");
}
void printDetails(P p[],int n){
    int time=0;
    int wt = 0;
    int tat = 0;
    for(int i=0;i<n;i++){
        // Assigning pid
        gc[i].pid = p[i].pid;
        // calculating times
        // If CPU is idle, jump time
        if (time < p[i].arrival)
            time = p[i].arrival;
        p[i].start = time;
        time += p[i].burst;
        p[i].completion = time;
        p[i].waiting = p[i].start - p[i].arrival;
        p[i].turnaround = p[i].completion - p[i].arrival;
        gc[i].pid = p[i].pid;
        gc[i].start = p[i].start;
        gc[i].end = p[i].completion;
        wt += p[i].waiting;
        tat += p[i].turnaround;
    }
    printGantt(gc,n);
    printf("The average waiting time: %f\n",(float)wt/n);
    printf("The average turn around time: %f\n",(float)tat/n);
}
void FCFS(P p[],int n){
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            if (p[i].arrival > p[j].arrival){
                P temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
    printDetails(p,n);
}
void main(){
    int n;
    printf("Enter the number of processes: ");
    scanf("%d",&n);
    printf("-----Enter the process details-----\n");
    for(int i=0;i<n;i++){
        printf("Enter the process id: ");
        scanf("%d",&p[i].pid);
        printf("Enter the ptocess arrival time: ");
        scanf("%d",&p[i].arrival);
        printf("Enter the ptocess burst time: ");
        scanf("%d",&p[i].burst);
        printf("Enter the process's priority: ");
        scanf("%d",&p[i].priority);
    }
    int choice = 1;
    while(choice){
        printf("-----Operations-----\n");
        printf("1.FCFS\n2.SJF\n3.SRTF\n4.PRIORITY\n5.RR\n");
        printf("Enter your choice: ");
        int ch;
        scanf("%d",&ch);
        switch(ch){
            case 1: FCFS(p,n);
                    break;
                // case 2: SJF(p,n);
                //         break;
                // case 3: SRTF(p,n);
                //         break;
                // case 4: PRIORITY(p,n);
                //         break;
                // case 5: RR(p,n);
                //         break;
            default: printf("Invalid Input!!\n");
        }
        printf("Want to continue?(0,1): ");
        scanf("%d",&choice);
    }
}