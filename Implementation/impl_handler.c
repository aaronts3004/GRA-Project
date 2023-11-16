#include "impl_handler.h"

int handle_version(int impl_ver, size_t n){
    // user chose V1 and n>10^6: change to main
    if (impl_ver == 1){
        if (n > 10000000){
            fprintf(stdout, "Running trial division with input n > 10^6 is not efficiently feasible\n"
            "Switch to main implementation necessary\n");
            return 0;       // returning 0 for V0 as final version
        }
    }
    
    // Checking if avx2 is supported on the CPU so that implementation V3 can be run
    if (impl_ver == 3){
        if (__builtin_cpu_supports("avx2")) {    
            fprintf(stdout, "AVX2 ISA extension supported on currently running CPU.\n");         
        } else { 
            // In case of no support, change the implementation to the main one 
            fprintf(stdout,"AVX2 ISA extensions not supported on running CPU. Switch to main implementation necessary\n");
            return 0;      
        }
    }
    // Edge cases handled: return the same version to the main program
    return impl_ver;      
}

