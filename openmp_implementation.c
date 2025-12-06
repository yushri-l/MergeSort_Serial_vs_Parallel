#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_SIZE 250000   // max array size we expect

// merge two sorted halves of the array (normal merge procedure)
void merge(int arr[], int l, int m, int r) {
    int i, j, k, n1 = m - l + 1, n2 = r - m;

    // temp arrays for left and right split
    int L[n1], R[n2];

    // copy left side into temp array
    for(i = 0; i < n1; i++)
        L[i] = arr[l + i];

    // copy right side into temp array
    for(j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // merge everything back in sorted order
    i = 0;
    j = 0;
    k = l;

    // pick smaller of L[i] and R[j] and move it to arr[k]
    while(i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    // copy leftovers from left side
    while(i < n1)
        arr[k++] = L[i++];

    // copy leftovers from right side
    while(j < n2)
        arr[k++] = R[j++];
}

// normal serial mergesort (used for small chunks)
void mergeSortSerial(int arr[], int l, int r) {
    if(l < r) {
        int m = l + (r - l) / 2;

        // sort left half
        mergeSortSerial(arr, l, m);

        // sort right half
        mergeSortSerial(arr, m + 1, r);

        // merge them
        merge(arr, l, m, r);
    }
}

// parallel mergesort using OpenMP sections
void mergeSortParallel(int arr[], int l, int r) {

    // only sort if more than one element
    if(l < r) {

        // if the segment is small, no point using threads → use serial version
        if(r - l < 1000) {
            mergeSortSerial(arr, l, r);

        } else {
            int m = l + (r - l) / 2;

            // run left and right halves in parallel
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSortParallel(arr, l, m);      // left side

                #pragma omp section
                mergeSortParallel(arr, m + 1, r);  // right side
            }

            // merge both sorted halves
            merge(arr, l, m, r);
        }
    }
}

int main() {

    int i, size, arr[MAX_SIZE];

    // load data from file
    FILE *file = fopen("data.txt", "r");
    if(file == NULL) {
        printf("Error: Cannot open data.txt\n");
        return -1;
    }

    size = 250000;

    // reading all numbers into the array
    for(i = 0; i < size; i++) {
        fscanf(file, "%d", &arr[i]);
    }
    fclose(file);

    // start timer
    double start = omp_get_wtime();

    // run parallel mergesort
    mergeSortParallel(arr, 0, size - 1);

    // end timer
    double end = omp_get_wtime();

    // write sorted output to file
    FILE *sort = fopen("sorted_omp.txt", "w");
    if(sort == NULL) {
        printf("Error: Cannot create sorted_omp.txt\n");
        return -1;
    }

    for(int i = 0; i < size; i++) {
        fprintf(sort, "%d\n", arr[i]);
    }
    fclose(sort);

    printf("Sorting completed!\n");
    printf("Execution time in seconds: %f\n", (end - start));
    printf("Sorted data written to sorted_omp.txt\n");

    return 0;
}
