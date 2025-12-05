#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_SIZE 250000

void merge(int arr[], int l, int m, int r) {
    int i, j, k, n1 = m - l + 1, n2 = r - m;
    int L[n1], R[n2];

    for(i=0; i<n1; i++)
        L[i] = arr[l + i];
    for(j=0; j<n2; j++)
        R[j] = arr[m + 1 + j];

    i=0;
    j=0;
    k=l;
    while(i<n1 && j<n2) 
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    
    while (i<n1) 
        arr[k++] = L[i++];

    while(j<n2) 
        arr[k++] = R[j++];
}

void mergeSortSerial(int arr[], int l, int r) {
    if(l<r) {
        int m = l + (r-l) / 2;
        mergeSortSerial(arr, l, m);
        mergeSortSerial(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

void mergeSortParallel(int arr[], int l, int r) {
    if (l<r) {
        if(r-l < 1000) {
            mergeSortSerial(arr, l, r);
        } else {
            int m = l + (r-l) / 2;
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSortParallel(arr, l, m);
                #pragma omp section
                mergeSortParallel(arr, m+1, r);
            }
            merge(arr, l, m, r);
        }
    }
}

int main() {
    int i, size, arr[MAX_SIZE];

    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Error: Cannot open data.txt\n");
        return -1;
    }

    size = 250000;
    for(i=0; i<size; i++) {
        fscanf(file, "%d", &arr[i]);
    }
    fclose(file);

    double start = omp_get_wtime();
    mergeSortParallel(arr, 0, size-1);
    double end = omp_get_wtime();
    
    FILE *sort = fopen("sorted_omp.txt", "w");
    if (sort == NULL) {
        printf("Error: Cannot create sorted_omp.txt\n");
        return -1;
    }
    
    for(int i=0; i<size; i++) {
        fprintf(sort, "%d\n", arr[i]);
    }
    fclose(sort);
    
    printf("Sorting completed!\n");
    printf("Execution time in seconds: %f\n", (end-start));
    printf("Sorted data written to sorted_omp.txt\n");
    
    return 0;
}