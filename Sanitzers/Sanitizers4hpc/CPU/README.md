# Sanitizers4hpc with CPU codes

Example code:

-   `buggyreduction_mpiomp.c`: Santizers4hpc with ThreadSanitizer
    example code

## `buggyreduction_mpiomp.c`

This is a simple MPI code based on `buggyreduction_omp.c`.

```
$ salloc -C cpu -n 2 -c 2 -q shared -t 20
...

$ cc -fsanitize=thread -g -O1 -fopenmp buggyreduction_mpiomp.c -o buggyreduction_mpiomp

$ export OMP_NUM_THREADS=2

$ module load sanitizers4hpc

$ sanitizers4hpc -l "-n 2 -c 2" -- ./buggyreduction_mpiomp
```
