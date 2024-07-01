-   `buggyreduction_omp.c`: ThreadSanitizer example codes
    ```
    $ cc -fsanitize=thread -g -O1 -fopenmp buggyreduction_omp.c
    $ export OMP_NUM_THREADS=8
    $ sanitizers4hpc -l "-n 2" -- ./a.out
    ```
- `buggyreduction_mpiomp.c`: Santizers4hpc with ThreadSanitizer example code
    ```
    $ cc -fsanitize=thread -g -O1 -fopenmp buggyreduction_mpiomp.c
    $ export OMP_NUM_THREADS=2
    $ sanitizers4hpc -l "-n 2" -- ./a.out
    ```
