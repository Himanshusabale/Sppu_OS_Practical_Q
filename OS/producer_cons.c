#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define BUFFER_SIZE 5
#define NUM_SIZE 10

int in = 0 , out = 0 , buffer[BUFFER_SIZE];
sem_t empty;
sem_t full;
sem_t mutex;

void* producer(void* arg){
    int item;
    int prod_items = 0;
    while(prod_items < NUM_SIZE){
        item = rand() % 100;
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[in]=item;
        printf("\nProducer produced an item %d",item);
        in = (in+1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&full);
        prod_items++;
        sleep(1);
    }
    pthread_exit(NULL);
}
void* consumer(void* arg){
    int item;
    int cons_items = 0;
    while(cons_items < NUM_SIZE){
        //item = rand() % 100;
        sem_wait(&full);
        sem_wait(&mutex);
        item = buffer[out];
        printf("\nConsumer consumed an item %d",item);
        out = (out+1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&empty);
        cons_items++;
        sleep(2);
    }
    pthread_exit(NULL);
    
}

int main(){
    pthread_t prod , cons ;
    
    sem_init(&empty , 0 , BUFFER_SIZE);
    sem_init(&full , 0 , 0);
    sem_init(&mutex , 0 , 1);
    
    pthread_create(&prod , NULL , producer , NULL);
    pthread_create(&cons , NULL , consumer , NULL);
    
    pthread_join(prod , NULL);
    pthread_join(cons , NULL);
    
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    
    return 0;

}






/*#include<stdio.h>
int alloc[10][10] , need[10][10] , maxreq[10][10] , avail[10];
int np , nr;


void readmatrix(int matrix[10][10]){
    for(int i=0 ; i<np ;i++){
        for(int j = 0 ;j<nr;j++){
            scanf("%d",&matrix[i][j]);
        }
    }
    
}

void displaymatrix(int matrix[10][10]){
    for(int i=0 ; i<np ;i++){
        printf("\nP%d:",i);
        for(int j=0 ; j<nr ;j++){
            printf(" %d ",matrix[i][j]);
        }
    }
    
}
void calculate(int matrix[10][10]){
    for(int i=0 ; i<np ; i++){
        for(int j=0 ; j<nr ; j++){
            need[i][j] = maxreq[i][j] - alloc[i][j];
        }
    }
}

void bankers(){
    int finish[10] , safe[10] , k=0 , flag;
    for(int i = 0 ; i<np ;i++){
        finish[i] = 0;
    }
    int i = 0;
    while( i<np ){
        if(finish[i] == 0){
            flag = 0;
            for(int j = 0;j<nr;j++){
                if(need[i][j] > avail[j]){
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                finish[i] = 1;
                safe[k++] = i;
                for(int j = 0;j<nr;j++){
                    avail[j] += alloc[i][j];
                }
                i = (i+1)%np;
                continue;
            }
            
        }
        i++;
    }
    flag = 0;
    for(int i =0;i<np ; i++){
        if(finish[i] == 0){
            printf("\nNot\n");
            flag = 1;
            break;
        }
    }
    
    if(flag == 0){
        printf("\n---------------Safe Sequence------------\n");
        for(int i =0;i<np;i++){
            printf("->P%d",safe[i]);
        }
    }
    
}

int main(){
    
    printf("Enter the total number of Process :");
    scanf("%d",&np);
    printf("Enter the total number of resources :");
    scanf("%d",&nr);
    printf("Enter the allocation matrix :");
    readmatrix(alloc);
    printf("Enter the maxreq matrix:");
    readmatrix(maxreq);
    printf("enetr the Available matrix:");
    for(int i =0; i<nr ; i++){
        scanf("%d",&avail[i]);
    }
    
    printf("\nAllocation Matrix:");
    displaymatrix(alloc);
    printf("\nMax Request Matrix:");
    displaymatrix(maxreq);
    calculate(need);
    displaymatrix(need);
    bankers();
    return 0;
    
    
}*/


// gcc -o producer_consumer producer_consumer.c -lpthread
// ./producer_consumer
// sudo apt-get install pthreads