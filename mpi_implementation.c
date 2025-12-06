#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_SIZE 250000   // total amount of data

void merge(int *a, int *b, int l, int m, int r);
void mergeSort(int *a, int *b, int l, int r);

int main(int argc, char** argv) {

    int world_rank;   // id of each MPI process
    int world_size;   // total number of MPI processes
    int size = 250000;

    int *original_array = NULL;  // only used by rank 0
    int *sub_array;              // chunk of data for each process
    int *tmp_array;              // temp array for local sort
    int *sorted = NULL;          // final collected sorted list (rank 0)
    int *other_array;

    double start_time, end_time;

    // start MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // rank 0 loads the data from file
    if (world_rank == 0) {
        original_array = (int *)malloc(size * sizeof(int));

        FILE *file = fopen("data.txt", "r");
        if (file == NULL) {
            printf("Error: Cannot open data.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            return -1;
        }

        // read all numbers into memory
        for(int i = 0; i < size; i++) {
            fscanf(file, "%d", &original_array[i]);
        }
        fclose(file);

        // start timing after reading file
        start_time = MPI_Wtime();
    }

    // each process will get size/world_size amount of data
    int sub_size = size / world_size;

    // allocate memory for each process's chunk
    sub_array = (int *)malloc(sub_size * sizeof(int));

    // scatter the big array -> each process gets a chunk
    MPI_Scatter(original_array, sub_size, MPI_INT,
                sub_array, sub_size, MPI_INT,
                0, MPI_COMM_WORLD);

    // sort the local chunk
    tmp_array = (int *)malloc(sub_size * sizeof(int));
    mergeSort(sub_array, tmp_array, 0, sub_size - 1);

    // gather all sorted chunks back to rank 0
    if (world_rank == 0) {
        sorted = (int *)malloc(size * sizeof(int));
    }

    MPI_Gather(sub_array, sub_size, MPI_INT,
               sorted, sub_size, MPI_INT,
               0, MPI_COMM_WORLD);

    // rank 0 now merges all sorted chunks
    if (world_rank == 0) {
        other_array = (int *)malloc(size * sizeof(int));

        // final merge sort over the combined chunks
        mergeSort(sorted, other_array, 0, size - 1);

        end_time = MPI_Wtime();

        // write results to file
        FILE *output = fopen("sorted_mpi.txt", "w");
        if (output == NULL) {
            printf("Error: Cannot create sorted_mpi.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            return -1;
        }

        for(int i = 0; i < size; i++) {
            fprintf(output, "%d\n", sorted[i]);
        }
        fclose(output);

        printf("Sorting completed!\n");
        printf("Execution time in seconds: %f\n", end_time - start_time);
        printf("Sorted data written to sorted_mpi.txt\n");
        printf("Used %d MPI processes\n", world_size);

        // clean up only used by rank 0
        free(sorted);
        free(other_array);
        free(original_array);
    }

    // free memory used by all processes
    free(sub_array);
    free(tmp_array);

    // end MPI session
    MPI_Finalize();

    return 0;
}

//merge two sorted halves into one 
void merge(int *a, int *b, int l, int m, int r) {

    int h = l;      // pointer into left half
    int i = l;      // pointer in temp array
    int j = m + 1;  // pointer into right half

    // merge until one half finishes
    while(h <= m && j <= r) {
        if(a[h] <= a[j]) {
            b[i] = a[h];
            h++;
        } else {
            b[i] = a[j];
            j++;
        }
        i++;
    }

    // copy the leftover part (whichever side still has data)
    if(h > m) {
        for(int k = j; k <= r; k++)
            b[i++] = a[k];
    } else {
        for(int k = h; k <= m; k++)
            b[i++] = a[k];
    }

    // copy everything back to the main array
    for(int k = l; k <= r; k++)
        a[k] = b[k];
}

//basic recursive merge sort
void mergeSort(int *a, int *b, int l, int r) {
    if(l < r) {
        int m = (l + r) / 2;

        // sort left half
        mergeSort(a, b, l, m);

        // sort right half
        mergeSort(a, b, m + 1, r);

        // merge sorted halves
        merge(a, b, l, m, r);
    }
}
