#include <stdio.h>

int main (int argc, char **argv) {
  int sum = 0;
  #pragma omp parallel for shared(sum)
  for (int i=0; i<1000; i++)
    sum += i;

  printf("sum = %d\n", sum);
  return 0;
}
