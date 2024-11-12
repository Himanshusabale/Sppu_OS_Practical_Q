#include <stdio.h>
#include <math.h>
// this library is for qsort function
#include <stdlib.h>
// this library is for memset function
#include <string.h>

#define disk_size 200

void calculateDiff(int *tracks, int *dist, int n, int head)
{
    for (int i = 0; i < n; i++)
    {
        dist[i] = fabs(head - tracks[i]);
    }
}

// Return the index of track which is at minimum distance with head
int findMinTrackPosition(int *tracks, int *dist, int *isAccessed, int n, int head)
{
    int min = __INT_MAX__;
    int index = -1;
    for (int i = 0; i < n; i++)
    {
        // if the track is not previously accessed
        // and dist[i] is less than minimum
        // then update the
        if (isAccessed[i] == 0 && min > dist[i])
        {
            min = dist[i];
            index = i;
        }
    }
    return index;
}
void SSTF(int *tracks, int head, int n)
{

    // dist will keep track of distance between curr_head and each track
    // 2nd value of this 2d array is boolean isAccessed
    int dist[n];
    int isAccessed[n];

    // first in sequence will be the head and then rest of the tracks hence n+1
    int seek_sequence[n + 1];
    memset(dist, 0, n * sizeof(int));
    memset(isAccessed, 0, n * sizeof(int));
    memset(seek_sequence, 0, (n + 1) * sizeof(int));

    int seek_count = 0;

    for (int i = 0; i < n; i++)
    {
        seek_sequence[i] = head;
        calculateDiff(tracks, dist, n, head);
        // get the index of track with minimum distance
        int index = findMinTrackPosition(tracks, dist, isAccessed, n, head);
        // make isAccessed value to 1 to indicate that track is accessed
        isAccessed[index] = 1;

        seek_count += dist[index];

        // accessed track is not new head
        head = tracks[index];
    }
    // update the last head in seek sequences
    seek_sequence[n] = head;

    printf("Total seek operations: %d\n", seek_count);

    printf("Seek sequence is : \n");

    for (int i = 0; i < n; i++)
    {
        printf("%d->", seek_sequence[i]);
    }
    printf("\n");
}

// void is added to comparator function to make it more generic and work with different data types
int comp(const void *l, const void *n)
{
    // if difference negative then l is small
    // if difference is positive then l is large
    // if 0 then both l and n are equal
    // This function sorts them is ascending order
    return (*(int *)l - *(int *)n);
}
void SCAN(int *tracks, int head, int n, int direction)
{
    // direction 1 means rightside
    // direction 0 means leftside
    int left = 0, right = 0;
    int leftside[n], rightside[n];
    if (direction == 0)
        leftside[left++] = 0;
    else if (direction == 1)
        rightside[right++] = disk_size - 1;

    for (int i = 0; i < n; i++)
    {
        if (tracks[i] < head)
            leftside[left++] = tracks[i];
        if (tracks[i] > head)
            rightside[right++] = tracks[i];
    }

    qsort(leftside, left, sizeof(int), comp);
    qsort(rightside, right, sizeof(int), comp);

    int seek_sequence[n + 3];
    int curr_track, dist, seek_count = 0;

    int go = 2, index = 0;
    while (go--)
    {
        if (direction == 0)
        {
            for (int i = left - 1; i >= 0; i--)
            {
                // get the current track
                curr_track = leftside[i];
                // insert the curr_track into seeksequence
                seek_sequence[index++] = curr_track;

                dist = fabs(curr_track - head);

                seek_count += dist;
                head = curr_track;
            }
            direction = 1;
        }
        else if (direction == 1)
        {
            for (int i = 0; i < right; i++)
            {
                // get the current track
                curr_track = rightside[i];
                // insert the curr_track into seeksequence
                seek_sequence[index++] = curr_track;

                dist = fabs(curr_track - head);

                seek_count += dist;
                head = curr_track;
            }
            direction = 0;
        }
    }

    printf("Num of seek process = %d\n", seek_count);
    printf("Sequence is: ");
    for (int p_s = 0; p_s < index; p_s++)
    {
        printf("%d\t", seek_sequence[p_s]);
    }
}

void CLOOK(int *tracks, int head, int n)
{

    int leftside[n], rightside[n];
    int left = 0, right = 0;
    for (int i = 0; i < n; i++)
    {
        if (tracks[i] < head)
            leftside[left++] = tracks[i];
        if (tracks[i] > head)
            rightside[right++] = tracks[i];
    }

    qsort(leftside, left, sizeof(int), comp);
    qsort(rightside, right, sizeof(int), comp);

    int seek_sequence[n + 1];
    int curr_track, dist, seek_count = 0;
    int index = 0;

    // first service the request on right side of head
    for (int i = 0; i < right; i++)
    {
        curr_track = rightside[i];

        seek_sequence[index++] = curr_track;

        // calculate distance
        dist = fabs(head - curr_track);

        // calculate seekcount
        seek_count += dist;

        // update the head
        head = curr_track;
    }
    // once reached to the right end then go to
    // start of left side
    // therefore calculate the seekcount for going to right end to leftstart
    dist = fabs(head - leftside[0]);
    seek_count += dist;
    head = leftside[0];
    // now go to the start of left side
    // and start services request from that point
    for (int i = 0; i < left; i++)
    {
        curr_track = leftside[i];
        seek_sequence[index++] = curr_track;

        dist = fabs(head - curr_track);

        seek_count += dist;

        head = curr_track;
    }

    printf("Total number of seek operations performed: %d\n", seek_count);

    printf("Seek sequence is: \n");

    for (int i = 0; i < index; i++)
    {
        printf("%d\t", seek_sequence[i]);
    }
    printf("\n");
}

int main()
{
    int n;
    printf("Enter number of tracks: ");
    scanf("%d", &n);
    int tracks[n];
    printf("Enter the tracks:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &tracks[i]);

    int head;
    printf("Enter initial head position: ");
    scanf("%d", &head);

    int choice, direction;
    printf("Choose the algorithm:\n1. SSTF\n2. SCAN\n3. CLOOK\n");
    scanf("%d", &choice);

    if (choice == 2)
    {
        printf("Enter direction (0 for left, 1 for right): ");
        scanf("%d", &direction);
    }

    switch (choice)
    {
    case 1:
        SSTF(tracks, head, n);
        break;
    case 2:
        SCAN(tracks, head, n, direction);
        break;
    case 3:
        CLOOK(tracks, head, n);
        break;
    default:
        printf("Invalid choice.\n");
    }

    return 0;
}


// gcc -o disk_scheduling disk_scheduling.c -lm
// ./disk_scheduling



assignment-08
 #include <stdio.h>
 #include <stdlib.h>
 #define MAX_REQUESTS 100
 // Function prototypes
 void sstf(int requests[], int n, int initial_head);
 void scan(int requests[], int n, int initial_head, int total_cylinders);
 void c_look(int requests[], int n, int initial_head, int total_cylinders);
 int main() {
 int requests[MAX_REQUESTS];
 int n, initial_head, total_cylinders;
 // Input the number of requests
 printf("Enter the number of disk requests: ");
 scanf("%d", &n);
 // Input the disk requests
 printf("Enter the disk requests: ");
 for (int i = 0; i < n; i++) {
 scanf("%d", &requests[i]);
 }
 // Input the initial head position
 printf("Enter the initial head position: ");
 scanf("%d", &initial_head);
 // Input the total number of cylinders
 printf("Enter the total number of cylinders: ");
 scanf("%d", &total_cylinders);
 // SSTF Disk Scheduling
 printf("\nSSTF Disk Scheduling:\n");
 sstf(requests, n, initial_head);
 // SCAN Disk Scheduling
 printf("\nSCAN Disk Scheduling:\n");
 scan(requests, n, initial_head, total_cylinders);
 // C-LOOK Disk Scheduling
 printf("\nC-LOOK Disk Scheduling:\n");
c_look(requests, n, initial_head, total_cylinders);
 return 0;
 }
 // Shortest Seek Time First (SSTF) algorithm
 void sstf(int requests[], int n, int initial_head) {
 int completed[MAX_REQUESTS] = {0};
 int total_distance = 0;
 int current_head = initial_head;
 for (int i = 0; i < n; i++) {
 int min_distance = 10000;
 int next_index =-1;
 // Find the closest request
 for (int j = 0; j < n; j++) {
 if (!completed[j]) {
 int distance = abs(current_head- requests[j]);
 if (distance < min_distance) {
 min_distance = distance;
 next_index = j;
 }
 }
 }
 // Move to the closest request
 if (next_index !=-1) {
 total_distance += min_distance;
 current_head = requests[next_index];
 completed[next_index] = 1;
 printf("Served Request: %d\n", requests[next_index]);
 }
 }
 printf("Total head movement: %d\n", total_distance);
 }
 // SCAN Disk Scheduling algorithm (Elevator Algorithm)
 void scan(int requests[], int n, int initial_head, int total_cylinders) {
 int total_distance = 0;
 int current_head = initial_head;
 int direction = 1; // 1 for moving right (towards the end),-1 for moving left
// Sort the requests in ascending order
 for (int i = 0; i < n; i++) {
 for (int j = 0; j < n- i- 1; j++) {
 if (requests[j] > requests[j + 1]) {
 int temp = requests[j];
 requests[j] = requests[j + 1];
 requests[j + 1] = temp;
 }
 }
 }
 printf("Served Requests in SCAN order:\n");
 // Move towards the right (greater requests)
 for (int i = 0; i < n; i++) {
 if (requests[i] >= current_head) {
 total_distance += abs(current_head- requests[i]);
 current_head = requests[i];
 printf("%d ", requests[i]);
 }
 }
 // After reaching the end, move to the last cylinder and reverse direction
 if (current_head != total_cylinders- 1) {
 total_distance += abs(current_head- (total_cylinders- 1));
 current_head = total_cylinders- 1;
 }
 // Now move back in the opposite direction
 for (int i = n- 1; i >= 0; i--) {
 if (requests[i] < initial_head) {
 total_distance += abs(current_head- requests[i]);
 current_head = requests[i];
 printf("%d ", requests[i]);
 }
 }
 printf("\nTotal head movement: %d\n", total_distance);
 }
 // C-LOOK Disk Scheduling algorithm
 void c_look(int requests[], int n, int initial_head, int total_cylinders) {
 int total_distance = 0;
 int current_head = initial_head;
// Sort the requests in ascending order
 for (int i = 0; i < n; i++) {
 for (int j = 0; j < n- i- 1; j++) {
 if (requests[j] > requests[j + 1]) {
 int temp = requests[j];
 requests[j] = requests[j + 1];
 requests[j + 1] = temp;
 }
 }
 }
 printf("Served Requests in C-LOOK order:\n");
 // Serve requests to the right of the head (larger requests)
 for (int i = 0; i < n; i++) {
 if (requests[i] >= current_head) {
 total_distance += abs(current_head- requests[i]);
 current_head = requests[i];
 printf("%d ", requests[i]);
 }
 }
 // Jump back to the beginning of the requests (smallest one)
 if (current_head != requests[0]) {
 total_distance += abs(current_head- requests[0]);
 current_head = requests[0];
 }
 // Serve remaining requests to the right of the smallest one
 for (int i = 0; i < n; i++) {
 if (requests[i] < initial_head) {
 total_distance += abs(current_head- requests[i]);
 current_head = requests[i];
 printf("%d ", requests[i]);
 }
 }
 printf("\nTotal head movement: %d\n", total_distance);
 }
/tmp/8a8V5blqHX.o
 Enter the number of disk requests: 8
 Enter the disk requests: 98 183 37 122 14 124 65 67
 Enter the initial head position: 53
 Enter the total number of cylinders: 200
 SSTF Disk Scheduling:
 Served Request: 65
 Served Request: 67
 Served Request: 37
 Served Request: 14
 Served Request: 98
 Served Request: 122
 Served Request: 124
 Served Request: 183
 Total head movement: 236
 SCANDisk Scheduling:
 Served Requests in SCAN order:
 65 67 98 122 124 183 37 14
 Total head movement: 331
 C-LOOK Disk Scheduling:
 Served Requests in C-LOOK order:
 65 67 98 122 124 183 14 37
 Total head movement: 322
 === Code Execution Successful ===
 /tmp/IbuaigowJh.o
 Enter the number of disk requests: 9
 Enter the disk requests: 55 58 39 18 90 160 150 38 184
 Enter the initial head position: 100
 Enter the total number of cylinders: 200
 SSTF Disk Scheduling:
 Served Request: 90
 Served Request: 58
 Served Request: 55
 Served Request: 39
 Served Request: 38
 Served Request: 18
Served Request: 150
 Served Request: 160
 Served Request: 184
 Total head movement: 248
 SCANDisk Scheduling:
 Served Requests in SCAN order:
 150 160 184 90 58 55 39 38 18
 Total head movement: 280
 C-LOOK Disk Scheduling:
 Served Requests in C-LOOK order:
 150 160 184 18 38 39 55 58 90
 Total head movement: 322
 === Code Execution Successful ===



 new #include <stdio.h>
#include <limits.h>

// Function to display the current state of frames
void displayFrameState(int frames[], int frameCount) {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == -1)
            printf(" - ");
        else
            printf(" %d ", frames[i]);
   }
   printf("\n");
}

// First-Come-First-Serve (FCFS) Page Replacement Algorithm
void fcfsPageReplacement(int referenceString[], int totalPages, int frameCount) {
    int frames[frameCount], pageFaultCount = 0, nextFrameIndex = 0;
    for (int i = 0; i < frameCount; i++) frames[i] = -1;
        printf("\nFCFS Page Replacement:\n");
        for (int i = 0; i < totalPages; i++) {
            int currentPage = referenceString[i];
            int pageFound = 0;
            // Check if the page is already in a frame
            for (int j = 0; j < frameCount; j++) {
                 if (frames[j] == currentPage) {
                      pageFound = 1;
                      break;
                 }
            }
            
            if (!pageFound) {
                 frames[nextFrameIndex] = currentPage;
                 nextFrameIndex = (nextFrameIndex + 1) % frameCount;
                 pageFaultCount++;
             }
                 displayFrameState(frames, frameCount);
        }
       printf("Total Page Faults: %d\n", pageFaultCount);
}


// Function to find the optimal frame to replace
int findOptimalReplacement(int referenceString[], int frames[], int frameCount, int currentIndex, int totalPages) {
int farthestUsage = currentIndex, replacePosition = -1;
for (int i = 0; i < frameCount; i++) {
int j;
for (j = currentIndex; j < totalPages; j++) {
if (frames[i] == referenceString[j]) {
if (j > farthestUsage) {
farthestUsage = j;
replacePosition = i;
}
break;
}
}
if (j == totalPages) {
return i;
}
}
return (replacePosition == -1) ? 0 : replacePosition;
}


// Optimal Page Replacement Algorithm
void optimalPageReplacement(int referenceString[], int totalPages, int frameCount) {
int frames[frameCount], pageFaultCount = 0;
for (int i = 0; i < frameCount; i++) frames[i] = -1;
printf("\nOptimal Page Replacement:\n");
for (int i = 0; i < totalPages; i++) {
int currentPage = referenceString[i];
int pageFound = 0;
// Check if the page is already in a frame
for (int j = 0; j < frameCount; j++) {
if (frames[j] == currentPage) {

pageFound = 1;
break;
}
}
if (!pageFound) {
if (i < frameCount) {
frames[i] = currentPage;
} else {
int replaceIndex = findOptimalReplacement(referenceString, frames, frameCount, i +
1, totalPages);
frames[replaceIndex] = currentPage;
}
pageFaultCount++;
}
displayFrameState(frames, frameCount);
}
printf("Total Page Faults: %d\n", pageFaultCount);
}

// Least Recently Used (LRU) Page Replacement Algorithm
void lruPageReplacement(int referenceString[], int totalPages, int frameCount) {
int frames[frameCount], pageFaultCount = 0, timeCounter = 0, lastUsedTime[frameCount];
for (int i = 0; i < frameCount; i++) frames[i] = -1;
printf("\nLRU Page Replacement:\n");
for (int i = 0; i < totalPages; i++) {
int currentPage = referenceString[i];
int pageFound = 0;
// Check if the page is already in a frame
for (int j = 0; j < frameCount; j++) {
if (frames[j] == currentPage) {
timeCounter++;
lastUsedTime[j] = timeCounter;
pageFound = 1;
break;
}
}
if (!pageFound) {
if (i < frameCount) {
frames[i] = currentPage;
lastUsedTime[i] = ++timeCounter;
} else {
int leastRecentTime = INT_MAX, replaceIndex = -1;
for (int j = 0; j < frameCount; j++) {
if (lastUsedTime[j] < leastRecentTime) {
leastRecentTime = lastUsedTime[j];
replaceIndex = j;
}
}
frames[replaceIndex] = currentPage;
lastUsedTime[replaceIndex] = ++timeCounter;
}
pageFaultCount++;
}
displayFrameState(frames, frameCount);
}
printf("Total Page Faults: %d\n", pageFaultCount);
}
// Main function
int main() {
int totalPages, frameCount, algorithmChoice;
printf("Enter total number of pages: ");
scanf("%d", &totalPages);
int referenceString[totalPages];
printf("Enter the reference string (pages):\n");
for (int i = 0; i < totalPages; i++) {
scanf("%d", &referenceString[i]);
}
printf("Enter the number of frames (minimum 3): ");
scanf("%d", &frameCount);
if (frameCount < 3) {
printf("Frame count should be at least 3. Exiting.\n");
return 0;
}
printf("\nSelect Page Replacement Algorithm:\n");
printf("1. FCFS\n2. LRU\n3. Optimal\nEnter choice: ");
scanf("%d", &algorithmChoice);
switch (algorithmChoice) {
case 1:
fcfsPageReplacement(referenceString, totalPages, frameCount);
break;
case 2:
lruPageReplacement(referenceString, totalPages, frameCount);
break;
case 3:
optimalPageReplacement(referenceString, totalPages, frameCount);
break;
default:
printf("Invalid choice. Exiting.\n");
return 0;
}
return 0;
}
