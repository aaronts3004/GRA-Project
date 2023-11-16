#include "prim.h"

/* --- Naive implementation of the 'prim' function ---
    This function computes the first n prime numbers by applying a naive primality test on every number until the first 
    n primes have been found. This function is efficient for small values of n (n < 10^6), however after the first 
    1 million primes it's very inefficient, as the inner loop starts again from 3 for every test number.
    Even with mathematical optimizations (checking odd numbers only and checking until the sqrt of each number), the time complexity is constrained.
*/


size_t prim_V1(size_t n, uint64_t prims[n]){
    // Adding the first 2 prime numbers manually considering input edge cases
    prims[0] = 2;
    if (n == 1){        // Input n=1 -> 2 is the only prime
        return 1; 
    }
    prims[1] = 3;       // Input n=2 -> 2,3 are the first two primes
    if (n == 2){
        return 2;
    }

    uint64_t count = 2; // Initialize count at 2 since the first 2 primes have already been found
    // Start testing numbers from 5 and up. Test only odd numbers (test+=2), 
    // since all even numbers (except 2) are non-prime
    for (uint64_t test = 5; count < n; test+=2){
        bool is_prime = true;

        // Test whether the current 'test' number is divisible by any factor. If yes,then it's a composite number
        // Checking only up until sqrt(test) is enough
        uint64_t max_prime = sqrt(test)+1;
        for (uint64_t i = 0; i < count; i++){
            if (prims[i] >= max_prime){
                break;
            }
            if (test % prims[i] == 0){    // Found a factor by which current test number is divisible
                is_prime = false;
                break;
            }
        }
        
        if (is_prime){      
            prims[count++] = test;       // No composite factor was found. Current test number is prime
        }
    }
    return count;
}