// First Approach
// #include <stdio.h>
// #define MAX 99999
//
// typedef struct {
//     int id;
//     char name;
// } Node;
//
// typedef struct {
//     Node node1;
//     Node node2;
//     int weight;
// } Edge;
//
// int parent[100];
// int rankArr[100];
//
// void makeSet(int V) {
//     for (int i = 0; i < V; i++) {
//         parent[i] = i;
//         rankArr[i] = 0;
//     }
// }
//
// int findSet(int x) {
//     if (parent[x] != x)
//         parent[x] = findSet(parent[x]);
//     return parent[x];
// }
//
// void unionSet(int x, int y) {
//     int rx = findSet(x);
//     int ry = findSet(y);
//
//     if (rx != ry) {
//         if (rankArr[rx] < rankArr[ry])
//             parent[rx] = ry;
//         else if (rankArr[rx] > rankArr[ry])
//             parent[ry] = rx;
//         else {
//             parent[ry] = rx;
//             rankArr[rx]++;
//         }
//     }
// }
//
// void sortEdge(Edge edge[], int eCount) {
//     for (int i = 0; i < eCount - 1; i++) {
//         for (int j = i + 1; j < eCount; j++) {
//             if (edge[j].weight < edge[i].weight) {
//                 Edge temp = edge[i];
//                 edge[i] = edge[j];
//                 edge[j] = temp;
//             }
//         }
//     }
// }
//
// int main() {
//     int V;
//     printf("Enter number of vertices: ");
//     scanf("%d", &V);
//
//     Node node[V];
//     printf("Enter names of vertices: ");
//     for (int i = 0; i < V; i++) {
//         node[i].id = i;
//         scanf(" %c", &node[i].name);
//     }
//
//     int graph[V][V];
//     printf("Enter graph adjacency matrix:\n");
//     for (int i = 0; i < V; i++) {
//         for (int j = 0; j < V; j++) {
//             scanf("%d", &graph[i][j]);
//         }
//     }
//
//     Edge edge[100];
//     int eCount = 0;
//
//     for (int i = 0; i < V; i++) {
//         for (int j = i + 1; j < V; j++) {
//             if (graph[i][j] > 0) {
//                 edge[eCount].node1 = node[i];
//                 edge[eCount].node2 = node[j];
//                 edge[eCount].weight = graph[i][j];
//                 eCount++;
//             }
//         }
//     }
//
//     sortEdge(edge, eCount);
//
//     makeSet(V);
//
//     int minCost = 0;
//
//     printf("\nEdges in MST:\n");
//     for (int i = 0; i < eCount; i++) {
//         int u = edge[i].node1.id;
//         int v = edge[i].node2.id;
//
//         if (findSet(u) != findSet(v)) {
//             printf("%c -- %c  (%d)\n", node[u].name, node[v].name, edge[i].weight);
//             minCost += edge[i].weight;
//             unionSet(u, v);
//         }
//     }
//
//     printf("\nMinimum Cost: %d\n", minCost);
//
//     return 0;
// }



// Second Approach
#include <stdio.h>
#include <stdlib.h>

int comparator(const int p1[], const int p2[]) {
    return p1[2]-p2[2];
}

void makeSet(int parent[],int rank[],int n) {
    for (int i=0;i<n;i++) {
        parent[i]=i;
        rank[i]=0;
    }
}

int findParent(int parent[], int component)
{
    if (parent[component] == component)
        return component;

    return parent[component]
           = findParent(parent, parent[component]);
}

void unionSet(int u, int v, int parent[], int rank[], int n)
{
    u = findParent(parent, u);
    v = findParent(parent, v);

    if (rank[u] < rank[v]) {
        parent[u] = v;
    }
    else if (rank[u] > rank[v]) {
        parent[v] = u;
    }
    else {
        parent[v] = u;

        rank[u]++;
    }
}

int kruskalAlgo(int n, int edge[n][3])
{
    qsort(edge, n, sizeof(edge[0]), comparator);

    int parent[n];
    int rank[n];

    makeSet(parent, rank, n);

    int minCost = 0;
    for (int i = 0; i < n; i++) {
        int v1 = findParent(parent, edge[i][0]);
        int v2 = findParent(parent, edge[i][1]);
        int wt = edge[i][2];

        if (v1 != v2) {
            unionSet(v1, v2, parent, rank, n);
            minCost += wt;
        }
    }

    return minCost;
}
int main() {
    int edge[5][3] = { { 0, 1, 10 },
                       { 0, 2, 6 },
                       { 0, 3, 5 },
                       { 1, 3, 15 },
                       { 2, 3, 4 } };

    printf("%d",kruskalAlgo(5, edge));
}