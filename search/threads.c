#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define OUTPUT_FILE "./search/threads_output.txt"
#define NUM_THREADS 4  // Number of threads to use

// Global variables for shared data
int* arr;
int size;
int key;
int segment_size;
int ans0 = -1;
int ans1 = -1;
int ans2 = -1;
int ans3 = -1;



// Function to perform linear search on a segment of the array
void* linearSearchThread(void* arg) {
    int thread_id = *(int*)arg;
    int start = thread_id * segment_size;
    int end;

	if (thread_id == NUM_THREADS - 1) {
	    end = size;
	} else {
	    end = (thread_id + 1) * segment_size;
	}
  //  int end = (thread_id == NUM_THREADS - 1) ? size : (thread_id + 1) * segment_size;
    
    for (int i = start; i < end; ++i) {
        if (arr[i] == key) {
        switch(thread_id){
        	case 0:
        		ans0=i;
        		break;
        	case 1:
           		ans1=i;
        		break;     		
        	case 2:
        	        ans2=i;
        		break;
        	case 3:		
                	ans3=i;
        		break;
        }
          //  ans = i;
            break;
        }
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));  // Seed for random number generation

    printf("\033[33m ```MULTITHREADING ```\n````````ACTIVE````````\n");
   // printf("```MULTIPROCESSING ```\n````````ACTIVE````````\n");
    printf("\033[0m");

    // Generate a random size between 10000 and 100000
    //size = rand() % 900001 + 1000000;
        size = 50000000;
    arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    // Fill the array with random values between 1 and 100
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 10000 + 1;
    }

    key = 1212;

    printf("The array size is: %d, and the key we are searching for is: %d\n",size,key);
    

    segment_size = size / NUM_THREADS;
    
    clock_t start_time = clock();

    // Create multiple threads
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, linearSearchThread, (void*)&thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    free(arr);  // Deallocate memory for the array
    if(ans0!=-1)
    	printf("Key found at index: %d by Thread 0\n", ans0);
    else 
    	printf("Key not found in Thread 0\n");	
    if(ans1!=-1)
    	printf("Key found at index: %d by Thread 1\n", ans1);
    else 
    	printf("Key not found in Thread 1\n");	
    if(ans2!=-1)
    	printf("Key found at index: %d by Thread 2\n", ans2);
    else 
    	printf("Key not found in Thread 2\n");	    	
    if(ans3!=-1)
    	printf("Key found at index: %d by Thread 3\n", ans3);	
    else 
    	printf("Key not found in Thread 3\n");	    			
    
    printf("Time taken was: %.6f seconds\n",elapsed_time);
    
    FILE *output_file = fopen(OUTPUT_FILE, "a");
    if (output_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(output_file, "Array size: %d, Time taken: %.6f seconds\n", size, elapsed_time);
    fclose(output_file);
    
    return 0;
}

