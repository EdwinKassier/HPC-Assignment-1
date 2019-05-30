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
MPICC=mpiCC
MPIRUN=mpirun
SRC := src/
BIN := bin/

GCCFLAGS:=-std=c99

#Default
all:
	$(GCC) $(GCCFLAGS) -fopenmp $(SRC)QuickSort-S.c -o $(BIN)QuickSort-S -lm
	$(GCC) $(GCCFLAGS) -fopenmp $(SRC)QuickSort-OMP.c -o $(BIN)QuickSort-OMP -lm
	$(GCC) $(GCCFLAGS) -fopenmp $(SRC)PSRS-OMP.c -o $(BIN)PSRS-OMP -lm
	$(MPICC) -openmpi $(SRC)QuickSort-MPI.c -o $(BIN)QuickSort-MPI -lm
	$(MPICC) -openmpi $(SRC)PSRS-MPI.c -o $(BIN)PSRS-MPI -lm
	@echo Complete
clean:
	rm $(BIN)*

#Run Serial
run1:
	@echo running
	./$(BIN)QuickSort-S

#Run quicksort OMP
run2:
	@echo running
	./$(BIN)QuickSort-OMP

#Run PSRS OMP
run3:
	@echo running
	./$(BIN)PSRS-OMP

#Run all tests at once
run4:
	@echo running
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	$(MPIRUN) -np 2 $(BIN)PSRS-MPI
	
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
	$(MPIRUN) -np 4 $(BIN)PSRS-MPI
    

run5:
	$(MPIRUN) -np 2 $(BIN)QuickSort-MPI 1000
	$(MPIRUN) -np 4 $(BIN)QuickSort-MPI 1000
