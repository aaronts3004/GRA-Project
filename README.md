# Project in C
### This project was developed by Kerem Colak, Ignat Kharlamov, and Aaron Schulze.
The goal is to calculate **big prime numbers** and store all of them into an array.
As the computation can become quite expensive for bigger and bigger numbers when using naive/traditional algorithms,
different **implementation alternatives** were implemented and tested against each other in terms of their **speed and efficiency.**

To test the program, clone the repository in a local directory of yours.
A Makefile is provided; the make command will compile the code locally:
```make```

The executable is named 'prim'.
To get more information about how to run the executable with the proper options, print the 'help' table:
```
./prim -h
````
Example usages of the program:
```
./prim -n100000 -V0 -B10
./prim -n100000 -V2 -B10

