#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 100 // Defining a maximum length to set value limits

// Logic for Queue used in BFS
int Queue[MAX];
int front = -1;
int sizeQ = 0;
int rear = -1;
void pushQ(int x){
    if(front==-1){
        front = rear = 0;
        Queue[front] = x;
        sizeQ++;
        return;
    }
    else if(rear==MAX-1){
        printf("Queue limit exceeded!!\n");
    }else{
        Queue[++rear] = x;
        sizeQ++;
    }
}
int popQ(){
    if(front ==rear== -1){
        printf("Queue is empty!!\n");
        return 0;
    }
    sizeQ--;
    return Queue[front++];
}
// Logic for stack used in DFS
int Stack[MAX];
int top = -1;
int sizeS = 0;
void pushS(int x){
    if(top==MAX-1){
        printf("Stack is full!!\n");
        return;
    }
    Stack[++top] = x;
    sizeS++;
}
int popS(){
    if(top == -1) {
        printf("Stack is empty!!\n");
        return 0;
    }
    sizeS--;
    return Stack[top--];
}
// Structure for adjacency list node
struct edgeList{
    int val;
    struct edgeList* next;
};
// Initializing the list
struct edgeList adjList[MAX];
// initializing evry node with NULL
void initList(int V) {
    for (int i = 0; i < V; i++) {
        adjList[i].next = NULL;
    }
}
// Making of the List
void makeList(int V,int edges[MAX][MAX],int n){
    for(int i=0;i<n;i++){
        struct edgeList* newNode=(struct edgeList*)malloc(sizeof(struct edgeList));
        newNode->val=edges[i][1];
        newNode->next=adjList[edges[i][0]].next;
        adjList[edges[i][0]].next=newNode;
        struct edgeList* newNode1=(struct edgeList*)malloc(sizeof(struct edgeList));
        newNode1->val=edges[i][0];
        newNode1->next=adjList[edges[i][1]].next;
        adjList[edges[i][1]].next=newNode1;

    }
    printf("Adjacency list created successfully!!\n");
}
// Printing the List
void printList(int V){
    printf("The adjacency list is:\n");
    for(int i=0;i<V;i++){
        struct edgeList* temp=adjList[i].next;
        printf("%d: ",i);
        while(temp!=NULL){
            printf("%d -> ",temp->val);
            temp=temp->next;
        }
        printf("NULL\n");
    }
}
// initializing the matrix
int adjMat[MAX][MAX];
// making of the matrix
void makeMat(int V,int edges[MAX][MAX],int n){
    for(int i=0;i<V;i++){
        for(int j=0;j<V;j++){
            adjMat[i][j] = 0;
        }
    }
    for(int i=0;i<n;i++){
        adjMat[edges[i][0]][edges[i][1]] = 1;
    }
    printf("Adjacency Matrix created Successsfully!!\n");
}
// printing the matrix
void printMat(int V){
    printf("The Adjacency Matrix is:\n");
    for(int i=0;i<V;i++){
        for(int j=0;j<V;j++){
            printf("%d ",adjMat[i][j]);
        }
        printf("\n");
    }
}
bool isPresent(int x,int seen[],int n){
    for(int i=0;i<n;i++){
        if (seen[i]==x){
            return true;
        }
    }
    return false;
}
void BFS(int V, struct edgeList adjList[MAX], int n) {
    pushQ(0);
    int seen[MAX] = {0};
    int seenCount = 0;
    seen[seenCount++] = 0;
    int i = 1;
    while (sizeQ != 0) {
        int v = popQ();
        printf("visit_%d: %d\n", i++, v);
        struct edgeList* temp = adjList[v].next;
        while (temp != NULL) {
            if (!isPresent(temp->val, seen, seenCount)) {
                pushQ(temp->val);
                seen[seenCount++] = temp->val;
            }
            temp = temp->next;
        }
    }
}
void DFS(int V, struct edgeList adjList[MAX], int n) {
    pushS(0);
    int seen[MAX] = {0};
    int seenCount = 0;
    seen[seenCount++] = 0;
    int i = 1;
    while (sizeS != 0) {
        int v = popS();
        printf("visit_%d: %d\n", i++, v);
        struct edgeList* temp = adjList[v].next;
        while (temp != NULL) {
            if (!isPresent(temp->val, seen, seenCount)) {
                pushS(temp->val);
                seen[seenCount++] = temp->val;
            }
            temp = temp->next;
        }
    }
}
void main() {
    int V,edges[MAX][MAX],n;
    printf("Enter the number of vertices: ");
    scanf("%d",&V);
    printf("Enter the number of edges: ");
    scanf("%d",&n);
    printf("Enter the edges in pairs:\n");
    for(int i=0;i<n;i++){
        printf("Enter the pair: ");
        scanf("%d%d",&edges[i][0],&edges[i][1]);
    }
    // makeList(V,edges);
    int choice=1;
    while(choice){
        printf("1.Make List\n2.Print List\n3.Make Matrix\n4.Print Matrix\n5.search Matrix\nEnter your choice: ");
        int ch;
        scanf("%d",&ch);
        switch(ch){
            case 1:
                makeList(V,edges,n);
                break;
            case 2:
                printList(V);
                break;
            case 3:
                makeMat(V,edges,n);
                break;
            case 4:
                printMat(V);
                break;
            case 5:
                printf("1.BFS\n2.DFS\nChoose the type:");
                int c;
                scanf("%d",&c);
                switch(c){
                    case 1: 
                        BFS(V,adjList,n);
                        break;
                    case 2: 
                        DFS(V,adjList,n);
                        break;
                    default: 
                        printf("Invalid input!!Try again!!\n");
                }
                break;
            default: printf("Invalid choice\n");
        }
        printf("Do you want to continue? (1/0): ");
        scanf("%d",&choice);
    }
}