// My Approach
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 100

typedef struct Node {
    int id;
    char name;
    int key;
    int PI;
} Node;

Node heap[MAX];
int heapSize = 0;
int positionInHeap[MAX];

void swap(int a, int b) {
    Node temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;

    positionInHeap[heap[a].id] = a;
    positionInHeap[heap[b].id] = b;
}

void minHeapify(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int smallest = i;

    if (left < heapSize && heap[left].key < heap[smallest].key)
        smallest = left;
    if (right < heapSize && heap[right].key < heap[smallest].key)
        smallest = right;

    if (smallest != i) {
        swap(i, smallest);
        minHeapify(smallest);
    }
}

void buildMinHeap() {
    for (int i = (heapSize / 2) - 1; i >= 0; i--) {
        minHeapify(i);
    }
}

Node popMinHeap() {
    if (heapSize == 0) {
        Node dummy = {-1, ' ', INT_MAX, -1};
        return dummy;
    }

    Node temp = heap[0];
    positionInHeap[temp.id] = -1;

    heapSize--;
    if (heapSize > 0) {
        heap[0] = heap[heapSize];
        positionInHeap[heap[0].id] = 0;
        minHeapify(0);
    }
    return temp;
}

void decreaseKey(int vertexId, int newKey) {
    int i = positionInHeap[vertexId];
    if (i == -1) return;

    heap[i].key = newKey;

    while (i > 0 && heap[(i - 1) / 2].key > heap[i].key) {
        swap(i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

void calShortestPath(int graph[MAX][MAX], int source, int V, Node node[]) {
    bool visited[MAX] = {false};

    for (int i = 0; i < MAX; i++) {
        positionInHeap[i] = -1;
    }

    heapSize = V;
    for (int i = 0; i < V; i++) {
        node[i].key = INT_MAX;
        node[i].PI = -1;
        heap[i] = node[i];
        positionInHeap[node[i].id] = i;
    }

    node[source].key = 0;
    heap[positionInHeap[source]].key = 0;

    buildMinHeap();

    while (heapSize > 0) {
        Node u = popMinHeap();
        if (u.id == -1) break;
        visited[u.id] = true;

        for (int i = 0; i < V; i++) {
            if (graph[u.id][i] != 0 && !visited[i]) {
                int newDist = u.key + graph[u.id][i];
                if (newDist < node[i].key) {
                    node[i].key = newDist;
                    node[i].PI = u.id;
                    decreaseKey(i, newDist);
                }
            }
        }
    }

    printf("The shortest path from node %c (id %d):\n", node[source].name, source);
    for (int i = 0; i < V; i++) {
        if (node[i].key == INT_MAX)
            printf("Node %c -> dist = INF\n", node[i].name);
        else
            printf("Node %c -> dist = %d", node[i].name, node[i].key);

        if (node[i].PI != -1)
            printf(" (from %c)", node[node[i].PI].name);
        printf("\n");
    }
    printf("\n");
}

int main() {
    int V;
    printf("Enter the number of vertices: ");
    scanf("%d", &V);

    if (V <= 0 || V > MAX) {
        printf("Invalid number of vertices!\n");
        return 1;
    }

    Node node[MAX];

    printf("Enter node names (A-Z, single chars):\n");
    for (int i = 0; i < V; i++) {
        printf("Node %d: ", i);
        scanf(" %c", &node[i].name);
        node[i].id = i;
        node[i].key = INT_MAX;
        node[i].PI = -1;
    }

    int graph[MAX][MAX];
    printf("Enter the adjacency matrix (0 for no edge):\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    int src;
    printf("Enter source vertex ID (0 to %d): ", V - 1);
    scanf("%d", &src);

    if (src < 0 || src >= V) {
        printf("Invalid source!\n");
        return 1;
    }

    calShortestPath(graph, src, V, node);

    return 0;
}