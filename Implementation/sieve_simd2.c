#include "prim.h"
#include <immintrin.h>


size_t prim_V3(size_t n, uint64_t prims[n]){
    uint64_t max_prime;
    if (n >= 6){
        long double dnum = (long double) n;     
        max_prime = (uint64_t)((dnum* ((log(dnum)))) + (dnum * ((log(log(dnum)))))); 
    } else {
        uint64_t first_six_primes[6] = {2, 3, 5, 7, 11};        // Add the first 5 primes manually if n < 6
        for (size_t i = 0; i < n; i++){
            prims[i] = first_six_primes[i];
        }
        return n;
    }

    // Sieve Implementation, using SIMD
    uint64_t z = 0;     // Counter: is incremented each time a new prime number is found

    // Allocate an array of 'max_prime' chars
    uint8_t* sieve_array;
    sieve_array = malloc(max_prime*sizeof(uint8_t));

    // Check malloc
    if (sieve_array == NULL){
        fprintf(stderr, "Could not allocate memory for primes marking sieve_array in prim_V3 function.\n");
        return -1;
    }

    // At first, arr[cur_prime] = 1 (prime true) for all the potential primes
    for(size_t cur_prime = 0; cur_prime < max_prime; cur_prime++){
        sieve_array[cur_prime] = 1;
    }

    prims[z++] = 2;
    // Erathosthenes Algorithm to mark all the prime and non-prime numbers
    for(uint64_t cur_prime = 3; cur_prime < max_prime; cur_prime+=2){
        if(sieve_array[cur_prime] == 1){     // Prime number was found
            if (z == n){                
                break;
            }
            prims[z] = cur_prime;           // Write the current prime into 'prims' 
            z++;

            // SIMD
            // Initialize start index and mark this first multiple in the sieve_array of numbers/in the sieve
            uint64_t j = cur_prime*cur_prime;

            if (j > max_prime){         // All the smaller multiples will have been already crossed
                continue;
            }

            sieve_array[j] = 0;         // First multiple set as non-prime
            // Pre-compute the first four multiples of the found prime
            uint64_t mul1 = cur_prime*1;
            uint64_t mul2 = cur_prime*2;
            uint64_t mul3 = cur_prime*3;
            uint64_t mul4 = cur_prime*4;

            // Load base into vector
            __m256i base = _mm256_set_epi64x(j,j,j,j);

            uint64_t f1,f2,f3,f4;
            uint64_t offset;

            for (size_t counter = 0; ; counter +=4){
                offset = cur_prime*counter;
                f1 = offset + mul1;
                f2 = offset + mul2;
                f3 = offset + mul3;
                f4 = offset + mul4;
           
                // Fill a 256bit vector with the 4 64bit factors/multiples
                 __m256i factors = _mm256_set_epi64x(f4,f3,f2,f1);

                // Compute the indices which are mulitples of the current prime using a SIMD vector addition
                __m256i res = _mm256_add_epi64(base, factors);
         
                // Read the result and update the entries in the sieve_array for the computed indices
                uint64_t* indices = (uint64_t *)&res;
                
                if (indices[0] >= max_prime){
                    break;
                } else {
                    sieve_array[indices[0]] = 'M';
                }
                if (indices[1] >= max_prime){
                    break;
                } else {
                    sieve_array[indices[1]] = 'M';
                }
                if (indices[2] >= max_prime){
                    break;
                } else {
                    sieve_array[indices[2]] = 'M';
                }
                if (indices[3] >= max_prime){
                    break;
                } else {
                    sieve_array[indices[3]] = 'M';
                }
            }
        }
    }
    free(sieve_array);
    return z;
}
