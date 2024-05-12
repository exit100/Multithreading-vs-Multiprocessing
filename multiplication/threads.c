#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_SIZE 250
#define OUTPUT_FILE "./multiplication/threads_output.txt"

typedef struct {
    int size;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
} SharedData;

SharedData shared_data;

void generate_random_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 10; // Generate random numbers between 0 and 9
        }
    }
}

void *multiply(void *args) {
    int row = *((int *)args);
    int size = shared_data.size;

    for (int j = 0; j < size; j++) { // column loop that runs on matrix 1
        int sum = 0;
        for (int i = 0; i < size; i++) { // row loop
            sum += shared_data.matrix1[row][i] * shared_data.matrix2[i][j];
        }
        shared_data.result[row][j] = sum;
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <matrix_size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("\033[33m ```MULTITHREADING ```\n````````ACTIVE````````\n");
   // printf("```MULTIPROCESSING ```\n````````ACTIVE````````\n");
    printf("\033[0m");


    int size = atoi(argv[1]);
    if (size <= 0 || size > MAX_SIZE) {
        fprintf(stderr, "Matrix size must be between 1 and %d\n", MAX_SIZE);
        exit(EXIT_FAILURE);
    }

    pthread_t threads[MAX_SIZE];
    int *thread_args[MAX_SIZE];

    // Seed random number generator
   // srand(time(NULL));

    // Set matrix size
    shared_data.size = size;

    // Generate random matrices
    generate_random_matrix(shared_data.matrix1, size);
    generate_random_matrix(shared_data.matrix2, size);

    // Start timing
    clock_t start_time = clock();

    // Create threads
    for (int i = 0; i < size; i++) {
        thread_args[i] = (int *)malloc(sizeof(int));
        *thread_args[i] = i;
        if (pthread_create(&threads[i], NULL, multiply, (void *)thread_args[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < size; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    // End timing
    clock_t end_time = clock();

    // Calculate elapsed time in seconds
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Display the result
    // printf("Matrix 1:\n");
    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         printf("%d ", shared_data.matrix1[i][j]);
    //     }
    //     printf("\n");
    // }

    // printf("\nMatrix 2:\n");
    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         printf("%d ", shared_data.matrix2[i][j]);
    //     }
    //     printf("\n");
    // }

    // printf("\nResult:\n");
    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         printf("%d ", shared_data.result[i][j]);
    //     }
    //     printf("\n");
    // }

    // Display time taken for computation
    printf("\nTime taken: %.6f seconds\n", elapsed_time);

    // Write time taken and matrix size to output file
    FILE *output_file = fopen(OUTPUT_FILE, "a");
    if (output_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(output_file, "Matrix Size: %d x %d, Time taken: %.6f seconds\n", size, size, elapsed_time);
    fclose(output_file);

    // Free allocated memory
    for (int i = 0; i < size; i++) {
        free(thread_args[i]);
    }

    return 0;
}
