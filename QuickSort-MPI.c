//
// Retrieved by Edwin Kassier from http://www.macs.hw.ac.uk/~hwloidl/Courses/F21DP/srcs/qsort.c
//

/*
   qsort.c - Parallel sorting algorithm based on quicksort

   compile: mpicc -Wall -o qsort qsort.c
   run:     mpirun -np num_procs qsort in_file out_file
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

double startTime;

/* swap entries in array v at positions i and j; used by quicksort */
static inline /* this improves performance; Exercise: by how much? */
void swap(int * v, int i, int j)
{
    int t = v[i];
    v[i] = v[j];
    v[j] = t;
}

/* (quick) sort slice of array v; slice starts at s and is of length n */
void quicksort(int * v, int s, int n)
{
    int x, p, i;
    // base case?
    if (n <= 1)
        return;
    // pick pivot and swap with first element
    x = v[s + n/2];
    swap(v, s, s + n/2);
    // partition slice starting at s+1
    p = s;
    for (i = s+1; i < s+n; i++)
        if (v[i] < x) {
            p++;
            swap(v, i, p);
        }
    // swap pivot into place
    swap(v, s, p);
    // recurse into partition
    quicksort(v, s, p-s);
    quicksort(v, p+1, s+n-p-1);
}


/* merge two sorted arrays v1, v2 of lengths n1, n2, respectively */
int * merge(int * v1, int n1, int * v2, int n2)
{
    int * result = (int *)malloc((n1 + n2) * sizeof(int));
    int i = 0;
    int j = 0;
    int k;
    for (k = 0; k < n1 + n2; k++) {
        if (i >= n1) {
            result[k] = v2[j];
            j++;
        }
        else if (j >= n2) {
            result[k] = v1[i];
            i++;
        }
        else if (v1[i] < v2[j]) { // indices in bounds as i < n1 && j < n2
            result[k] = v1[i];
            i++;
        }
        else { // v2[j] <= v1[i]
            result[k] = v2[j];
            j++;
        }
    }
    return result;
}

int main(int argc, char ** argv)
{
    int n;
    int * data = NULL;
    int cs, s;
    int * chunk;
    int o;
    int * other;
    int step;
    int p, id;
    MPI_Status status;
    double elapsed_time;
    int i;
    int num;
    int c;
    int j;

    FILE *out;
    out = fopen("QS-OMP.txt", "w+");


    printf("Time of execution: 1000 inputs, 4 processes");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &p);
        MPI_Comm_rank(MPI_COMM_WORLD, &id);

        //seed random number generator
        srand(1000);
        n = 1000;
        //Initialise chunk size
        cs = (n % p != 0) ? n / p + 1 : n / p;
        //initialise array
        data = (int *) malloc(p * cs * sizeof(int));

        //Generating random number list
        for (c = 0; c < 1000; c++) {
            num = rand() % 5000 + 1;
            data[c] = num;
        }

        // start the timer
        MPI_Barrier(MPI_COMM_WORLD);
        elapsed_time = -MPI_Wtime();

        // broadcast size
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // compute chunk size
        c = (n % p != 0) ? n / p + 1 : n / p;

        // scatter data
        chunk = (int *) malloc(c * sizeof(int));
        MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
        free(data);
        data = NULL;

        // compute size of own chunk and sort it
        s = (n >= c * (id + 1)) ? c : n - c * id;
        quicksort(chunk, 0, s);

        // up to log_2 p merge steps
        for (step = 1; step < p; step = 2 * step) {
            if (id % (2 * step) != 0) {
                // id is no multiple of 2*step: send chunk to id-step and exit loop
                MPI_Send(chunk, s, MPI_INT, id - step, 0, MPI_COMM_WORLD);
                break;
            }
            // id is multiple of 2*step: merge in chunk from id+step (if it exists)
            if (id + step < p) {
                // compute size of chunk to be received
                o = (n >= c * (id + 2 * step)) ? c * step : n - c * (id + step);
                // receive other chunk
                other = (int *) malloc(o * sizeof(int));
                MPI_Recv(other, o, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
                // merge and free memory
                data = merge(chunk, s, other, o);
                free(chunk);
                free(other);
                chunk = data;
                s = s + o;
            }
        }

        // stop the timer
        elapsed_time += MPI_Wtime();

        //printf(out, "%f", elapsed_time);
        printf("Quicksort %d ints on %d procs: %f secs\n", n, p, elapsed_time);

        MPI_Finalize();
    }

    printf(".\n");
    printf("Time of execution: 10000 inputs, 4 processes");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &p);
        MPI_Comm_rank(MPI_COMM_WORLD, &id);

        //seed random number generator
        srand(10000);
        n = 10000;
        //Initialise chunk size
        cs = (n % p != 0) ? n / p + 1 : n / p;
        //initialise array
        data = (int *) malloc(p * cs * sizeof(int));

        //Generating random number list
        for (c = 0; c < 10000; c++) {
            num = rand() % 5000 + 1;
            data[c] = num;
        }

        // start the timer
        MPI_Barrier(MPI_COMM_WORLD);
        elapsed_time = -MPI_Wtime();

        // broadcast size
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // compute chunk size
        c = (n % p != 0) ? n / p + 1 : n / p;

        // scatter data
        chunk = (int *) malloc(c * sizeof(int));
        MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
        free(data);
        data = NULL;

        // compute size of own chunk and sort it
        s = (n >= c * (id + 1)) ? c : n - c * id;
        quicksort(chunk, 0, s);

        // up to log_2 p merge steps
        for (step = 1; step < p; step = 2 * step) {
            if (id % (2 * step) != 0) {
                // id is no multiple of 2*step: send chunk to id-step and exit loop
                MPI_Send(chunk, s, MPI_INT, id - step, 0, MPI_COMM_WORLD);
                break;
            }
            // id is multiple of 2*step: merge in chunk from id+step (if it exists)
            if (id + step < p) {
                // compute size of chunk to be received
                o = (n >= c * (id + 2 * step)) ? c * step : n - c * (id + step);
                // receive other chunk
                other = (int *) malloc(o * sizeof(int));
                MPI_Recv(other, o, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
                // merge and free memory
                data = merge(chunk, s, other, o);
                free(chunk);
                free(other);
                chunk = data;
                s = s + o;
            }
        }

        // stop the timer
        elapsed_time += MPI_Wtime();

        printf("%f", elapsed_time);
        //printf("Quicksort %d ints on %d procs: %f secs\n", n, p, elapsed_time);

        MPI_Finalize();
    }

    printf(".\n");
    printf("Time of execution: 100000 inputs, 4 processes");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &p);
        MPI_Comm_rank(MPI_COMM_WORLD, &id);

        //seed random number generator
        srand(100000);
        n = 100000;
        //Initialise chunk size
        cs = (n % p != 0) ? n / p + 1 : n / p;
        //initialise array
        data = (int *) malloc(p * cs * sizeof(int));

        //Generating random number list
        for (c = 0; c < 100000; c++) {
            num = rand() % 5000 + 1;
            data[c] = num;
        }

        // start the timer
        MPI_Barrier(MPI_COMM_WORLD);
        elapsed_time = -MPI_Wtime();

        // broadcast size
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // compute chunk size
        c = (n % p != 0) ? n / p + 1 : n / p;

        // scatter data
        chunk = (int *) malloc(c * sizeof(int));
        MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
        free(data);
        data = NULL;

        // compute size of own chunk and sort it
        s = (n >= c * (id + 1)) ? c : n - c * id;
        quicksort(chunk, 0, s);

        // up to log_2 p merge steps
        for (step = 1; step < p; step = 2 * step) {
            if (id % (2 * step) != 0) {
                // id is no multiple of 2*step: send chunk to id-step and exit loop
                MPI_Send(chunk, s, MPI_INT, id - step, 0, MPI_COMM_WORLD);
                break;
            }
            // id is multiple of 2*step: merge in chunk from id+step (if it exists)
            if (id + step < p) {
                // compute size of chunk to be received
                o = (n >= c * (id + 2 * step)) ? c * step : n - c * (id + step);
                // receive other chunk
                other = (int *) malloc(o * sizeof(int));
                MPI_Recv(other, o, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
                // merge and free memory
                data = merge(chunk, s, other, o);
                free(chunk);
                free(other);
                chunk = data;
                s = s + o;
            }
        }

        // stop the timer
        elapsed_time += MPI_Wtime();

        printf("%f", elapsed_time);
        //printf("Quicksort %d ints on %d procs: %f secs\n", n, p, elapsed_time);

        MPI_Finalize();
    }

    printf(".\n");
    printf("Time of execution: 1000000 inputs, 4 processes");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &p);
        MPI_Comm_rank(MPI_COMM_WORLD, &id);

        //seed random number generator
        srand(1000000);
        n = 1000000;
        //Initialise chunk size
        cs = (n % p != 0) ? n / p + 1 : n / p;
        //initialise array
        data = (int *) malloc(p * cs * sizeof(int));

        //Generating random number list
        for (c = 0; c < 1000000; c++) {
            num = rand() % 5000 + 1;
            data[c] = num;
        }

        // start the timer
        MPI_Barrier(MPI_COMM_WORLD);
        elapsed_time = -MPI_Wtime();

        // broadcast size
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // compute chunk size
        c = (n % p != 0) ? n / p + 1 : n / p;

        // scatter data
        chunk = (int *) malloc(c * sizeof(int));
        MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
        free(data);
        data = NULL;

        // compute size of own chunk and sort it
        s = (n >= c * (id + 1)) ? c : n - c * id;
        quicksort(chunk, 0, s);

        // up to log_2 p merge steps
        for (step = 1; step < p; step = 2 * step) {
            if (id % (2 * step) != 0) {
                // id is no multiple of 2*step: send chunk to id-step and exit loop
                MPI_Send(chunk, s, MPI_INT, id - step, 0, MPI_COMM_WORLD);
                break;
            }
            // id is multiple of 2*step: merge in chunk from id+step (if it exists)
            if (id + step < p) {
                // compute size of chunk to be received
                o = (n >= c * (id + 2 * step)) ? c * step : n - c * (id + step);
                // receive other chunk
                other = (int *) malloc(o * sizeof(int));
                MPI_Recv(other, o, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
                // merge and free memory
                data = merge(chunk, s, other, o);
                free(chunk);
                free(other);
                chunk = data;
                s = s + o;
            }
        }

        // stop the timer
        elapsed_time += MPI_Wtime();

        printf("%f", elapsed_time);
        //printf("Quicksort %d ints on %d procs: %f secs\n", n, p, elapsed_time);

        MPI_Finalize();
    }
    return 0;
}