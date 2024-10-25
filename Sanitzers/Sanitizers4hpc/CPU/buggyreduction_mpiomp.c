#include "mpi.h"
#include <stdio.h>

int main (int argc, char **argv) {
  int rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sum = 0;
  #pragma omp parallel for shared(sum)
  for (int i=0; i<1000; i++)
    sum += i;

  printf("%d: sum = %d\n", rank, sum);

  MPI_Finalize();
  return 0;
}
