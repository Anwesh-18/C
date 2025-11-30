#include <stdio.h>
#include <stdlib.h>

// Designing the structure of Process 
typedef struct{
    int pid;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int start;
    int completion;
    int waiting;
    int turnaround;
    int index;
}Process;
Process* p = NULL;
int n = 0;

typedef struct{
    int pid;
    int start;
    int end;
}Gantt;
Gantt *gc = NULL;
int gc_size = 0;

typedef struct {
    int pid;
    int arrival;
    int remaining;
    int burst;
    int priority;
    int index;
    int key;
}HeapNode;
HeapNode *heap = NULL;
int heapSize = 0;

void printGantt() {
    for(int i=0;i<gc_size;i++){
        if (gc[i].pid == -1) {
            printf("| %s (%d - %d) ", "IDLE", gc[i].start, gc[i].end);
        }else {
            printf("| P%d (%d - %d) ", gc[i].pid, gc[i].start, gc[i].end);
        }
    }
    printf("|\n");
}
void addGantt(int pid,int start,int end) {
    gc = realloc(gc,(gc_size+1)*sizeof(Gantt));

    gc[gc_size].pid = pid;
    gc[gc_size].start = start;
    gc[gc_size].end = end;

    gc_size++;
}
void sortByArrivalTime() {
    for (int i=0;i<n-1;i++) {
        for (int j=i+1;j<n;j++) {
            if (p[i].arrival > p[j].arrival) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

void swap(HeapNode* a,HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}
void heapify(int i) {
    int left = 2*i+1;
    int right = 2*i+2;
    int smallest = i;

    if (left<heapSize && heap[smallest].key > heap[left].key) {
        smallest = left;
    }
    if (right<heapSize && heap[smallest].key > heap[right].key) {
        smallest = right;
    }
    if (smallest != i) {
        swap(&heap[i],&heap[smallest]);
        heapify(smallest);
    }
}
void heapPush(HeapNode node) {
    heap = realloc(heap,(heapSize+1)*sizeof(HeapNode));
    heap[heapSize] = node;
    int i = heapSize;
    heapSize++;

    while (i>0 && heap[i].key < heap[(i-1)/2].key) {
        swap(&heap[i],&heap[(i-1)/2]);
        i = (i-1)/2;
    }
}
HeapNode heapPop() {
    HeapNode res = heap[0];
    heap[0] = heap[heapSize-1];
    heapSize--;

    heapify(0);
    return res;
}
// FCFS SCHEDULING
void FCFS(){
    gc_size = 0;
    sortByArrivalTime();
    int time = 0;
    for (int i=0;i<n;i++) {
        if (p[i].arrival > time) {
            addGantt(-1,time,p[i].arrival);
            time = p[i].arrival;
        }
        p[i].start = time;
        p[i].waiting = p[i].start - p[i].arrival;
        time += p[i].burst;
        p[i].completion = time;
        p[i].turnaround = p[i].completion - p[i].arrival;
        addGantt(p[i].pid,p[i].start,p[i].completion);
    }
    printGantt();
}
// SJF SCHEDULING
void SJF() {
    gc_size = 0;
    sortByArrivalTime();
    int time = 0;
    int completed = 0;
    int i = 0;
    while (completed < n) {
        while (i<n && p[i].arrival <= time) {
            HeapNode node;
            node.pid = p[i].pid;
            node.arrival = p[i].arrival;
            node.remaining = p[i].remaining;
            node.burst = p[i].burst;
            node.priority = p[i].priority;
            node.index = p[i].index;

            node.key = node.burst;
            heapPush(node);
            i++;
        }
        if (heapSize == 0) {
            addGantt(-1,time,p[i].arrival);
            time = p[i].arrival;
            continue;
        }
        HeapNode h = heapPop();
        Process curr = p[h.index];
        curr.start = time;
        time += curr.burst;
        curr.completion = time;
        curr.turnaround = curr.completion - curr.arrival;
        curr.waiting = curr.start - curr.arrival;
        p[curr.index] = curr;
        addGantt(curr.pid,curr.start,curr.completion);
        completed ++;
    }
    printGantt();
}
// SRTF SCHEDULING
void SRTF() {
    gc_size = 0;
    sortByArrivalTime();
    heapSize = 0;
    free(heap);
    heap = NULL;

    int time = 0;
    int completed = 0;
    int i = 0;

    int prev_pid = -1;
    int blocktStart = 0;

    for (int i=0;i<n;i++) {
        p[i].start = -1;
    }

    while (completed < n) {
        while (i<n && p[i].arrival <= time) {
            HeapNode node;
            node.pid = p[i].pid;
            node.arrival = p[i].arrival;
            node.remaining = p[i].remaining;
            node.burst = p[i].burst;
            node.priority = p[i].priority;
            node.index = p[i].index;

            node.key = p[i].remaining;
            heapPush(node);
            i++;
        }
        if (heapSize == 0) {
            if (prev_pid != -1) {
                addGantt(prev_pid,blocktStart,time);
            }
            prev_pid = -1;
            time ++;
            continue;
        }
        HeapNode h = heapPop();
        Process curr = p[h.index];

        if (curr.pid != prev_pid) {
            if (prev_pid != -1) {
                addGantt(prev_pid,blocktStart,time);
            }
            blocktStart = time;
        }
        curr.remaining --;
        time ++;
        if (curr.remaining == 0) {
            curr.completion = time;
            curr.turnaround = curr.completion - curr.arrival;
            curr.waiting = curr.turnaround - curr.burst;
            p[curr.index] = curr;
            addGantt(curr.pid,blocktStart,time);
            completed ++;
            prev_pid = -1;
        }else {
            p[curr.index] = curr;
            h.remaining  = curr.remaining;
            h.key = curr.remaining;
            heapPush(h);

            prev_pid = curr.pid;
        }
    }
    printGantt();
}
// PRIORITY SCHEDULING
void PRIORITY_P() {
    gc_size = 0;
    sortByArrivalTime();
    heapSize = 0;
    free(heap);
    heap = NULL;
    int time = 0;

    int completed = 0;
    int i = 0;

    int prev_pid = -1;
    int blockStart = 0;

    for (int i=0;i<n;i++) {
        p[i].start = -1;
    }

    while (completed < n) {
        while (i < n && p[i].arrival <= time) {
            HeapNode node;
            node.pid = p[i].pid;
            node.arrival = p[i].arrival;
            node.remaining = p[i].remaining;
            node.burst = p[i].burst;
            node.priority = p[i].priority;
            node.index = p[i].index;

            node.key = p[i].priority;
            heapPush(node);
            i++;
        }
        if (heapSize == 0) {
            if (prev_pid != -1) {
                addGantt(prev_pid, blockStart, time);
            }
            prev_pid = -1;
            time++;
            continue;
        }
        HeapNode h = heapPop();
        Process curr = p[h.index];

        if (curr.start == -1)
            curr.start = time;

        if (curr.pid != prev_pid) {
            if (prev_pid != -1) {
                addGantt(prev_pid, blockStart, time);
            }
            blockStart = time;
        }
        time++;
        curr.remaining --;
        if (curr.remaining == 0) {
            curr.completion = time;
            curr.turnaround = curr.completion - curr.arrival;
            curr.waiting = curr.turnaround - curr.burst;

            p[curr.index] = curr;
            addGantt(curr.pid, blockStart, time);

            prev_pid = -1;
            completed++;
        }else {
            p[curr.index] = curr;
            h.remaining  = curr.remaining;
            h.key = curr.priority;
            heapPush(h);
            prev_pid = curr.pid;
        }
    }
    printGantt();
}
void PRIORITY_N() {
    gc_size = 0;
    sortByArrivalTime();
    heapSize = 0;
    free(heap);
    heap = NULL;
    int time = 0;
    int completed = 0;
    int i = 0;

    while (completed < n){
        while (i < n && p[i].arrival <= time) {
            HeapNode node;
            node.pid = p[i].pid;
            node.arrival = p[i].arrival;
            node.remaining = p[i].remaining;
            node.burst = p[i].burst;
            node.priority = p[i].priority;
            node.index = p[i].index;

            node.key = p[i].priority;
            heapPush(node);
            i++;
        }

        if (heapSize == 0) {
            addGantt(-1,time,p[i].arrival);
            time = p[i].arrival;
            continue;
        }
        HeapNode h = heapPop();
        Process curr = p[h.index];
        curr.start = time;
        time += curr.burst;
        curr.completion = time;
        curr.turnaround = curr.completion - curr.arrival;
        curr.waiting = curr.turnaround- curr.burst;
        p[curr.index] = curr;
        addGantt(curr.pid,curr.start,curr.completion);
        completed ++;
    }
    printGantt();
}
int *queue = NULL;
int q_front = 0, q_rear = 0, q_size = 0;

void initQueue() {
    free(queue);
    queue = NULL;
    q_front = q_rear = q_size = 0;
}

int isQueueEmpty() {
    return q_size == 0;
}

void pushQueue(int idx) {
    queue = realloc(queue, (q_size + 1) * sizeof(int));
    queue[q_rear] = idx;
    q_rear++;
    q_size++;
}
int popQueue() {
    int idx = queue[q_front];
    q_front++;
    q_size--;

    if (q_front == q_rear) {
        free(queue);
        queue = NULL;
        q_front = q_rear = q_size = 0;
    }
    return idx;
}
// RR Scheduling
void RR(int tc) {
    // free(queue);
    initQueue();
    gc_size = 0;
    sortByArrivalTime();

    for (int k = 0; k < n; k++) {
        p[k].remaining = p[k].burst;
    }

    int time = 0;
    int blockStart = 0;
    int completed = 0;
    int i = 0;
    int prev_pid = -1;

    while (completed < n) {
        while (i<n && p[i].arrival <= time) {
            pushQueue(p[i].index);
            i++;
        }
        if (isQueueEmpty()) {
            if (i<n) {
                addGantt(-1,time,p[i].arrival);
                time = p[i].arrival;
                continue;
            }else {
                break;
            }
        }

        int idx = popQueue();
        Process curr = p[idx];

        if (curr.pid != prev_pid) {
            if (prev_pid != -1) {
                addGantt(prev_pid,blockStart,time);
            }
            blockStart = time;
        }
        int runTime = (curr.remaining<tc) ? curr.remaining : tc;
        for (int t = 0; t < runTime; t++) {
            time++;
            curr.remaining--;

            while (i < n && p[i].arrival <= time) {
                pushQueue(p[i].index);
                i++;
            }

            if (curr.remaining == 0)
                break;
        }
        if (curr.remaining == 0) {
            curr.completion = time;
            curr.turnaround = curr.completion - curr.arrival;
            curr.waiting = curr.turnaround - curr.burst;

            addGantt(curr.pid, blockStart, time);
            prev_pid = -1;
            completed++;
        }
        else {
            // not completed → requeue
            pushQueue(curr.index);
            prev_pid = curr.pid;
        }
        p[curr.index] = curr;
    }
    printGantt();
}

int main(){
    printf("Enter the number of processes: ");
    scanf("%d",&n);

    p = calloc(n,sizeof(Process));

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
        p[i].index = i;
        p[i].remaining = p[i].burst;
    }
    int choice = 1;
    while(choice){
        printf("-----Operations-----\n");
        printf("1.FCFS\n2.SJF\n3.SRTF\n4.PRIORITY\n5.RR\n");
        printf("Enter your choice: ");
        int ch,x;
        scanf("%d",&ch);
        switch(ch){
            case 1: FCFS();
                    break;
            case 2: SJF();
                    break;
            case 3: SRTF();
                    break;
            case 4: printf("Which one:\n1.Preemptive\n2.Non-preemptive\nEnter your choice: ");
                    scanf("%d",&x);
                    switch(x) {
                        case 1: PRIORITY_P();
                                break;
                        case 2: PRIORITY_N();
                                break;
                        default: printf("Wrong choice!! Try again\n");
                    }
                    break;
            case 5: printf("Enter the timeQuantum: ");
                    scanf("%d",&x);
                    RR(x);
                    break;
            default: printf("Invalid Input!!\n");
        }
        printf("Want to continue?(0,1): ");
        scanf("%d",&choice);
    }
    printf("Thank You!!\n");
    free(p);
    free(gc);
    return 0;
}