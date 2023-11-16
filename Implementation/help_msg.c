#include "help_msg.h"

const char* usage_msg = 
    "\nUsage: %s [options] to compute the first 'n' prime numbers\n"
    "   or: %s -t to execute predefined test cases and exit\n"
    "   or: %s -h to print additional help information and exit.\n"
    "\nExample usage: %s -n 1000 -V 0 -B5\n";

const char* help_msg = 
    "\n----------------------------- Prime Numbers Computation Program -----------------------------\n"
    "Execution of the program will compute the first 'n' prime numbers and return the number of found primes.\n"
    "The following optional command line arguments can be set for the execution:\n"
    "   -n <number>: total number of primes to be calculated (default 1000)\n"
    "   -V <number>: chooses the algorithm to be run (default V0). Available versions:\n"
    "                0: Sieve, 1: Trial division, 2: Sieve SIMD1, 3: Sieve SIMD2\n"
    "   -B <number>: number of benchmarking iterations (default 3); avg runtime is measured and output\n"
    "   -h: print this help message and exit; the alternative long option is '--help'\n"
    "   -T: execute predefined test cases\n"
    "   -P: print the array of primes; option available only for n < 1000\n"
    "\n"
    "Example usage 1: %s -n1000 -V2\n"
    "Example usage 2: %s -n 1000 -V 0 -B5\n"
    "---------------------------------------------------------------------------------------------\n"
    "\n";

const char* add_help = 
    "For additional help run %s with the option '-h' or '--help'.\n";

void print_usage(const char* progname){
    fprintf(stderr, usage_msg, progname, progname, progname, progname);
}

void print_help(const char* progname) {
    fprintf(stdout, help_msg, progname, progname);
}

void printAdditionalHelp(const char* progname){
    fprintf(stderr, add_help, progname);
}

