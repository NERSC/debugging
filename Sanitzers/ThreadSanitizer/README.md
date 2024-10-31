# ThreadSanitizer (TSAN)

Example codes:

-   `buggyreduction_omp.c`: ThreadSanitizer example codes
-   `tiny_race.c`: a pthread example code; from [Clang ThreadSanitizer
    documentation](https://clang.llvm.org/docs/ThreadSanitizer.html)
    ```
    $ clang -fsanitize=thread -g -O1 tiny_race.c -o tiny_race
    $ ./tiny_race
    ```

## `buggyreduction_omp.c`

This code doesn't have the `reduction` clause for the variable
`sum`, so there is a race condition among OpenMP threads.

To build and run:

```
$ cc -fsanitize=thread -g -O1 -fopenmp buggyreduction_omp.c -o buggyreduction_omp

$ export OMP_NUM_THREADS=8

$ ./buggyreduction_omp
=================
WARNING: ThreadSanitizer: data race (pid=2240264)
  Read of size 4 at 0x7ffdf6e678bc by thread T1:
    #0 main._omp_fn.0 /pscratch/sd/e/elvis/sanitizers/buggyreduction_omp.c:6 (a.out+0x400895)
    #1 <null> <null> (libgomp.so.1+0x1dd4d)

  Previous write of size 4 at 0x7ffdf6e678bc by main thread:
    #0 main._omp_fn.0 /pscratch/sd/e/elvis/sanitizers/buggyreduction_omp.c:7 (a.out+0x4008aa)
    #1 GOMP_parallel <null> (libgomp.so.1+0x14e95)

  Location is stack of main thread.

  Location is global '<null>' at 0x000000000000 ([stack]+0x1e8bc)

  Thread T1 (tid=2240266, running) created by main thread at:
    #0 pthread_create <null> (libtsan.so.2+0x61be6)
    #1 <null> <null> (libgomp.so.1+0x1e38f)

SUMMARY: ThreadSanitizer: data race /pscratch/sd/e/elvis/sanitizers/buggyreduction_omp.c:6 in main._omp_fn.0
==================
sum = 335625
ThreadSanitizer: reported 1 warnings
```

You may have to run a few times to see the error (because of a race
condition!).
