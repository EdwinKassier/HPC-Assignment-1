#include "omp.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
//Retrieved by Edwin Kassier from https://github.com/Fitzpasd/Parallel-sort-by-regular-sampling
/* headers */
int lcompare(const void * ptr2num1, const void * ptr2num2);
long long *merge(long long * left, long long * right, int l_end, int r_end);
long long *merge_sort(long long * arr, int size);
void insertion_sort(long long *arr, int n);
void calc_partition_borders(long long array[],
                            int start,
                            int end,
                            int sublist_sizes[],
                            int at,
                            long long pivots[],
                            int first_p,
                            int last_p);
void psrs_sort(long long *a, int n,int P);
void sortll(long long *a, int len);

/* sort an array in non-descending order */
void psrs_sort(long long *a, int n,int P) {
            // Testing shows that our algorithm is now the quickest
            int p, size, rsize, sample_size;
            long long *sample, *pivots;
            int *partition_borders, *bucket_sizes, *result_positions;
            long long **loc_a_ptrs;

            // Determine the appropriate number of threads to use
            p=P;
            omp_set_num_threads(p);

            size  = (n + p - 1) / p;
            rsize = (size + p - 1) / p;
            sample_size = p * (p - 1);

            loc_a_ptrs = malloc(p * sizeof(long long *));
            sample = malloc(sample_size * sizeof(long long));
            partition_borders = malloc(p * (p + 1) * sizeof(int));
            bucket_sizes = malloc(p * sizeof(int));
            result_positions = malloc(p * sizeof(int));
            pivots = malloc((p - 1) * sizeof(long long));

#pragma omp parallel
            {
                int i, j, max, thread_num, start, end, loc_size, offset, this_result_size;
                long long *loc_a, *this_result, *current_a;

                thread_num = omp_get_thread_num();
                start = thread_num * size;
                end = start + size - 1;
                if(end >= n) end = n - 1;
                loc_size = (end - start + 1);
                end = end % size;

                loc_a = malloc(loc_size * sizeof(long long));
                memcpy(loc_a, a + start, loc_size * sizeof(long long));
                loc_a_ptrs[thread_num] = loc_a;

                sortll(loc_a, loc_size); // Testing shows that this sequential sort is quickest in this instance

                offset = thread_num * (p - 1) - 1;

                for(i = 1; i < p; i++) {
                    if(i * rsize <= end) {
                        sample[offset + i] = loc_a[i * rsize - 1];
                    } else {
                        sample[offset + i] = loc_a[end];
                    }
                }

#pragma omp barrier

#pragma omp single
                {
                    merge_sort(sample, sample_size); // Testing shows that this sequential sort is quickest in this instance
                    for(i = 0; i < p - 1; i++) {
                        pivots[i] = sample[i * p + p / 2];
                    }
                }

#pragma omp barrier

                offset = thread_num * (p + 1);
                partition_borders[offset] = 0;
                partition_borders[offset + p] = end + 1;
                calc_partition_borders(loc_a, 0, loc_size-1, partition_borders, offset, pivots, 1, p-1);

#pragma omp barrier

                max = p * (p + 1);
                bucket_sizes[thread_num] = 0;
                for(i = thread_num; i < max; i += p + 1) {
                    bucket_sizes[thread_num] += partition_borders[i + 1] - partition_borders[i];
                }

#pragma omp barrier

#pragma omp single
                {
                    result_positions[0] = 0;
                    for(i = 1; i < p; i++) {
                        result_positions[i] = bucket_sizes[i-1] + result_positions[i-1];
                    }
                }

#pragma omp barrier

                this_result = a + result_positions[thread_num];

                if(thread_num == p-1) {
                    this_result_size = n - result_positions[thread_num];
                } else {
                    this_result_size = result_positions[thread_num+1] - result_positions[thread_num];
                }

                // pluck this threads sublist from each of the local arrays
                this_result = a + result_positions[thread_num];

                for(i = 0, j = 0; i < p; i++) {
                    int low, high, partition_size;
                    offset = i * (p + 1) + thread_num;
                    low = partition_borders[offset];
                    high = partition_borders[offset+1];
                    partition_size = (high - low);
                    if(partition_size > 0) {
                        memcpy(this_result+j, &(loc_a_ptrs[i][low]), partition_size * sizeof(long long));
                        j += partition_size;
                    }
                }

                // sort p local sorted arrays
                sortll(this_result, this_result_size); // Testing shows that this sequential sort is quickest in this instance

#pragma omp barrier
                free(loc_a);
            }

            free(loc_a_ptrs);
            free(sample);
            free(partition_borders);
            free(bucket_sizes);
            free(result_positions);
            free(pivots);
        }


/* determine the boundaries for the sublists of an local array */
void calc_partition_borders(long long array[],    // array being sorted
                            int start,
                            int end,              // separate the array into current process range
                            int result[],
                            int at,               // this process start point in result
                            long long pivots[],   // the pivot values
                            int first_pv,         // first pivot
                            int last_pv)          // last pivot
{
    int mid, lowerbound, upperbound, center;
    long long pv;

    mid = (first_pv + last_pv) / 2;
    pv = pivots[mid-1];
    lowerbound = start;
    upperbound = end;
    while(lowerbound <= upperbound) {
        center = (lowerbound + upperbound) / 2;
        if(array[center] > pv) {
            upperbound = center - 1;
        } else {
            lowerbound = center + 1;
        }
    }
    result[at + mid] = lowerbound;

    if(first_pv < mid) {
        calc_partition_borders(array, start, lowerbound - 1, result, at, pivots, first_pv, mid - 1);
    }
    if(mid < last_pv) {
        calc_partition_borders(array, lowerbound, end, result, at, pivots, mid + 1, last_pv);
    }
}


/* Compare long longs (lifted from the harness) */
int lcompare(const void * ptr2num1, const void * ptr2num2)
{
    long long num1 = *((long long*) ptr2num1);
    long long num2 = *((long long*) ptr2num2);

    if ( num1 > num2 )
        return 1;
    else if ( num1 < num2 )
        return -1;
    else
        return 0;
}


/*
  Sort a portion of an array
  @todo: see if merge sort might be better in some cases
*/
void sortll(long long *a, int len)
{
    qsort(a, len, sizeof(long long), lcompare);
}

/*
  Standard merge sort
*/
long long *merge_sort(long long * arr, int size){
    // Arrays shorter than 1 are already sorted
    if(size > 1){
        int middle = size / 2, i;
        long long *left, *right;
        left = arr;
        right = arr + middle;

        left = merge_sort(left, middle);
        right = merge_sort(right, size-middle);
        return merge(left, right, middle,size-middle);
    }else { return arr; }
}

long long *merge(long long * left, long long * right, int l_end, int r_end){
    int temp_off, l_off, r_off, size = l_end+r_end;
    long long *temp = malloc(sizeof(long long) * l_end);

    // Copy lower half into temp buffer
    for(l_off=0, temp_off=0; left+l_off != right; l_off++, temp_off++){
        *(temp + temp_off) = *(left + l_off);
    }

    temp_off=0; l_off=0; r_off=0;

    while(l_off < size){
        if(temp_off < l_end){
            if(r_off < r_end){
                if(*(temp+temp_off) < *(right+r_off)){
                    *(left+l_off) = *(temp+temp_off);
                    temp_off++;
                }else{
                    *(left+l_off) = *(right+r_off);
                    r_off++;
                }
            }else{
                *(left+l_off) = *(temp+temp_off);
                temp_off++;
            }
        }else{
            if(r_off < r_end) {
                *(left + l_off) = *(right + r_off);
                r_off++;
            }else{
                printf("\nERROR - merging loop going too far\n");
            }
        }
        l_off++;
    }
    free(temp);
    return left;
}

/*
  Standard insertion sort
*/
void insertion_sort(long long *arr, int n){
    int i, j, k, temp;

    for ( i = 1 ; i <= n ; i++ )
    {
        for ( j = 0 ; j < i ; j++ )
        {
            if ( arr[j] > arr[i] )
            {
                temp = arr[j] ;
                arr[j] = arr[i] ;

                for ( k = i ; k > j ; k-- )
                    arr[k] = arr[k - 1] ;

                arr[k + 1] = temp ;
            }
        }
    }
}

int llcompare(const void * ptr2num1, const void * ptr2num2)
{
    long long num1 = *((long long*) ptr2num1);
    long long num2 = *((long long*) ptr2num2);

    if ( num1 > num2 )
        return 1;
    else if ( num1 < num2 )
        return -1;
    else
        return 0;
}

/* sort an array into non-decreasing order */
void sort(long long a[], int size)
{
    qsort(a, size, sizeof(long long), llcompare);
}

/* write out an array of integers up to 'size' */
void write_out(long long a[], int size)
{
    int i;

    for ( i = 0; i < size; i++ ) {
        printf("%lld\n", a[i]);
    }
}


int main(int argc, char ** argv)
{
    long long * array;
    long long * copy;
    long long sort_time;
    int array_size;
    int j;
    int c, num;
    double start_time, end_time; /* start and end times */
    int numWorkers;



    printf("PSRS OMP\n");
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

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 1000);

        //Generating random number list
        for (c = 0; c < 1000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }

        start_time = omp_get_wtime();
        psrs_sort(arr, 1000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //10000
    printf("Time of execution: 10000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 10000);

        //Generating random number list
        for (c = 0; c < 10000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }

        start_time = omp_get_wtime();
        psrs_sort(arr, 10000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //100000
    printf("Time of execution: 100000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(100000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 100000);

        //Generating random number list
        for (c = 0; c < 100000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 100000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //1000000
    printf("Time of execution: 1000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(1000000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 1000000);

        //Generating random number list
        for (c = 0; c < 1000000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 1000000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //10000000
    printf("Time of execution: 10000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 10000000);

        //Generating random number list
        for (c = 0; c < 10000000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 10000000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    printf(".\n");
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

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 1000);

        //Generating random number list
        for (c = 0; c < 1000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 1000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //10000
    printf("Time of execution: 10000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 10000);

        //Generating random number list
        for (c = 0; c < 10000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 10000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //100000
    printf("Time of execution: 100000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(100000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 100000);

        //Generating random number list
        for (c = 0; c < 100000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 100000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //1000000
    printf("Time of execution: 1000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(1000000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 1000000);

        //Generating random number list
        for (c = 0; c < 1000000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 1000000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //10000000
    printf("Time of execution: 10000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 10000000);

        //Generating random number list
        for (c = 0; c < 10000000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 10000000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }



    printf(".\n");
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

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 1000);

        //Generating random number list
        for (c = 0; c < 1000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 1000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //10000
    printf("Time of execution: 10000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 10000);

        //Generating random number list
        for (c = 0; c < 10000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 10000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //100000
    printf("Time of execution: 100000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(100000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 100000);

        //Generating random number list
        for (c = 0; c < 100000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 100000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //1000000
    printf("Time of execution: 1000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(1000000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 1000000);

        //Generating random number list
        for (c = 0; c < 1000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 1000000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }

    //10000000
    printf("Time of execution: 10000000 inputs");
    printf(".\n");
    for (j = 0; j < 10; j++) {
        //seed random number generator
        srand(10000000);

        //Initialise array
        long long *arr = malloc(sizeof(long long) * 10000000);

        //Generating random number list
        for (c = 0; c < 10000000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = (long long)num;
        }


        start_time = omp_get_wtime();
        psrs_sort(arr, 10000000,numWorkers);
        end_time = omp_get_wtime();
        free(arr);

        printf("%g\n", end_time - start_time);
    }
    return 0;
}