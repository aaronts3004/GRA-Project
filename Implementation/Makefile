
# C Compiler setting
CC=gcc
# Compiler options for main program
CFLAGS = -O0 -Wall -Wextra
# Specific compiler flags for algorithms					
CFLAGS_ALG = -O3 -Wall -Wextra -lm
# Subdirectory for all the object files
OBJ_DIR = ./obj

# List of the object files necessary to build the main program
MAIN_OBJ = $(OBJ_DIR)/help_msg.o $(OBJ_DIR)/impl_handler.o $(OBJ_DIR)/strconv.o $(OBJ_DIR)/testing.o $(OBJ_DIR)/algorithms.o

# List of the algorithms' object files
ALGO_OBJ = $(OBJ_DIR)/sieve.o $(OBJ_DIR)/trial.o $(OBJ_DIR)/sieve_simd.o $(OBJ_DIR)/sieve_simd2.o

.PHONY: all clean			# 'all' and 'clean' are phony/dummy targets
all: prim					# Main target to be built

# Link all the object files together with main.c
prim: main.c $(MAIN_OBJ) prim.h
	$(CC) -o prim main.c $(MAIN_OBJ) -lm 

# Compile the source files with helper functions into separate object files
$(OBJ_DIR)/help_msg.o: help_msg.c help_msg.h
	$(CC) -c help_msg.c -o $(OBJ_DIR)/help_msg.o

$(OBJ_DIR)/impl_handler.o: impl_handler.c impl_handler.h
	$(CC) -c impl_handler.c -o $(OBJ_DIR)/impl_handler.o

$(OBJ_DIR)/strconv.o: strconv.c strconv.h 
	$(CC) -c strconv.c -o $(OBJ_DIR)/strconv.o

$(OBJ_DIR)/testing.o: testing.c testing.h $(OBJ_DIR)/algorithms.o prim.h 
	$(CC) -c testing.c -o $(OBJ_DIR)/testing.o -lm

# Link the object files of the algorithms' source files all together into one object file
$(OBJ_DIR)/algorithms.o: $(ALGO_OBJ) 
	ld -r $(ALGO_OBJ) -o $(OBJ_DIR)/algorithms.o 

# Compile the different algortihms' source files into separate object files
$(OBJ_DIR)/sieve.o: sieve.c prim.h
	$(CC) $(CFLAGS_ALG) -c sieve.c -o $(OBJ_DIR)/sieve.o 
$(OBJ_DIR)/trial.o: trial.c prim.h
	$(CC) $(CFLAGS_ALG) -c trial.c -o $(OBJ_DIR)/trial.o
$(OBJ_DIR)/sieve_simd.o: sieve_simd.c prim.h
	$(CC) $(CFLAGS_ALG) -c sieve_simd.c -o $(OBJ_DIR)/sieve_simd.o
$(OBJ_DIR)/sieve_simd2.o: sieve_simd2.c prim.h
	$(CC) $(CFLAGS_ALG) -c sieve_simd2.c -o $(OBJ_DIR)/sieve_simd2.o -mavx2

clean:
	rm -f prim $(MAIN_OBJ) $(ALGO_OBJ)