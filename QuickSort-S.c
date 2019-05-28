//
// Created by Edwin on 2019-05-12.
//


#include<stdio.h>
#include <omp.h>
#include <stdlib.h>

// A utility function to swap two elements
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int * arr, int low, int high)
{
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int * arr, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/* Function to validate array has been sorted */
void Validate(int arr[], int size){
    int i;
    for (i=0; i < size-1; i++)
        printf("%d ", arr[i]);
    printf("Valid");
}

// Driver program to test above functions
int main(int argc, char *argv[]){



    int j;
    int c, num;
    int * Large = NULL;

    printf("Serial Quicksort\n");
    //1000 inputs
    printf("Time of execution: 1000 inputs");
    printf(".\n");
    for(j=0;j<10;j++){
    //Initialise array
    int arr[1000] ={0};

    //seed random number generator
    srand(1000);

    //Generating random number list
    for (c = 0; c < 1000; c++) {
        num = rand() % 5000 + 1;
        arr[c] = num;
    }

        int n = sizeof(arr)/sizeof(arr[0]);
        //start timer
        double start_time = omp_get_wtime();
        quickSort(arr, 0, n - 1);
        //stop timer
        double end_time = omp_get_wtime() - start_time;
        printf("%f\n",end_time);
   }

    //10000 inputs
    printf("Time of execution: 10000 inputs");
    printf(".\n");
    for(j=0;j<10;j++){
        //Initialise array
        int arr[10000] ={0};

        //seed random number generator
        srand(10000);

        //Generating random number list
        for (c = 0; c < 10000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = num;
        }

        int n = sizeof(arr)/sizeof(arr[0]);
        //start timer
        double start_time = omp_get_wtime();
        quickSort(arr, 0, n - 1);
        //stop timer
        double end_time = omp_get_wtime() - start_time;
        printf("%f\n",end_time);
    }

    //100000 inputs
    printf("Time of execution: 100000 inputs");
    printf(".\n");
    for(j=0;j<10;j++){
        //Initialise array
        int arr[100000] ={0};

        //seed random number generator
        srand(100000);

        //Generating random number list
        for (c = 0; c < 100000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = num;
        }

        int n = sizeof(arr)/sizeof(arr[0]);
        //start timer
        double start_time = omp_get_wtime();
        quickSort(arr, 0, n - 1);
        //stop timer
        double end_time = omp_get_wtime() - start_time;
        printf("%f\n",end_time);

    }

    //1000000 inputs
    printf("Time of execution: 1000000 inputs");
    printf(".\n");
    for(j=0;j<10;j++){
        //Initialise array
        int arr[1000000] ={0};

        //seed random number generator
        srand(1000000);

        //Generating random number list
        for (c = 0; c < 1000000; c++) {
            num = rand() % 5000 + 1;
            arr[c] = num;
        }

        int n = sizeof(arr)/sizeof(arr[0]);
        //start timer
        double start_time = omp_get_wtime();
        quickSort(arr, 0, n - 1);
        //stop timer
        double end_time = omp_get_wtime() - start_time;
        printf("%f\n",end_time);
    }



    //10000000 inputs
    printf("Time of execution: 1000000 inputs");
    printf(".\n");
    for(j=0;j<10;j++){
        //Initialise array
        int *Large = malloc (sizeof(int)*10000000);

        //seed random number generator
        srand(10000000);

        //Generating random number list
        for (c = 0; c < 10000000; c++) {
            num = rand() % 5000 + 1;
            Large[c] = (int)num;
        }

        int n = sizeof(Large)/sizeof(Large[0]);
        //start timer
        double start_time = omp_get_wtime();
        quickSort(Large, 0, n - 1);
        //stop timer
        double end_time = omp_get_wtime() - start_time;
        printf("%f\n",end_time);
        free(Large);
    }




    return 0;
}

