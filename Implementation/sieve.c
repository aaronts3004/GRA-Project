#include "prim.h"

/* Main implementation V0: uses an optimized version of the Sieve of Eratosthenes */

size_t prim(size_t n, uint64_t prims[n]){

    /* Approximation for the n-th prime number: p(n) = n*log(n) + n*log(log(n)) --- Works only for n >= 6 (!)
       Sources of formulas: 
        - https://stackoverflow.com/a/4911857
    */

    uint64_t max_prime;
    if (n >= 6){
        long double double_num = (long double) n;     // Upcast 'n' into a long double, which can contain a 'uint64_t' number
        max_prime = (uint64_t)((double_num* ((log(double_num)))) + (double_num * ((log(log(double_num)))))); 
    } else {
        uint64_t first_six_primes[6] = {2, 3, 5, 7, 11};        // Add the first 5 primes manually if n < 6
        for (size_t i = 0; i < n; i++){
            prims[i] = first_six_primes[i];
        }
        return n;
    }
    uint64_t MAX = sqrt(max_prime)+1;      

    // Sieve Implementation 
    uint64_t z = 0;     // Counter: is incremented each time a new prime number is found
    
    // Char array to mark all the non-prime numbers up until 'last_prim' through Erathosthenes' Algorithm
    // Marking: if arr[i] = 0 then arr[i] is a potential prime. Else arr[i] = 1 and i-th number is not prime (true=eliminated)
    char* sieve_array;
    
    // Initializing the array to 0 through a call to calloc to avoid looping through the array
    sieve_array = (char*) calloc((max_prime/2) + 1, sizeof(char));

    // Check malloc
    if (sieve_array == NULL){
        fprintf(stderr, "Could not allocate memory for primes marking array in prim_V0 function.\n");
        return -1;
    }

    // Erathosthenes Algorithm to mark all the prime and non-prime numbers
    prims[z++] = 2;
    for(uint64_t i = 3; i < max_prime; i+=2){
        if (!(sieve_array[i/2])) {            // Prime number was found
            if (z == n) {              // All primes found              
                break;
            }
            prims[z++] = i;           // Write the current prime into 'prims'
    
            // Inner loop to mark all the multiples of the current prime as 'non-prime'
            if (i < MAX) {
                // Start eliminatig from 'i*i' since any smaller number will have been already eliminated
                // Increment 'j' in steps of 2 since even numbers are not considered
                for(uint64_t j = (i*i); j < max_prime; j+=(i*2)){
                    sieve_array[j/2] = 1;
                }
            }
        }
    }
    free(sieve_array);
    return z;
}
