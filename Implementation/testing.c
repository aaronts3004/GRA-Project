#include "testing.h"

/* TESTING FUNCTIONS */
void printPrimes(const uint64_t* primes_array, size_t n){
    printf("\nThe following first %lu primes have been written into the array:\n", n);
    if (n == 0){
        return;
    }
    int i;
    printf("[" );
    for (i = 0; i < n; i++){
        if (i == n-1){              // Print last-last prime
            printf("%lu", primes_array[i]);
            break;
        }
        printf("%lu, ", primes_array[i]);
        if ((i>0)&&(i % 20 == 0)){
            printf("\n");           // print a newline every 20 primes
        }
    }
    printf("]\n");
}

void testAll(size_t n) {
    // Array to store primes 
    uint64_t* prims_array = (uint64_t *) malloc(n * sizeof(uint64_t));
    if (prims_array == NULL){
        fprintf(stderr, "Failed to allocate memory for prims_array in testAll() function\n");
        exit(EXIT_FAILURE);
    }
    printf("Running test with n=%lu\n", n);

    struct timespec start;         // Variables to measure time
    struct timespec end;
    double time;

    // Run V0 and measure time
    clock_gettime(CLOCK_MONOTONIC, &start);
    prim(n, prims_array);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time = end.tv_sec - start.tv_sec + 1e-9*(end.tv_nsec - start.tv_nsec);
    printf("V0, Sieve: %f seconds.\n", time);
    memset(prims_array, 0, n*sizeof(uint64_t));

    if (n < 10000000) {         
        clock_gettime(CLOCK_MONOTONIC, &start);
        prim_V1(n, prims_array);
        clock_gettime(CLOCK_MONOTONIC, &end);
        time = end.tv_sec - start.tv_sec + 1e-9*(end.tv_nsec - start.tv_nsec);
        printf("V1, Trial: %f seconds.\n", time);
        memset(prims_array, 0, n*sizeof(uint64_t));

    } else {        // Avoid running V1 with large input
        printf("Did not run linear, for n>1000000 trial division is inefficient.\n");
    }

    // Run V2
    clock_gettime(CLOCK_MONOTONIC, &start);
    prim_V2(n, prims_array);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time = end.tv_sec - start.tv_sec + 1e-9*(end.tv_nsec - start.tv_nsec);
    printf("V2, Sieve SIMD1: %f seconds.\n", time);
    memset(prims_array, 0, n*sizeof(uint64_t));

    // Run V3
    clock_gettime(CLOCK_MONOTONIC, &start);
    prim_V3(n, prims_array);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time = end.tv_sec - start.tv_sec + 1e-9*(end.tv_nsec - start.tv_nsec);
    printf("V3, Sieve SIMD2: %f seconds.\n", time);
    memset(prims_array, 0, n*sizeof(uint64_t));
    
    // Free memory
    free(prims_array);
}

void testCorrect(void){
    int failed = 0;
    size_t n = 50;
    // Initialize the array with the first n=100 correct primes
    uint64_t expected_array[50] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 
                                59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 
                                127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 
                                193, 197, 199, 211, 223, 227, 229};
    // Allocate array for 100 numbers
    uint64_t* result_array = (uint64_t *) malloc(n * sizeof(uint64_t));
    // Call the prim function for each implementation and check for correctness
    prim(n, result_array);
    for (int i = 0; i < n; i++){
        if (expected_array[i] != result_array[i]){
            fprintf(stderr, "Failed test for implementation V0 at index i=%d\n"
                            "expected %lu but was %lu\n", i, expected_array[i], result_array[i]);
            failed++;
        }
    }
    memset(result_array, 0, n*sizeof(uint64_t));        // reset result to 0

    prim_V1(n, result_array);
    for (int i = 0; i < n; i++){
        if (expected_array[i] != result_array[i]){
            fprintf(stderr, "Failed test for implementation V1 at index i=%d\n"
                            "expected %lu but was %lu\n", i, expected_array[i], result_array[i]);
            failed++;
        }
    }
    memset(result_array, 0, n*sizeof(uint64_t));

    prim_V2(n, result_array);
    for (int i = 0; i < n; i++){
        if (expected_array[i] != result_array[i]){
            fprintf(stderr, "Failed test for implementation V2 at index i=%d\n"
                            "expected %lu but was %lu\n", i, expected_array[i], result_array[i]);
            failed++;
        }
    }
    memset(result_array, 0, n*sizeof(uint64_t));

    prim_V2(n, result_array);
    for (int i = 0; i < n; i++){
        if (expected_array[i] != result_array[i]){
            fprintf(stderr, "Failed test for implementation V3 at index i=%d\n"
                            "expected %lu but was %lu\n", i, expected_array[i], result_array[i]);
            failed++;
        }
    }
    printf("Testing correctness: failed %d tests in total\n", failed);
    free(result_array);
}