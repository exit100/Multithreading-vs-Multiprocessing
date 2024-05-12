#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX_SIZE 250
#define OUTPUT_FILE "./multiplication/process_output.txt"

typedef struct {
    int size;
    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
} SharedData;

void generate_random_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 10; // Generate random numbers between 0 and 9
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Provide the size of the matrix as well.\n");
        exit(EXIT_FAILURE);
    }

    printf("\033[33m ```MULTIPROCESSING ```\n````````ACTIVE````````\n");
   // printf("```MULTIPROCESSING ```\n````````ACTIVE````````\n");
    printf("\033[0m");


    int size = atoi(argv[1]);
    if (size <= 0 || size > MAX_SIZE) {
        fprintf(stderr, "Matrix size must be between 1 and %d.\n", MAX_SIZE);
        exit(EXIT_FAILURE);
    }

    const int SHARED_SIZE = sizeof(SharedData);
    const char *name = "/sharedMemory";
    int shared_mem_fd;
    SharedData *shared_data;

    // Seed random number generator
    srand(time(NULL));

    // Create shared memory segment
    shared_mem_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shared_mem_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(shared_mem_fd, SHARED_SIZE);

    // Map shared memory segment into address space
    shared_data = (SharedData *)mmap(NULL, SHARED_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Set matrix size
    shared_data->size = size;

    // Generate random matrices
    generate_random_matrix(shared_data->matrix1, size);
    generate_random_matrix(shared_data->matrix2, size);

    // Start timing
    clock_t start_time = clock();

    // Fork child processes
    for (int i = 0; i < size; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            int row = i;
            for (int col = 0; col < size; col++) {
                int sum = 0;
                for (int k = 0; k < size; k++) {
                    sum += shared_data->matrix1[row][k] * shared_data->matrix2[k][col];
                }
                shared_data->result[row][col] = sum;
            }
            exit(EXIT_SUCCESS);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < size; i++) {
        wait(NULL);
    }

    // End timing for parallel computation
    clock_t end_time = clock();

    // Calculate elapsed time in seconds for parallel computation
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Display the result
    // printf("Matrix 1:\n");
    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         printf("%d ", shared_data->matrix1[i][j]);
    //     }
    //     printf("\n");
    // }

    // printf("\nMatrix 2:\n");
    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         printf("%d ", shared_data->matrix2[i][j]);
    //     }
    //     printf("\n");
    // }

    // printf("\nResult:\n");
    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         printf("%d ", shared_data->result[i][j]);
    //     }
    //     printf("\n");
    // }

    // Display time taken for parallel computation
    printf("\nTime taken: %.6f seconds\n", elapsed_time);

    // Write time taken and matrix size to output file
    FILE *output_file = fopen(OUTPUT_FILE, "a");
    if (output_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(output_file, "Matrix Size: %d x %d, Time taken: %.6f seconds\n", size, size, elapsed_time);
    fclose(output_file);

    // Unmap and close shared memory
    if (munmap(shared_data, SHARED_SIZE) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    close(shared_mem_fd);

    // Unlink shared memory
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}
