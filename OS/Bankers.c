#include<stdio.h>
#include<stdbool.h>

int main(){
    int n, m;
    int i, j;
    printf("processss");
    scanf("%d", &n);
    printf("resources");
    scanf("%d", &m);
    
    int allocation[n][m];
    int max[n][m];
    int need[n][m];
    int safe_seq[n];
    int avail[m];
    bool finish[n];
    
    printf("enter allocation:");
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            scanf("%d", &allocation[i][j]);
        }
    }
    
    printf("enter max:");
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            scanf("%d", &max[i][j]);
        }
    }
    
    printf("enter avail:");
    for(i = 0; i < m; i++){
        scanf("%d", &avail[i]);
    }

    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    
    printf("need matrix is:");
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf("%d", need[i][j]);
        }
    }
    
    int count = 0;
    while(count < n){
        bool found = false;
        for(int i = 0; i < n; i++){
            if(finish[i] == false){
                bool allocate = true;
                for(int j = 0; j < m; j++){
                    if(need[i][j] > avail[j]){
                        allocate = false;
                        break;
                    }
                }
                
                if(allocate){
                    for(int k = 0; k < m; k++){
                        avail[k] += allocation[i][k];
                    }
                    safe_seq[count++] = i;
                    finish[i] = true;
                    found = true;
                    printf("process allocation and released: %d\n", i);
                }
            }
        }
        
        if(found == false){
            printf("unsafe\n");
            return 0;
        }
    }
    
    printf("safe seq is:");
    for(int i = 0; i < n; i++){
        printf("%d ", safe_seq[i]);
    }
    printf("\n");

    return 0;
}

/*processss
3
resources
3
enter allocation:
1 2 2
1 0 3
1 3 1
enter max:
3 3 2
1 2 3
1 3 5
enter avail:
2 1 0
need matrix is:
210020004
process allocation and released: 1
process allocation and released: 0
process allocation and released: 2
safe seq is: 1 0 2

gcc bankers_algorithm.c -o bankers_algorithm
./bankers_algorithm

*/