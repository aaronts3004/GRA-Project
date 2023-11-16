#include "strconv.h"
#include <stdint.h>

size_t str_to_int(const char *s){
    // Handle NULL pointer
    if (s == NULL){
        fprintf(stderr, "Fatal error: the received input string is NULL and cannot be converted\n");
        exit(EXIT_FAILURE);
    }

    // Handle negative numbers: increment the 's' pointer to "skip" whitespaces
    while ((*s) == ' '){s++;}
    if (s[0] == '-'){       // Input string starts with '-' (might) symbolize a negative number
        // Next char is not the null-byte and it's a numerical character
        if (s[1] != '\0' && ((s[1] >= 48 && s[1] <= 57))){
            fprintf(stderr, "Invalid input string: \"%s\". Enter non-negative numbers only\n", s);
        } else {
            fprintf(stderr, "Could not convert input string \"%s\" to integer.\n",s);
        }
        fprintf(stderr, "For additional help run the program with '-h' or '--help'.\n");
        exit(EXIT_FAILURE);
    }

    errno = 0;          
    char* endptr;       // String to store the non-numerical part of input (for error-handling)
    size_t num_conv = strtoull(s, &endptr, 0);       // Conversion using C library

    // Check for errors: endptr is still pointing at the first character (no char could be converted)
    // or endptr is NOT pointing at the null-terminating byte (conversion was not completed successfully)
    if (endptr == s || *endptr != '\0'){
        fprintf(stderr, "Could not convert input string \"%s\" to integer.\n"
                        "For additional help run the program with '-h' or '--help'.\n",s);
        exit(EXIT_FAILURE);
    } else if (errno == ERANGE){        /* Number is out of range */
        if (num_conv == ULLONG_MAX){
            fprintf (stderr, "The entered number \"%s\" overflows size_t.\n", s);
        } else {
            fprintf (stderr, "The entered number \"%s\" underflows/overflows size_t.\n", s);
        }
        fprintf (stderr, "For additional help run the program with '-h' or '--help'.\n");
        exit(EXIT_FAILURE);
    } else if (errno != 0){
        fprintf (stderr, "An unnspecified error occurred when converting the string \"%s\".\n", s);
        exit(EXIT_FAILURE);
    } 
    return num_conv;
}

/*--------------------------------------------------------------------------------------------------*/
// Test cases for string converter
static size_t test_str_conv(void) {

    char* s;
    size_t num = 0;
    size_t exp = 0;

    s = "";      // empty string
    num = str_to_int(s);
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %lu\n", num);
        return EXIT_FAILURE;
    }

    s = "\0";       // null-terminated string
    num = str_to_int(s);
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %lu\n", num);
        return EXIT_FAILURE;
    }

    s = "-1";           // Correct input, negative number
    num = str_to_int(s);
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %lu\n", num);
        return EXIT_FAILURE;
    }

    s = "       -1234\0";   // Invalid negative number with leading whitespace
    num = str_to_int(s);
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %lu\n", num);
        return EXIT_FAILURE;
    }

    s = "-1234\0";      // Correct input, negative number  
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %lu\n", num);
        return EXIT_FAILURE;
    }

    s = "0x1234\0";           // hex
    num = str_to_int(s);
    exp = 4660;
    if (num != exp){
        printf("Failed test: input string is %s\n", s);
        printf("Expected out: %lu - but was: %lu\n", exp, num);
    }
   
    s = "0x1234P\0";              // invalid hex because of P
    num = str_to_int(s);
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %zu\n", num);
        return EXIT_FAILURE;
    }

    s = "01234\0";            // octal
    num = str_to_int(s);
    exp = 668;
    if (num != exp){
        printf("Failed test: input string is %s\n", s);
        printf("Expected out: %lu - but was: %lu\n", exp, num);
        return -1;
    }

    s = "1234a\0";            // char inside number
    num = str_to_int(s);
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %lu\n", num);
        return EXIT_FAILURE;
    }

    s = "123a4\0";          // char inside number
    num = str_to_int(s);
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %lu\n", num);
        return EXIT_FAILURE;
    }

    s = "13245426235772546325262626472326424512531314124132435415236361462346162346135413\0";       // big number
    num = str_to_int(s);
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %lu\n", num);
        return EXIT_FAILURE;
    }

    // 2^64 - 1: correct input
    /*s = "18446744073709551615\0";
    num = str_to_int(s);
    uint64_t max = 18446744073709551615;
    if (num != max){
        printf("Failed test: input string is %s\n", s);
        printf("Expected out: %lu - but was: %lu\n", exp, num);
        return -1;
    }*/
    
    // 2^64 
    s = "18446744073709551616\0";
    num = str_to_int(s);
    if (num != -1){
        printf("Failed test: input string is %s\n", s);
        printf("Expected return -1 but was %lu\n", num);
        return EXIT_FAILURE;
    }

    s = "hello\0";            // non-numerical
    num = str_to_int(s);
    exp = -1;
    if (num != exp){
        printf("Failed test: input string is %s\n", s);
        printf("Expected out: %lu - but was: %lu\n", exp, num);
        return -1;
    }

    s = "0\0";              // 0 is valid input
    num = str_to_int(s);
    exp = 0;
    if (num != exp){
        printf("Failed test: input string is %s\n", s);
        printf("Expected out: %lu - but was: %lu\n", exp, num);
        return -1;
    }

    s = "0000000000000\0";  // 0 as valid input
    num = str_to_int(s);
    exp = 0;
    if (num != exp){
        printf("Failed test: input string is %s\n", s);
        printf("Expected out: %lu - but was: %lu\n", exp, num);
        return -1;
    }

    printf("Tests successful!!!\n");
    return 0;
}
