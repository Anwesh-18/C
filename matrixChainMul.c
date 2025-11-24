#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void mulMatrix(int arr[],int n){
    int N = n-1;
    int dp[N+1][N+1];

    for(int i=0;i<n;i++){
        dp[i][i] = 0;
    }

    for(int L = 2;L<=N;L++){
        for(int i=1;i<=N-L+1;i++){
            int j = i + L -1;
            dp[i][j] = INT_MAX;
            for(int k=i;k<j;k++){
                int cost = dp[i][k] + dp[k+1][j] + (arr[i-1]*arr[k]*arr[j]);
                if(cost < dp[i][j]){
                    dp[i][j] = cost;
                }
            }
        }
    }
    printf("DP TABLE (i,j from 1 to %d):\n", N);
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (j < i) {
                printf("   -   ");
            } else {
                if (dp[i][j] == INT_MAX)
                    printf(" INF  ");
                else
                    printf("%5d ", dp[i][j]);
            }
        }
        printf("\n");
    }
}
void main(){
    int r,c,n;
    printf("Enter the array size: ");
    scanf("%d",&n);
    int myArr[n];
    printf("Enter the array: ");
    for(int i=0;i<n;i++){
        scanf("%d",&myArr[i]);
    }
    mulMatrix(myArr,n);
}