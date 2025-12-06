#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 250000   // max number of elements we expect

// This function merges two already sorted halves of the array.
// Left part = arr[l..m], right part = arr[m+1..r]
void merge(int arr[], int l, int m, int r) {

    // sizes of the two sub arrays
    int n1 = m - l + 1;
    int n2 = r - m;

    // temp arrays
    int left[n1], right[n2];

    // copy left half
    for(int i = 0; i < n1; i++)
        left[i] = arr[l + i];

    // copy right half
    for(int i = 0; i < n2; i++)
        right[i] = arr[m + 1 + i];

    // i -> left index, j -> right index, k -> main array index
    int i = 0, j = 0, k = l;

    // merge both halves into the main array
    while(i < n1 && j < n2) {
        if(left[i] < right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // copy remaining values from left side if any
    while(i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }

    // copy remaining values from right side
    while(j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}


// Standard merge sort: split, sort, and merge
void mergeSort(int arr[], int l, int r) {
    if(l < r) {
        int m = l + (r - l) / 2;   // midpoint

        // sort left half
        mergeSort(arr, l, m);

        // sort right half
        mergeSort(arr, m + 1, r);

        // merge the two sorted halves
        merge(arr, l, m, r);
    }
}


int main() {

    int size = 250000;   // how many numbers to read
    int arr[MAX_SIZE];   // array to store numbers

    // opening the input file
    FILE *file = fopen("data.txt", "r");
    if(file == NULL) {
        printf("Error: Cannot open file\n");
        return -1;
    }

    // reading values from file into array
    for(int i = 0; i < size; i++) {
        fscanf(file, "%d", &arr[i]);
    }
    fclose(file);

    // setup for timing
    clock_t start, end;
    double cpu_time_used;

    start = clock();                 // start measuring time
    mergeSort(arr, 0, size - 1);     // perform merge sort
    end = clock();                   // end time

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Sorting completed!\n");
    printf("Time taken: %f seconds\n", cpu_time_used);

    // writing sorted data to another file
    FILE *output = fopen("sorted_data.txt", "w");
    if(output == NULL) {
        printf("Error: Cannot create output file\n");
        return -1;
    }

    for(int i = 0; i < size; i++) {
        fprintf(output, "%d\n", arr[i]);
    }

    fclose(output);

    printf("Sorted data written to sorted_data.txt\n");

    return 0;
}
