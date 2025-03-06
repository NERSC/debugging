#include <stdlib.h>
#include <unistd.h>
#include "mpi.h"

int *chunk_a, *chunk_b;

void func_a(int size) {
    sleep(size);
    chunk_a = malloc(4000 * size);
    free(chunk_a);
} 

void func_b(int size) {
    chunk_b = malloc(2000 * size);
    func_a(size);
    free(chunk_b);
}

int main(int argc, char** argv) {
    int rank, size, i;
    int *a[10], *dynamicArray;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
	for (i = 0; i < size; i++) {
            dynamicArray = malloc(sizeof(int)*100000);
	}
    }

    for (i = 0; i < 10; i++) {
        a[i] = malloc(1000);
    }

    func_a(size);
    func_b(size/2);

    // Address not mapped error
    for (i = 0; i < 11; i++) {
        free(a[i]);
    }

    free(a[1]);         // Free previously freed pointer
    free(dynamicArray); // Should only be freed on proc 0

    MPI_Finalize();

    return 0;
}
