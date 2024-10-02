/* Taken from Valgrind memcheck manual */

#include <stdlib.h>
#include <unistd.h>

int main( void )
{
  char* arr = malloc(10);
  int* arr2 = malloc(sizeof(int));

  write( 1 /* stdout */, arr, 10 );

  exit(arr2[0]);
}
