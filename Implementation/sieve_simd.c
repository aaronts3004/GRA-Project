#include <emmintrin.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

void vector_modifier(__m128i* array, uint64_t* array_size, __m128i mask, uint64_t number, size_t index);
void printArray(uint64_t* prims, size_t n);

size_t prim_V2(size_t n, uint64_t prims[n]){
    // Sieve Implementation, using 128-bit SIMD vectors
    // Handle two edge cases: n=0, n=1
    if(!n)
        return 0;
    if(n == 1){
        prims[0] = 2;
        return 1;
    }
    /* Approximation for the n-th prime number: pn = n*log(n) + n*log(log(n))
       Sources of formulas: 
        - https://stackoverflow.com/a/4911857
    */
    uint64_t last_prime;
    long double dnum = (long double) n;     // Upcast 'n' into a long double, which can contain a 'uint64_t' number
    last_prime = (uint64_t)((dnum* ((log(dnum)))) + (dnum * ((log(log(dnum)))))); 


    uint64_t SQRT_MAX = sqrt(last_prime)+1; // Upper bound for the Erathosthenes Algorithm

    prims[0] = 2;  // Initialize the first element of the array
    uint64_t number = 3; // Intitialize the fisrt number in our sieve = 3


    /* Allocate an array of 128-bit vectors. Size is the count of numbers (without even ones) / 128 = last_prime/128/2 = last_prime/256 
    =>  Instead of having an array of integers we can pass to a binary representation where each bit represents a number removed or not removed.*/
    uint64_t array_size = last_prime/256+1;   

    __m128i offset = _mm_set_epi32(0,0,0,1);    // Initialize the offset to move in 128-bit vectors
    __m128i* array = (__m128i*) malloc (sizeof(__m128i)*array_size);  

    if (array == NULL){                         // Check malloc
        fprintf(stderr, "Could not allocate memory for primes marking array in prim_V2 function.\n");
        return -1;
    }

    //Initialize each bit with one. Each number is considered as a prime number at the beginning
    for(size_t i = 0; i < array_size; ++i){
        array[i] = _mm_set1_epi8(-1);
    }

    __m128i isprime;
    size_t z = 1;                               // prims index (the value to be returned)
    size_t index = 0;

    // Erathosthenes Algorithm. Entry point
    for(size_t i = 0; index < array_size; ++i, offset = _mm_set_epi32(0,0,0,1)){
        index = i/2;
        if((i%2)!=0) {
            offset = _mm_set_epi32(0,1,0,0);    // Move the highest bit in the lower 64-bit part of the vector to the lowest bit in the highest 64-bit part
        }
            
        //Iterate in the current vector
        for(; offset[0]!=0 || offset[1] !=0; number+=2, offset = offset<<1){
            if(z == n){                         // The last prime number was found
                free(array);
                return z;
            }
            isprime = _mm_and_si128(offset, array[index]);  // Check if the current number is prime
            if(isprime[0] != 0 || isprime[1] != 0){         // The current number is prime  
                prims[z] = number;
                ++z;
                if(number < SQRT_MAX)                       // Modify the remaining vectors only if the current number less than the upper bound
                    vector_modifier(array, &array_size, offset, number, index); // Modify the vectors
            }
        }
    }
    // Should never reach this point
    free(array);
    return z;
}


void vector_modifier(__m128i* array, uint64_t* array_size, __m128i mask, uint64_t number, size_t index){
    //Erathosthenes Algorithm to mark all the prime and non-prime numbers
    size_t x;
    uint64_t copy_number = number*number;   // Start with the power of 2
    index = (copy_number - 2)/256;          // Current index of the initial vector
    // Compute the offset
    x = ((copy_number-2)/2) % 128;
        if(x < 64){
            mask = _mm_set_epi32(0,0,0,1);
            mask = mask << x;
        }
        else{
            mask = _mm_set_epi32(0,1,0,0);
            mask = mask << (x - 64);
        }
    // Mark all the numbers    
    for(size_t i = index; i < *array_size;){
        array[i] = _mm_and_si128(~mask, array[i]);  // Mark the current bit
        i = (copy_number + number*2 - 2)/256;       // Compute the new index
        copy_number+=number*2;                      // Compute the next number
        x = ((copy_number-2)/2) % 128;
        // Compute the offset
        if(x < 64){
            mask = _mm_set_epi32(0,0,0,1);
            mask = mask << x;
        }
        else{
            mask = _mm_set_epi32(0,1,0,0);
            mask = mask << (x - 64);
        }
        
    }

}

