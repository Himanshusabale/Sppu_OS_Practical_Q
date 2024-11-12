 #include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5 // Smaller buffer size for simplicity
#define NUM_ITEMS 10  // Total items to produce and consume

int buffer[BUFFER_SIZE];
sem_t full, empty, mutex;
int in = 0, out = 0;

void *producer(void *arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        int item = rand() % 100; // Produce a random item
        sem_wait(&empty); // Wait for an empty slot
        sem_wait(&mutex); // Enter critical section

        buffer[in] = item; // Add item to buffer
        printf("Produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE; // Wrap around

        sem_post(&mutex); // Exit critical section
        sem_post(&full); // Signal that a new item is available
        sleep(1); // Simulate production time
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&full); // Wait for a full slot
        sem_wait(&mutex); // Enter critical section

        int item = buffer[out]; // Remove item from buffer
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE; // Wrap around

        sem_post(&mutex); // Exit critical section
        sem_post(&empty); // Signal that an empty slot is available
        sleep(2); // Simulate consumption time
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&full, 0, 0);  // No items in the buffer
    sem_init(&empty, 0, BUFFER_SIZE); // All slots are empty
    sem_init(&mutex, 0, 1);  // Mutex for critical section

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&mutex);

    return 0;
}

// gcc -o producer_consumer producer_consumer.c -lpthread
// ./producer_consumer
// sudo apt-get install pthreads


  Study of Basic Linux Commands: echo, ls, read, cat, touch, test, loops, arithmetic 
comparison, conditional loops, grep, sed etc. 
B. Write a program to implement an address book with options given below: a) Create 
address book. b) View address book. c) Insert a record. d) Delete a record. e) Modify a record. 
f) Exit 
Assignment No. 2: 
 
Process control system calls: The demonstration of FORK, EXECVE and WAITsystem calls along 
with zombie and orphan states. 
A. Implement the C program in which main program accepts the integers to be sorted. Main 
program uses the FORK system call to create a new process called a child process. Parent process 
sorts the integers using sorting algorithm and waits for child process using WAIT system call to sort 
the integers using any sorting algorithm. Also demonstrate zombie and orphan states. 
B. Implement the C program in which main program accepts an array. Main program uses the 
FORK system call to create a new process called a child process. Parent process sorts an array and 
passes the sorted array to child process through the command line arguments of EXECVE system 
call. The child process uses EXECVE system call to load new program which display array in reverse 
order. 
Assignment No. 3: 
Implement the C program for CPU Scheduling Algorithms: Shortest Job First 
(Preemptive) and Round Robin with different arrival time. 
Assignment No. 4: 
A. Thread synchronization using counting semaphores. Application to demonstrate: producer− 
consumer problem with counting semaphores and mutex. 
B. Thread synchronization and mutual exclusion using mutex. Application to demonstrate: Reader− 
Writer problem with reader priority. 
Assignment No. 5: 
Implement the C program for Deadlock Avoidance Algorithm: Bankers Algorithm. 
Assignment No. 6: 
Implement the C program for Page Replacement Algorithms: FCFS, LRU, and Optimal for frame size as 
minimum three. 
Assignment No. 7: 
Inter process communication in Linux using following. 
Assignment No. 8: Implement the C program for Disk Scheduling Algorithms: SSTF, SCAN, C−Look 
considering the initial head position moving away from the spindle