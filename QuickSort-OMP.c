/*	Homework 3
	Mattias Cederlund
	mcede@kth.se
    Quicksort using OpenMP
    Usage with gcc (version 4.4 or higher required):
	
	COMPILE
    gcc -fopenmp -o quicksort quicksort.c
	
	RUN
    ./quicksort [size] [numWorkers]
	Parameter [size] is size of array
	Parameter [numWorkers] is number of worker threads
	If no parameters are used the program will run with 
	default parameters [size] = 5000000 and [numWorkers] = 12
	Tests ran at u-shell.csc because load was so high at ict-servers
	that it was causing big varieties in time.

	
	Conclusions:
	When increasing number of threads we do not get a linear increase
	in preformance because a large part of the execution is not done 
	in parallel. The first execution of Qsort only uses one thread.
	At recursion depth 1 we use two threads, and at depth 2 we use four.
	A large part of execution is done before we have enough tasks to use
	all available threads.
*/

#include <stdio.h>
#include <omp.h>

#define MAXSIZE 5000000 /* Maximum size of array*/
#define MAXWORKERS 12 /* Maximum amount of worker threads */

int size = MAXSIZE;
int vector[MAXSIZE];
double start_time, end_time; /* start and end times */
int numWorkers;

/* Regular quiksort algorithm, with the only exception that
 * the recursive step is done in parallel with openmp tasks
 */
void Qsort(int first, int last) {
    int pivot, i_pivot, temp, left, right;
    if (first >= last) return; // no need to sort
    // otherwise select a pivot
    i_pivot = (first + last) / 2;
    pivot = vector[i_pivot];
    left = first;
    right = last;
    while (left <= right) {
        if (vector[left] > pivot) { // swap left element with right element
            temp = vector[left];
            vector[left] = vector[right];
            vector[right] = temp;
            if (right == i_pivot) {
                i_pivot = left;
            }
            right--;
        }
        else {
            left++;
        }
    }
    // place the pivot in its place (i.e. swap with right element)
    temp = vector[right];
    vector[right] = pivot;
    vector[i_pivot] = temp;
    // sort two sublists in parallel;

    /* The recursive steps in quicksort execution is implemented as separate tasks */
#pragma omp task
    Qsort(first, (right - 1));
#pragma omp task
    Qsort((right + 1), last);

}

int main(int argc, char *argv[]) {
    int j;
    FILE *out;
    out = fopen("PSRS-OMP.txt", "w+");

//1000
    fprintf(out,"Time of execution: 1000 inputs");
    fprintf(out,".\n");
    for (j = 0; j < 10; j++) {
        FILE *file = fopen("C:\\Users\\Edwin\\CLionProjects\\HPC-A1\\Output(1000).txt", "r");
        size = 1000;
//Number of threads
        numWorkers = 8;
        int i = 0;
        int num;
        while (fscanf(file, "%d", &num) > 0) {
            vector[i] = (int) num;
            i++;
        }
        fclose(file);

        omp_set_num_threads(numWorkers);
        start_time = omp_get_wtime();
        /* call Qsort  */
        /* The sorting is done in a parallel region */
#pragma omp parallel
        {
            /* But we only want to sort the list once, so the first call
             * to Qsort is done only once thanks to the single parameter
             */
#pragma omp single
            Qsort(0, (size - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        fprintf(out,"%g \n", end_time - start_time);
    }

    //10000
    fprintf(out,"Time of execution: 10000 inputs");
    fprintf(out,".\n");
    for (j = 0; j < 10; j++) {
        FILE *file = fopen("C:\\Users\\Edwin\\CLionProjects\\HPC-A1\\Output(10000).txt", "r");
        size = 10000;
//Number of threads
        numWorkers = 8;
        int i = 0;
        int num;
        while (fscanf(file, "%d", &num) > 0) {
            vector[i] = (int) num;
            i++;
        }
        fclose(file);

        omp_set_num_threads(numWorkers);
        start_time = omp_get_wtime();
        /* call Qsort  */
        /* The sorting is done in a parallel region */
#pragma omp parallel
        {
            /* But we only want to sort the list once, so the first call
             * to Qsort is done only once thanks to the single parameter
             */
#pragma omp single
            Qsort(0, (size - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        fprintf(out,"%g \n", end_time - start_time);
    }

    //100000
    fprintf(out,"Time of execution: 10000 inputs");
    fprintf(out,".\n");
    for (j = 0; j < 10; j++) {
        FILE *file = fopen("C:\\Users\\Edwin\\CLionProjects\\HPC-A1\\Output(100000).txt", "r");
        size = 100000;
//Number of threads
        numWorkers = 8;
        int i = 0;
        int num;
        while (fscanf(file, "%d", &num) > 0) {
            vector[i] = (int) num;
            i++;
        }
        fclose(file);

        omp_set_num_threads(numWorkers);
        start_time = omp_get_wtime();
        /* call Qsort  */
        /* The sorting is done in a parallel region */
#pragma omp parallel
        {
            /* But we only want to sort the list once, so the first call
             * to Qsort is done only once thanks to the single parameter
             */
#pragma omp single
            Qsort(0, (size - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        fprintf(out,"%g \n", end_time - start_time);
    }

    //1000000
    printf("Time of execution: 1000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        FILE *file = fopen("C:\\Users\\Edwin\\CLionProjects\\HPC-A1\\Output(1000000).txt", "r");
        size = 1000000;
//Number of threads
        numWorkers = 8;
        int i = 0;
        int num;
        while (fscanf(file, "%d", &num) > 0) {
            vector[i] = (int) num;
            i++;
        }
        fclose(file);

        omp_set_num_threads(numWorkers);
        start_time = omp_get_wtime();
        /* call Qsort  */
        /* The sorting is done in a parallel region */
#pragma omp parallel
        {
            /* But we only want to sort the list once, so the first call
             * to Qsort is done only once thanks to the single parameter
             */
#pragma omp single
            Qsort(0, (size - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        fprintf(out,"%g \n", end_time - start_time);
    }

    //10000000
    fprintf(out,"Time of execution: 10000000 inputs");
    fprintf(out,".\n");
    for (j = 0; j < 10; j++) {
        FILE *file = fopen("C:\\Users\\Edwin\\CLionProjects\\HPC-A1\\Output(10000000).txt", "r");
        size = 10000000;
//Number of threads
        numWorkers = 8;
        int i = 0;
        int num;
        while (fscanf(file, "%d", &num) > 0) {
            vector[i] = (int) num;
            i++;
        }
        fclose(file);

        omp_set_num_threads(numWorkers);
        start_time = omp_get_wtime();
        /* call Qsort  */
        /* The sorting is done in a parallel region */
#pragma omp parallel
        {
            /* But we only want to sort the list once, so the first call
             * to Qsort is done only once thanks to the single parameter
             */
#pragma omp single
            Qsort(0, (size - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        fprintf(out,"%g \n", end_time - start_time);
    }


    return 0;
}