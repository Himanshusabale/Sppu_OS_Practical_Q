#include <stdio.h>
#include <stdlib.h>

//replace the page which is loaded first
//if all pages are used in future, replace the page which is used first
void fifo(int pages[], int n, int capacity) {
    int frame[capacity], index = 0, faults = 0;
    for (int i = 0; i < capacity; i++) frame[i] = -1;    //@Adwait-Borate 

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == pages[i]) {                  //@Adwait-Borate 
                found = 1;
                break;
            }
        }  
        if (!found) { 
            frame[index] = pages[i];
            index = (index + 1) % capacity;             //@Adwait-Borate  
            faults++;
        }
        printf("Frame: ");
        for (int j = 0; j < capacity; j++) printf("%d ", &frame[j]);
        printf("\n");
    }
    printf("Total Page Faults (FIFO): %d\n", faults);
}


//replace the page which is not used in longest dimension of time in past
//Go to the past and check which page is not used for longest time
void lru(int pages[], int n, int capacity) {
    int frame[capacity], time[capacity], faults = 0, counter = 0;
    for (int i = 0; i < capacity; i++) frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0, least = 0;
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == pages[i]) {
                found = 1;
                time[j] = ++counter;                                                   //@Adwait-Borate
                break;
            }
        }
        if (!found) {
            for (int j = 1; j < capacity; j++) {
                if (time[j] < time[least]) least = j;                                   //@Adwait-Borate
            }
            frame[least] = pages[i];
            time[least] = ++counter;                                                    //Adwait-Borate
            faults++;
        }
        printf("Frame: ");
        for (int j = 0; j < capacity; j++) printf("%d ", frame[j]);
        printf("\n");
    }
    printf("Total Page Faults (LRU): %d\n", faults);
}


//replace the page which is not used in longest dimension of time in future
//if all pages are used in future, replace the page which is used in longest dimension of time in future
void optimal(int pages[], int n, int capacity) {
    int frame[capacity], faults = 0;
    for (int i = 0; i < capacity; i++) frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == pages[i]) {
                found = 1;  
                break;
            }
        }
        if (!found) {
            int replace = -1, farthest = i + 1;
            for (int j = 0; j < capacity; j++) {
                int k;
                for (k = i + 1; k < n; k++) {
                    if (frame[j] == pages[k]) break;
                }
                if (k == n) {                                             //@Adwait-Borate
                    replace = j;
                    break;
                } else if (k > farthest) {                                //@Adwait-Borate
                    farthest = k;
                    replace = j;
                }
            }
            if (replace == -1) replace = 0;                               //@Adwait-Borate
            frame[replace] = pages[i];
            faults++;
        }
        printf("Frame: ");
        for (int j = 0; j < capacity; j++) printf("%d ", frame[j]);
        printf("\n");
    }
    printf("Total Page Faults (Optimal): %d\n", faults);
}

int main() {
    int choice, n, capacity;
    printf("Enter number of pages: ");
    scanf("%d", &n);
    int pages[n];
    printf("Enter the pages: ");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);
    printf("Enter the frame capacity: ");
    scanf("%d", &capacity);

    do {
        printf("Choose Page Replacement Algorithm:\n1. FIFO\n2. LRU\n3. Optimal\n4. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fifo(pages, n, capacity);
                break;
            case 2:
                lru(pages, n, capacity);
                break;
            case 3:
                optimal(pages, n, capacity);
                break;
            case 4:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}

// ./page_replacement
// gcc page_replacement.c -o page_replacement
// nano page_replacement.c


#include <stdio.h>
 #include <stdlib.h>
 #define MAX_FRAMES 10
 #define MAX_PAGES 20
 void displayFrames(int frames[], int frameCount) {
 for (int i = 0; i < frameCount; i++) {
 if (frames[i] !=-1) {
 printf("%d ", frames[i]);
 } else {
 printf("- ");
 }
 }
 printf("\n");
 }
 // FCFS Page Replacement Algorithm
 void fcfs(int pages[], int pageCount, int frameCount) {
 int frames[MAX_FRAMES];
 for (int i = 0; i < frameCount; i++) frames[i] =-1; // Initialize frames as empty
 int pageFaults = 0;
 for (int i = 0; i < pageCount; i++) {
 int j;
 for (j = 0; j < frameCount; j++) {
 if (frames[j] == pages[i]) {
 break; // Page is already in a frame
 }
 }
 if (j == frameCount) { // Page fault
 frames[pageFaults % frameCount] = pages[i];
 pageFaults++;
 printf("Page fault: ");
 displayFrames(frames, frameCount);
 }
 }
 printf("Total Page Faults: %d\n", pageFaults);
 }
 // LRU Page Replacement Algorithm
 void lru(int pages[], int pageCount, int frameCount) {
 int frames[MAX_FRAMES];
 int lastUsed[MAX_FRAMES]; // To track when each frame was last used
 int pageFaults = 0;
for (int i = 0; i < frameCount; i++) {
 frames[i] =-1; // Initialize all frames as empty
 lastUsed[i] =-1; // Initialize last used times as empty
 }
 for (int i = 0; i < pageCount; i++) {
 int j;
 // Check if the page is already in one of the frames
 for (j = 0; j < frameCount; j++) {
 if (frames[j] == pages[i]) {
 lastUsed[j] = i; // Update the last used time for this frame
 break;
 }
 }
 if (j == frameCount) { // Page fault occurs
 int lruIndex = 0;
 // Find the least recently used frame
 for (int k = 1; k < frameCount; k++) {
 if (lastUsed[k] < lastUsed[lruIndex]) {
 lruIndex = k;
 }
 }
 // Replace the LRU frame with the new page
 frames[lruIndex] = pages[i];
 lastUsed[lruIndex] = i; // Update last used time
 pageFaults++;
 printf("Page fault: ");
 displayFrames(frames, frameCount);
 }
 }
 printf("Total Page Faults: %d\n", pageFaults);
 }
 // Optimal Page Replacement Algorithm
 void optimal(int pages[], int pageCount, int frameCount) {
 int frames[MAX_FRAMES];
 for (int i = 0; i < frameCount; i++) frames[i] =-1; // Initialize frames as empty
 int pageFaults = 0;
for (int i = 0; i < pageCount; i++) {
 int j;
 for (j = 0; j < frameCount; j++) {
 if (frames[j] == pages[i]) {
 break; // Page is already in a frame
 }
 }
 if (j == frameCount) { // Page fault
 int optimalIndex =-1;
 int farthest =-1;
 for (int k = 0; k < frameCount; k++) {
 int nextUsage =-1;
 for (int l = i + 1; l < pageCount; l++) {
 if (frames[k] == pages[l]) {
 nextUsage = l;
 break;
 }
 }
 if (nextUsage ==-1) {
 optimalIndex = k; // Found a frame that is not used again
 break;
 }
 if (nextUsage > farthest) {
 farthest = nextUsage;
 optimalIndex = k;
 }
 }
 frames[optimalIndex] = pages[i];
 pageFaults++;
 printf("Page fault: ");
 displayFrames(frames, frameCount);
 }
 }
 printf("Total Page Faults: %d\n", pageFaults);
 }
 int main() {
 int pages[MAX_PAGES];
 int pageCount, frameCount;
 printf("Enter number of pages (max %d): ", MAX_PAGES);
 scanf("%d", &pageCount);
 printf("Enter page reference string:\n");
for (int i = 0; i < pageCount; i++) {
 scanf("%d", &pages[i]);
 }
 // Take frame count from user with no restrictions
 printf("Enter number of frames: ");
 scanf("%d", &frameCount);
 printf("\n--- FCFS Algorithm---\n");
 fcfs(pages, pageCount, frameCount);
 printf("\n--- LRU Algorithm---\n");
 lru(pages, pageCount, frameCount);
 printf("\n--- Optimal Algorithm---\n");
 optimal(pages, pageCount, frameCount);
 return 0;
 }
 Fifo
 Enter number of pages (max 20): 15
 Enter page reference string:
 7 01203042303120
 Enter number of frames: 3--- FCFS Algorithm--
Page fault: 7--
 Page fault: 7 0
Page fault: 7 0 1
 Page fault: 2 0 1
 Page fault: 2 3 1
 Page fault: 2 3 0
 Page fault: 4 3 0
 Page fault: 4 2 0
 Page fault: 4 2 3
 Page fault: 0 2 3
 Page fault: 0 1 3
 Page fault: 0 1 2
 Total Page Faults: 12
 Optimal
Enter number of pages (max 20): 10
 Enter page reference string:
 4 761761272
 Enter number of frames: 3--- FCFS Algorithm--
Page fault: 4--
 Page fault: 4 7
Page fault: 4 7 6
 Page fault: 1 7 6
 Page fault: 1 2 6
 Page fault: 1 2 7
 Total Page Faults: 6--- LRU Algorithm--
Page fault: 4--
 Page fault: 4 7
Page fault: 4 7 6
 Page fault: 1 7 6
 Page fault: 1 2 6
 Page fault: 1 2 7
 Total Page Faults: 6--- Optimal Algorithm--
Page fault: 4--
 Page fault: 7--
 Page fault: 7 6
Page fault: 7 6 1
 Page fault: 7 2 1
 Total Page Faults: 5
 === Code Execution Successful ===