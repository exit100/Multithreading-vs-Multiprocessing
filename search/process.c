#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define OUTPUT_FILE "./search/process_output.txt"
#define NUM_PROCESSES 4  // Number of processes to use

// Function to perform linear search on a segment of the array
int linearSearchSegment(int arr[], int start, int end, int key) {
    for (int i = start; i < end; ++i) {
        if (arr[i] == key) {
            return i;
        }
    }
    return -1;  // Key not found
}

int main() {
    srand(time(NULL));  // Seed for random number generation
    int ans = -1;

    printf("\033[33m ```MULTIPROCESSING ```\n````````ACTIVE````````\n");
   // printf("```MULTIPROCESSING ```\n````````ACTIVE````````\n");
    printf("\033[0m");

    // Generate a random size between 10000 and 100000
   // int size = rand() % 900001 + 1000000;
    int size=50000000;
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    // Fill the array with random values between 1 and 100
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 10000 + 1;
    }


    int key = 1212;


    printf("The array size is: %d, and the key we are searching for is: %d\n",size,key);
    
    
    int segment_size = size / NUM_PROCESSES;
    
    clock_t start_time = clock();

    // Create multiple child processes
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        pid_t pid = fork();

        if (pid == 0) {  // Child process
            int start = i * segment_size;
            int end = (i == NUM_PROCESSES - 1) ? size : (i + 1) * segment_size;
            int result = linearSearchSegment(arr, start, end, key);
            if (result != -1) {
                printf("Key found at index %d in child process %d.\n", result, i);
                exit(EXIT_SUCCESS);
            } else {
                printf("Key not found in child process %d.\n", i);
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            perror("fork");
            return 1;
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        wait(NULL);
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    free(arr);  // Deallocate memory for the array
    printf("Time taken: %.6f seconds\n", elapsed_time);
    
    FILE *output_file = fopen(OUTPUT_FILE, "a");
    if (output_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(output_file, "Array size: %d, Time taken: %.6f seconds\n", size,elapsed_time);
    fclose(output_file);
    
    return 0;
}

