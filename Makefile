#
#
# HPC Assignment 1 Makefile
#
# Edwin Kassier(KSSRUB001)
# Updated:May 20 2019
#
#

#variables
GCC=gcc
MPICC=mpicc
MPIRUN=mpirun
SRC := src/
BIN := bin/

#Default
all:
	$(GCC) -fopenmp $(SRC)QuickSort-S.c -o $(BIN)QuickSort-S -lm
	$(GCC) -fopenmp $(SRC)QuickSort-OMP.c -o $(BIN)QuickSort-OMP -lm
	$(GCC) -fopenmp $(SRC)PSRS-OMP.c -o $(BIN)PSRS-OMP -lm
	$(MPICC) -openmpi $(SRC)QuickSort-MPI.c -o $(BIN)QuickSort-MPI -lm
	$(MPICC) -openmpi $(SRC)PSRS-MPI.c -o $(BIN)PSRS-MPI -lm
	@echo Complete
clean:
	rm $(BIN)

#Run all tests at once
run1:
	@echo running
	./$(BIN)QuickSort-S
	./$(BIN)QuickSort-OMP
	./$(BIN)PSRS-OMP

#Run all tests at once
run2:
	@echo running
	./$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	./$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	./$(MPIRUN) -np 2 $(BIN)QuickSort-MPI
	./$(MPIRUN) -np 4 $(BIN)QuickSort-MPI