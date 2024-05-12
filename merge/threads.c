#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE 1024  
int array[MAX_SIZE];

typedef struct node {
    int i;
    int j;
} NODE;

void* mergesort(void* a);
void merge(int arr[], int low1, int high1, int high2);

void mergeSort(int arr[], int low, int high) {
    int len = high - low + 1;

    if (len <= 1) return; 

    NODE n1, n2;
    int mid = low + len / 2;

    n1.i = low;
    n1.j = mid - 1;

    n2.i = mid;
    n2.j = high;

    pthread_t tid1, tid2;
    int ret;

    ret = pthread_create(&tid1, NULL, mergesort, &n1);
    if (ret) {
        fprintf(stderr, "Error: Unable to create thread - ret: %d\n", ret);
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&tid2, NULL, mergesort, &n2);
    if (ret) {
        fprintf(stderr, "Error: Unable to create thread - ret: %d\n", ret);
        exit(EXIT_FAILURE);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    merge(arr, low, mid - 1, high);
}

void merge(int arr[], int low1, int high1, int high2) {
    int count = high2 - low1 + 1;
    int sortedArray[count];
    int i = low1, k = high1 + 1, m = 0;
    while (i <= high1 && k <= high2) {
        if (arr[i] < arr[k])
            sortedArray[m++] = arr[i++];
        else if (arr[k] < arr[i])
            sortedArray[m++] = arr[k++];
        else if (arr[i] == arr[k]) {
            sortedArray[m++] = arr[i++];
            sortedArray[m++] = arr[k++];
        }
    }

    while (i <= high1)
        sortedArray[m++] = arr[i++];

    while (k <= high2)
        sortedArray[m++] = arr[k++];

    int arr_count = low1;
    for (i = 0; i < count; i++, low1++)
        arr[low1] = sortedArray[i];
}

void isSorted(int arr[], int len) {
    if (len == 1) {
        printf("The array has been sorted successfully\n");
        return;
    }

    int i;
    for (i = 1; i < len; i++) {
        if (arr[i] < arr[i - 1]) {
            printf("Sorting Not Done\n");
            return;
        }
    }
    printf("The array has been sorted successfully. You can view it in \"dataFile.txt\" \n");
    return;
}

void TakingInput(int arr[], int len) {
    srand(time(NULL));
    for (int i = 0; i < len; i++) {
        arr[i] = rand() % 1000;
    }
}

void* mergesort(void* a) {
    NODE* p = (NODE*)a;
    int mid = (p->i + p->j) / 2;

    if (p->i >= p->j)
        return NULL;

    NODE n1, n2;
    n1.i = p->i;
    n1.j = mid;
    n2.i = mid + 1;
    n2.j = p->j;

    pthread_t tid1, tid2;
    int ret;

    ret = pthread_create(&tid1, NULL, mergesort, &n1);
    if (ret) {
        fprintf(stderr, "Error: Unable to create thread - ret: %d\n", ret);
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&tid2, NULL, mergesort, &n2);
    if (ret) {
        fprintf(stderr, "Error: Unable to create thread - ret: %d\n", ret);
        exit(EXIT_FAILURE);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    merge(array, p->i, mid, p->j);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    printf("\033[33m ```MULTITHREADING ```\n````````ACTIVE````````\n");
    printf("\033[0m");

    int length;
    length = atoi(argv[1]);

    srand(time(NULL));
    TakingInput(array, length);

    FILE* dataFile = fopen("./merge/data.txt", "a");
    if (dataFile == NULL) {
        perror("Error opening file");
        return 1;
    }
    fprintf(dataFile,"\nUnsorted Array Threads:\n");
    for (int i = 0; i < length; i++) {
        fprintf(dataFile,"%d ", array[i]);
    }

    clock_t startingTime, endingTime;
    startingTime = clock();

    NODE n;
    n.i = 0;
    n.j = length - 1;
    pthread_t tid;
    int ret;

    ret = pthread_create(&tid, NULL, mergesort, &n);
    if (ret) {
        fprintf(stderr, "Error: Unable to create thread - ret: %d\n", ret);
        exit(EXIT_FAILURE);
    }

    pthread_join(tid, NULL);

    endingTime = clock();

    isSorted(array, length);

    fprintf(dataFile,"\nSorted Array Threads:\n");
    for (int i = 0; i < length; i++) {
        fprintf(dataFile,"%d ", array[i]);
    }
    fprintf(dataFile,"\n");

    fclose(dataFile);

    printf("Time taken: %f\n", (double)(((double)(endingTime - startingTime) / CLOCKS_PER_SEC)/1000));
    FILE *outputFile;
    outputFile = fopen("./merge/MUltithreading.txt", "a"); 

    if (outputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(outputFile, "Array Length: %d, Time Taken: %f seconds\n", length,(double)(((double)(endingTime - startingTime) / CLOCKS_PER_SEC)/1000));
    fclose(outputFile);

    return 0;
}
