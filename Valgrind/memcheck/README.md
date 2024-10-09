Valgrind example codes:

-   `memoryleak.c`: A memcheck example from [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html)
    ```
    $ gcc -g -O0 -o memoryleak memoryleak.c
    $ valgrind --leak-check=yes ./memoryleak
    ```
-   `memoryleak_mpi.c`: An embarassingly parallel MPI version of `memoryleak.c` to be used with valgrind or valgrind4hpc
    ```
    $ cc -g -O0 -o memoryleak_mpi memoryleak_mpi.c

    $ srun -n 8 valgrind --leak-check=yes ./memoryleak_mpi

    $ srun -n 8 valgrind --leak-check=yes --log-file=mc_%q{SLURM_JOB_ID}.%q{SLURM_PROCID}.out ./memoryleak_mpi

    $ valgrind4hpc -n 8 --valgrind-args="--leak-check=yes" ./memoryleak_mpi
    ```
-   `manual1.c`: An example code with system calls with invalid parameters; from [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html)
    ```
    $ gcc -g -O0 -o manuel1 manuel1.c
    $ valgrind ./manuel1
    ```
