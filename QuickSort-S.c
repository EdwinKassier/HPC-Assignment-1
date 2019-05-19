//
// Created by Edwin on 2019-05-12.
//

#include "QuickSort-S.h"

//
// Created by Edwin on 2019-05-11.
//
#include<stdio.h>
#include <omp.h>

// A utility function to swap two elements
void swap(int * x, int * y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int partition (int arr[], int low, int high)
{
    // pivot
    int pivot = arr[high];
    // Index of smaller element
    int i = (low - 1);

    for (int j = low; j <= high- 1; j++){
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot){
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int * arr, int lo, int hi){
    if(lo < hi){
        int p = partition(arr, lo, hi);
        quickSort(arr, lo, p - 1);
        quickSort(arr, p + 1, hi);
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
int main(){
    int i;
    for (i=0; i < 10; i++) {
    FILE *file = fopen("C:\\Users\\Edwin\\CLionProjects\\HPC-A1\\Output(100000).txt", "r");
    int arr[100000];

    int i=0;
    int num;
    while(fscanf(file, "%d", &num) > 0){
        arr[i] = num;
        i++;
    }
    fclose(file);

    int n = sizeof(arr)/sizeof(arr[0]);
        double start_time = omp_get_wtime();
        quickSort(arr, 0, n - 1);
        //Validate(arr,arr);
        double end_time = omp_get_wtime() - start_time;
        printf("Time of execution %f", end_time);
        printf(".\n");
    }
    return 0;
}

