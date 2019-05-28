//Retrieved by Edwin Kassier from https://github.com/mattec92/KTH/blob/master/ID1217%20-%20Concurrent%20Programming/Lab%203%20-%20OpenMP/quicksort.c

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define MAXSIZE 50000000 /* Maximum size of array*/
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
    int c, num;

    printf("QuickSort OMP\n");
    //Running with two threads
    printf("2 threads execution");
    printf(".\n");
    //Number of threads
    numWorkers = 2;

    //1000
    printf("Time of execution: 1000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(1000);

        //Generating random number list
        for (c = 0; c < 1000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (1000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        printf("%g \n", end_time - start_time);
    }

    //10000
    printf("Time of execution: 10000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000);

        //Generating random number list
        for (c = 0; c < 10000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (10000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */
        printf("%g \n", end_time - start_time);
    }

    //100000
    printf("Time of execution: 100000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(100000);

        //Generating random number list
        for (c = 0; c < 100000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (100000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        printf("%g \n", end_time - start_time);
    }

    //1000000
    printf("Time of execution: 1000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(1000000);

        //Generating random number list
        for (c = 0; c < 1000000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (1000000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */


        printf("%g \n", end_time - start_time);
    }

    //10000000
    printf("Time of execution: 10000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000000);

        //Generating random number list
        for (c = 0; c < 10000000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (10000000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        printf("%g \n", end_time - start_time);
    }

    printf("\n");


    //Running with four threads
    printf("4 threads execution");
    printf(".\n");
    //Number of threads
    numWorkers = 4;


//1000
    printf("Time of execution: 1000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(1000);

        //Generating random number list
        for (c = 0; c < 1000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (1000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */


        printf("%g \n", end_time - start_time);
    }

    //10000
    printf("Time of execution: 10000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000);

        //Generating random number list
        for (c = 0; c < 10000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (10000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        printf("%g \n", end_time - start_time);
    }

    //100000
    printf("Time of execution: 100000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(100000);

        //Generating random number list
        for (c = 0; c < 100000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (100000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        printf("%g \n", end_time - start_time);
    }

    //1000000
    printf("Time of execution: 1000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(1000000);

        //Generating random number list
        for (c = 0; c < 1000000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (1000000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */


        printf("%g \n", end_time - start_time);
    }

    //10000000
    printf("Time of execution: 10000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000000);

        //Generating random number list
        for (c = 0; c < 10000000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (10000000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        printf("%g \n", end_time - start_time);
    }

    printf("\n");
    //Running with eight threads
    printf("8 threads execution");
    printf(".\n");
    //Number of threads
    numWorkers = 8;
//1000
    printf("Time of execution: 1000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(1000);

        //Generating random number list
        for (c = 0; c < 1000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (1000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        printf("%g \n", end_time - start_time);
    }

    //10000
    printf("Time of execution: 10000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000);

        //Generating random number list
        for (c = 0; c < 10000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (10000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */


        printf("%g \n", end_time - start_time);
    }

    //100000
    printf("Time of execution: 100000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(100000);

        //Generating random number list
        for (c = 0; c < 100000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (100000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */

        printf("%g \n", end_time - start_time);
    }

    //1000000
    printf("Time of execution: 1000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(1000000);

        //Generating random number list
        for (c = 0; c < 1000000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (1000000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */


        printf("%g \n", end_time - start_time);
    }

    //10000000
    printf("Time of execution: 10000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000000);

        //Generating random number list
        for (c = 0; c < 10000000; c++) {
            num = rand() % 5000 + 1;
            vector[c] = num;
        }

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
            Qsort(0, (10000000 - 1));
        }
        end_time = omp_get_wtime();
        /* check if the vector is sorted and print the sorted vector */


        printf("%g \n", end_time - start_time);
    }




    return 0;
}