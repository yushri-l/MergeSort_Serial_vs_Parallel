#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 250000

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int left[n1], right[n2];

    for(int i=0; i<n1; i++)
        left[i] = arr[l + i];
    for(int i=0; i<n2; i++)
        right[i] = arr[m + 1 + i];

    int i=0, j=0, k=l;
    while(i<n1 && j<n2) {
        if (left[i] < right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i<n1) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while(j<n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if(l<r) {
        int m = l + (r-l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}

int main() {
    int size = 250000;
    int arr[MAX_SIZE];

    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Error: Cannot open file\n");
        return -1;
    }

    for(int i=0; i<size; i++) {
        fscanf(file, "%d", &arr[i]);
    }
    fclose(file);

    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    mergeSort(arr, 0, size-1);
    end = clock();
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Sorting completed!\n");
    printf("Time taken: %f seconds\n", cpu_time_used);
    
    // Write sorted data to a new file
    FILE *output = fopen("sorted_data.txt", "w");
    if (output == NULL) {
        printf("Error: Cannot create output file\n");
        return -1;
    }
    
    for(int i=0; i<size; i++) {
        fprintf(output, "%d\n", arr[i]);
    }
    fclose(output);
    
    printf("Sorted data written to sorted_data.txt\n");
    
    return 0;
}