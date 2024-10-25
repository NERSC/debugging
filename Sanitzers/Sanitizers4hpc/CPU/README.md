- `buggyreduction_mpiomp.c`: Santizers4hpc with ThreadSanitizer example code
    ```
    $ cc -fsanitize=thread -g -O1 -fopenmp buggyreduction_mpiomp.c -o buggyreduction_mpiomp
    $ export OMP_NUM_THREADS=2
    $ sanitizers4hpc -l "-n 2" -- ./buggyreduction_mpiomp
    ```
