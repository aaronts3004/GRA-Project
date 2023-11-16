#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include "prim.h"

/* Testing interface */

// Prints all the primes until n in the resulting array; n must be smaller than 1000
void printPrimes(const uint64_t* primes_array, size_t n);

 // Runs all the versions once with specified input n and output time for each implementation
void testAll(size_t n);  

// Tests correctness of each implementation by comparing the result array with the first 100 primes (pre-computed)
void testCorrect(void);