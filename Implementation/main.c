#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
#include <time.h>
#include "prim.h"
#include "strconv.h"
#include "impl_handler.h"
#include "help_msg.h"
#include "testing.h"

/* Helper functions */
static double curtime(void);

int main(int argc, char **argv) {

    // Error: not enough arguments were supplied
    if (argc == 1) {
        fprintf(stderr, "Wrong usage for program. Not enough arguments were supplied.\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    int impl_ver = 0;      // Version - Default version is V0 "main implementation"
 
    int bench_iter = 3;       // Default number of benchmarking iterations
    int benchm_opt = 0;          // Benchmarking option was set/not

    size_t n = 1000;        // total number of prime numbers to be calculated; has default value

    int testing = 0;              // check if tests have to be run
    int print_opt = 0;             // check if resulting array has to be printed in the end

    static struct option long_options[] = {              // option struct to define the long option '--help'
            {"help", no_argument, 0, 'h'},               // (different from one-character short option)
            {0}
    };

    // Command Line Interface handler to parse positional command line arguments
    /* Code references: Praktikum GRA 22/23, Blatt 5, T5.1, regula_falsi program + getopt() manpage under 'EXAMPLES' header */
    int opt;
    opterr = 0;     // Overrides default error message from getopt(). 

    // The valid option chars are specified in the 'optstring' of getopt()
    // For option chars 'V' and 'B' the following holds from the getopt() manpage:
    // "If the option has an *optional* argument, it must be written directly after the option character, if present"
    
    while ((opt = getopt_long(argc, argv, "V:B::n:ThP", long_options, NULL)) != -1) {
        switch (opt) {
            case 'V':
                impl_ver = str_to_int(optarg);
                if (!(impl_ver >= 0 && impl_ver <= 3)){       // An invalid version was set
                    fprintf(stderr, "The selected version is not available. Version option 'V' should be between 0 and 3.\n");
                    printAdditionalHelp(argv[0]);
                    return EXIT_FAILURE;
                }
                break;
            case 'B':  
                benchm_opt = 1;
                if (optarg != NULL) {       /* Parameter was passed*/
                    bench_iter = str_to_int(optarg);
                    if (bench_iter == 0 || bench_iter > 1000){      // Reasonable benchmarking lower and upper bounds
                        fprintf(stderr, "Reasonable number of benchmarking iterations 'B' should be in range [1,...,1000].\n");
                        printAdditionalHelp(argv[0]);
                        return -1;
                    }
                } else {       
                    // If the next element in argv does not start with a '-', it means that the next argv element is not an option
                    // and the user has input the parameter to the option 'B' with a whitespace between the option B and the numerical parameter. 
                    if ((optind<argc) && (*argv[optind] != '-')){
                        fprintf(stderr, "Wrong usage of parameter -B: enter parameter without whitespace after the option character.\n");
                        printAdditionalHelp(argv[0]);
                        return EXIT_FAILURE;
                    }
                } 
                break;
            case 'n':
                n = str_to_int(optarg);
                if (n == 0){
                    fprintf(stderr, "Invalid input for '-n' option. Value should be non-negative.\n");
                    printAdditionalHelp(argv[0]);
                    return EXIT_FAILURE;
                } 
                break;
            case 'T':   // Option to execute tests. Set testing to true
                testing = 1;
                break;
            case 'h':
                print_help(argv[0]);
                return 0;   // Program should terminate after printing help message
            case 'P':
                print_opt = 1;   // program should print array before printing
                break;
            default:    /* parsed opt = '?' - indicating missing input argument or invalid option character*/      
                fprintf(stderr, "The option char '%c' has a missing input parameter or is an invalid option.\n", (char) optopt);
                printAdditionalHelp(argv[0]);
                return EXIT_FAILURE;
        }
    }

    // ------------------------------------------ DONE PARSING ------------------------------------------------ // 
    // After parsing, optind is the index in argv of the first argv-element that is not an option.
    // So if argc is greater than optind, there are additional (unnecessary) command line arguments.
    if (optind < argc) {
        fprintf(stderr, "Too many command line input arguments were passed to the program: %s \n", argv[0]);
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // If the testing option 'T' was set execute tests
    if (testing){
        testAll(n);
        printf("\n");
        testCorrect();
        return 0;
    }

    // Allocate memory for the array which is going to store the prime numbers
    uint64_t* prims_array = (uint64_t *) malloc(n * sizeof(uint64_t));
    if (prims_array == NULL) {
        fprintf(stderr, "Could not allocate memory for the 'prims_array' array in the main program.\n");
        return EXIT_FAILURE;
    }

    // Using a function pointer to the chosen implementation to be used at runtime
    size_t (*impl_function_ptr)(size_t n, uint64_t prims_array[n]);

    impl_ver = handle_version(impl_ver, n);     // switch implementation in edge cases

    switch (impl_ver) {
        case 0 : 
            impl_function_ptr = prim;        
            printf("Selected Main Implementation: Sieve\n");
            break;
        case 1: 
            impl_function_ptr = prim_V1;
            printf("Selected Implementation: Trial Division\n");
            break;
        case 2: 
            impl_function_ptr = prim_V2;
            printf("Selected Implementation: Sieve SIMD1\n");
            break;
        case 3: 
            impl_function_ptr = prim_V3;
            printf("Selected Implementation: Sieve SIMD2\n");
            break;
        default: 
            impl_function_ptr = prim;
            printf("No Implementation Selected: Running Default Sieve\n");
            break;
    }
    
    if (benchm_opt) {
        double start = curtime();
        for (int i = 0; i < bench_iter; i++) {
            impl_function_ptr(n, prims_array);
        }
        double end = curtime();
        printf("V%d took %f seconds on average with B=%d iterations.\n", impl_ver,((end - start)/bench_iter), bench_iter);
    } else {
        impl_function_ptr(n, prims_array);
    }

    if (print_opt){
        if (n>1000){
            printf("Print option selected but cannot print array for n > 1000. Output will be unclear.\n");
        } else {
            printPrimes(prims_array, n);
        }
    }
    
    printf("\n");
    printf("Printing the n-th prime number to test correctness.\n");
    printf("prims_array[n] = %lu\n", prims_array[n - 1]);
    free(prims_array);
    return 0;
}

/*************************************  HELPER FUNCTIONS   ***********************************************/

/* Helper function for benchmarking. Using CLOCK_MONOTONIC to get linear time
    Formula to compute correct time references "Praktikum GRA22/23 – Woche 7 - 'Benchmarking' - Code Beispiel"
    Structure of helper function references "Praktikum GRA22/23 - Woche 7 - toupper_simd program"
*/
static double curtime(void) { 
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}






