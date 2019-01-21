#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

int array[SIZE]; // Global array to hold individual thread accumulators

typedef struct threadArgs { // Struct for thread argument
    int low;
    int high;
    int index;
} threadArgs;

void * sum(void * voidArg) { // Thread function
    threadArgs * myArgs = *((threadArgs**)voidArg);
    int low = myArgs->low;
    int high = myArgs->high;

    int accum = 0;
    for(int i = low; i < high; ++i) {
        accum += i;
    }
    array[myArgs->index] = accum;
}

int main() 
{
    for(int i = 0; i < SIZE; ++i) { // Initialize global array
        array[i] = 0;
    }

    int toSum;
    printf("Input the range to sum: ");
    scanf("%d", &toSum);

    int nThreads;
    printf("Input number of threads to use: ");
    scanf("%d", &nThreads);

    int leftOver = 0;
    while(toSum % nThreads) {
        leftOver += toSum;
        --toSum;
    }

    threadArgs ** myArgs = malloc(sizeof(threadArgs)*nThreads);
    for(int i = 0; i < nThreads; ++i) {
        int interval = toSum/nThreads;
        myArgs[i] = malloc(sizeof(threadArgs));
        myArgs[i]->low = interval*i;
        myArgs[i]->high = (interval*i) + interval;
        myArgs[i]->index = i;
    }

    pthread_t myThreads[nThreads];
    for(int i = 0; i < nThreads; ++i) {
        pthread_create(&myThreads[i], NULL, sum, (void*)&myArgs[i]); 
    }
    for(int i = 0; i < nThreads; ++i) {
        pthread_join(myThreads[i], NULL);
    }

    int accum = 0;
    for(int i = 0; i < nThreads; ++i) { // Sum individual thread totals
        accum += array[i];
    }
    accum += leftOver + toSum;

    printf("Sum of the given range: %d\n", accum);

    return 0;
}
