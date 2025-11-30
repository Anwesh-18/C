#include <stdio.h>
#include <stdlib.h>
// #include <stdlib.h>
// #define MAX 100

typedef struct {
    int val;
    int weight;
    float frac;
}Bag;
Bag* bag = NULL;
int size = 0;
void sortFunc() {
    for (int i=0;i<size-1;i++) {
        for (int j=i+1;j<size;j++) {
            if (bag[i].frac < bag[j].frac) {
                Bag temp = bag[i];
                bag[i] = bag[j];
                bag[j] = temp;
            }
        }
    }
}
float knapSack(int W) {
    for (int i=0;i<size;i++) {
        bag[i].frac = (float)bag[i].val/bag[i].weight;
    }
    sortFunc();
    float ans = 0.0;
    int remain = W;
    for (int i=0;i<size;i++) {
        if (remain>0) {
            if (bag[i].weight <= remain) {
                ans += bag[i].val;
                remain -= bag[i].weight;
            }else {
                ans += bag[i].frac * remain;
                remain = 0;
            }
        }else {
            break;
        }
    }
    return ans;
}
int main() {
    int N,W;
    printf("Enter the total number of items: ");
    scanf("%d",&N);
    bag = (Bag*)malloc(sizeof(Bag)*N);
    size = N;
    printf("Enter the item values:\n");
    for (int i=0;i<N;i++) {
        scanf("%d",&bag[i].val);
    }
    printf("Enter the item weights:\n");
    for (int i=0;i<N;i++) {
        scanf("%d",&bag[i].weight);
    }
    printf("Enter the capacity of knapsack: ");
    scanf("%d",&W);
    float ans = knapSack(W);
    printf("Answer: %.4f\n",ans);
    return 0;
}