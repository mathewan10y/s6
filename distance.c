#include <stdio.h>
#define MAX_NODES 10
#define INF 999
int main() {
    int n;
    int cost[MAX_NODES][MAX_NODES];
    int dist[MAX_NODES][MAX_NODES];
    int nextHop[MAX_NODES][MAX_NODES];
    printf("Enter number of routers: ");
    scanf("%d", &n);
    printf("Enter cost matrix (999 for no direct link):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d -> %d :",i+1,j+1);
            scanf("%d", &cost[i][j]);
            dist[i][j] = cost[i][j];
            if (i == j) 
                nextHop[i][j] = i;
            else if (cost[i][j] != INF)
                nextHop[i][j] = j;
            else
                nextHop[i][j] = -1;
        }
    }
    int updated;
    do {
        updated = 0;
        for (int i = 0; i < n; i++) {           
            for (int j = 0; j < n; j++) {       
                for (int k = 0; k < n; k++) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        nextHop[i][j] = nextHop[i][k];
                        updated = 1;
                    }
                }
            }
        }
    } while (updated);
    for (int i = 0; i < n; i++) {
        printf("\nRouting table for router %d:\n", i);
        printf("Destination\tNext Hop\tCost\n");
        for (int j =0; j < n; j++) {
            printf("%d\t\t", j+1);
            if (nextHop[i][j] == -1)
                printf("-\t\t");
            else
                printf("%d\t\t", nextHop[i][j]+1);
            printf("%d\n", dist[i][j]);
        }
    }
    return 0;
}

