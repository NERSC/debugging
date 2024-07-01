#include <iostream>

int main(int argc, char **argv) {
 int *array = new int[100];

  for (int i = 0; i < 110; ++i) // Access more than allocated memory.
      array[i] = i+1;

  delete [] array;

  return 0;
}
