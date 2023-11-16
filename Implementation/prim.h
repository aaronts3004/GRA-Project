#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

size_t prim(size_t n, uint64_t prims[n]);       // sieve
size_t prim_V1(size_t n, uint64_t prims[n]);    // trial division
size_t prim_V2(size_t n, uint64_t prims[n]);    // sieve_simd
size_t prim_V3(size_t n, uint64_t prims[n]);    // sieve_simd2
