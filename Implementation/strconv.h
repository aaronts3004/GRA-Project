#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

/* Helper function to convert input strings to size_t variables. 
    Code references: 
        - "Praktikum GRA 22/23, Blatt 6, T6.1, stringtonum program"
        - "strtoul" linux manual page, as well as 'EXAMPLE' paragraph of the "strtol" manual page
    Input: const string 's' containing the literal representation of a number
            which can be in the range value [0,...,2^64-1]
            in base decimal, hexadecimal or octal.
    Invalid inputs: negative numbers, numbers greater than 2^64-1 (range of size_t),
                    numbers containing alphabetic characters, base errors
*/
size_t str_to_int(const char*);


// Inner testing function
static size_t test_str_conv(void);  