# Project in C - Efficient computation of Prime Numbers
### This project was developed by Kerem Colak, Ignat Kharlamov, and Aaron Schulze.
### It is part of the course "Grundlagenpraktikum: Rechnerarchitektur" offered by the Technical University of Munich and in particular by the Chair of Computer Architecture and Parallel Systems.

The goal is to calculate **big prime numbers** and store all of them into an array.
As the computation can become quite expensive for bigger and bigger numbers when using naive/traditional algorithms,
different **implementation alternatives** were implemented and tested against each other in terms of their **speed and efficiency.**

To test the program, clone the repository in a local directory of yours.
A Makefile is provided; the make command will compile the code locally:
```
make
```

The executable is named 'prim'.
To get more information about how to run the executable with the proper options, print the 'help' table:
```
./prim -h
````
Example usages of the program:
```
./prim -n10000000 -V0 -B10
./prim -n10000000 -V2 -B10
```
The first program will calculate the first 100000 existing prime numbers, store them into an array. 
It will also run B=10 benchmarking iterations to calculate the average computation time in seconds when using the Sieve of Eratosthenes (V0: naive implementation)
The second program will keep the same functionality, however, the chosen algorithm (V2) will be faster (depending on the size of the input) since it makes use of SIMD instructions and bit-shifts,
which are used for parallel processing of data.
