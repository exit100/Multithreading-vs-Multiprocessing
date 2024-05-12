#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        pid_t child = fork();
        if (child == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (child == 0) {
            // Child process sorts the left half
            mergeSort(arr, l, m);
            exit(EXIT_SUCCESS);
        }
        else {
            // Parent process sorts the right half
            mergeSort(arr, m + 1, r);
            wait(NULL); // Wait for the child process to finish
        }

        merge(arr, l, m, r);
    }
}

void TakingInput(int arr[], int len)
{
    int i;
    for (i = 0; i < len; i++) {
        arr[i] = rand() % 1000;
    }
}

void printArr(int* arr, int size){
    for(int i=0;i<size;i++){
        printf("%d ",arr[i]);
    }
}

void isSorted(int arr[], int len)
{
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
    printf("The array has been sorted successfully\n");
}

int main(int argc, char* argv[])
{

    printf("\033[33m ```MULTIPROCESSING ```\n````````ACTIVE````````\n");
    printf("\033[0m");

    if (argc != 2) {
        printf("Usage: %s <array_length>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int length = atoi(argv[1]);
    if (length <= 0) {
        printf("Array length must be a positive integer\n");
        return EXIT_FAILURE;
    }

    int shmid = shmget(IPC_PRIVATE, length * sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return EXIT_FAILURE;
    }

    int* arr = (int*)shmat(shmid, NULL, 0);
    if (arr == (int*)(-1)) {
        perror("shmat");
        return EXIT_FAILURE;
    }

    TakingInput(arr, length);


    clock_t startingTime, endingTime;
    startingTime = clock();

    mergeSort(arr, 0, length - 1);

    endingTime=clock();
    printf("Time taken: %f\n", (double)(endingTime - startingTime) / CLOCKS_PER_SEC);

   // printArr(arr,length);

    shmdt(arr);
    shmctl(shmid, IPC_RMID, NULL);

    FILE *outputFile;
    outputFile = fopen("./merge/Multiprocessing.txt", "a"); 

    if (outputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(outputFile, "Array Length: %d, Time Taken: %f seconds\n", length, (endingTime - startingTime) / (double) CLOCKS_PER_SEC);
    fclose(outputFile);

    return EXIT_SUCCESS;
}
