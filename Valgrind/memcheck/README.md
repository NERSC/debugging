Valgrind example codes:

-   `a.c`: A memcheck example from [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html)
    ```
    $ gcc -g -O0 a.c
    $ valgrind --leak-check=yes ./a.out
    ```
-   `a_mpi.c`: An MPI version of `a.c` to be used with valgrind or valgrind4hpc
    ```
    $ cc -g -O0 a_mpi.c

    $ srun -n 8 valgrind --leak-check=yes ./a.out

    $ srun -n 8 valgrind --leak-check=yes --log-file=mc_%q{SLURM_JOB_ID}.%p.out ./a.out

    $ valgrind4hpc -n 8 --valgrind-args="--leak-check=yes" ./a.out
    ```
-   `manual1.c`: An example code with system calls with invalid parameters; from [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html)
    ```
    $ gcc -g -O0 manuel1.c
    $ valgrind ./a.out
    ```
