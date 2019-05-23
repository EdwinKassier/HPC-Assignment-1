cmake_minimum_required(VERSION 3.14)
project(HPC_A1 C)


include_directories(/Edwin/include/mpi/)
set(CMAKE_C_STANDARD 99)

# added -fopenmp
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -fopenmp")

#add_executable(HPC_A1 "QuickSort(Serial).c" "QuickSort(Serial).h" "QuickSort(OMP).c" "QuickSort(OMP).h")
#add_executable(QuickSort(Serial) "QuickSort-S.c")
#add_executable(QuickSort(OMP) "QuickSort-OMP.c")
add_executable(QuickSort-S QuickSort-S.c)
add_executable(QuickSort-OMP QuickSort-OMP.c)


add_executable(PSRS-OMP PSRS-OMP.c)
add_executable(QuickSort-MPI QuickSort-MPI.c)
add_executable(PSRS-MPI PSRS-MPI.c)