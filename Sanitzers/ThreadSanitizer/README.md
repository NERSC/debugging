-   `buggyreduction_omp.c`: ThreadSanitizer example codes
    ```
    $ cc -fsanitize=thread -g -O1 -fopenmp buggyreduction_omp.c -o buggyreduction_omp
    $ export OMP_NUM_THREADS=8
    $ ./buggyreduction_omp
    ```
- `tiny_race.c`: a pthread example code; from [Clang ThreadSanitizer
  documentation](https://clang.llvm.org/docs/ThreadSanitizer.html)
    ```
    $ clang -fsanitize=thread -g -O1 tiny_race.c -o tiny_race
    $ ./tiny_race
    ```
