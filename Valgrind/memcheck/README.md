# Memcheck: a memory error detector

## Use of uninitialized values

`manual1.c`:

-   An example code that uses a value which hasn't been defined.
-   From [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html)
-   x is not initialized, but it is used in a conditional statement

To build and run:

```
$ gcc -g -O0 -o manuel1 manuel1.c
$ valgrind ./manuel1
...
==1018824== Conditional jump or move depends on uninitialised value(s)
==1018824==    at 0x400525: main (manuel1.c:7)
...
```

## Illegal frees

`doublefree.c`:

-   From [Valgrind User Manual](https://valgrind.org/docs/manual/manual.html)
-   The code attempts to free the block pointed to by `p` twice

To build and run:

```
$ gcc -g -O0 -o doublefree doublefree.c
$ valgrind ./doublefree
...
==1064682== Invalid free() / delete / delete[] / realloc()
==1064682==    at 0x4E080EB: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==1064682==    by 0x400580: main (doublefree.c:10)
==1064682==  Address 0x523f040 is 0 bytes inside a block of size 177 free'd
==1064682==    at 0x4E080EB: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==1064682==    by 0x400580: main (doublefree.c:10)
==1064682==  Block was alloc'd at
==1064682==    at 0x4E056A4: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==1064682==    by 0x400567: main (doublefree.c:8)
...
```

## Memory leaks

### Serial code

`memoryleak.c`:

-   An example code from [Valgrind User
    Manual](https://valgrind.org/docs/manual/manual.html)
-   The code has two issues.
    - It makes an out-of-bound reference - writing 4 bytes beyond
      the allocated block, which generates the message `Invalid
      write of size 4`.
    - The program doesn't free the memory block before exiting the
      function `f()`, resulting in a memory leak of 40 bytes.

To build and run:

```
$ gcc -g -O0 -o memoryleak memoryleak.c
$ valgrind --leak-check=full ./memoryleak
...
==1127011== Invalid write of size 4
==1127011==    at 0x400534: f (memoryleak.c:6)
==1127011==    by 0x400545: main (memoryleak.c:11)
==1127011==  Address 0x523f068 is 0 bytes after a block of size 40 alloc'd
==1127011==    at 0x4E056A4: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==1127011==    by 0x400527: f (memoryleak.c:5)
==1127011==    by 0x400545: main (memoryleak.c:11)
==1127011==
==1127011==
==1127011== HEAP SUMMARY:
==1127011==     in use at exit: 40 bytes in 1 blocks
==1127011==   total heap usage: 1 allocs, 0 frees, 40 bytes allocated
==1127011==
==1127011== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1
==1127011==    at 0x4E056A4: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==1127011==    by 0x400527: f (memoryleak.c:5)
==1127011==    by 0x400545: main (memoryleak.c:11)
==1127011==
==1127011== LEAK SUMMARY:
==1127011==    definitely lost: 40 bytes in 1 blocks
==1127011==    indirectly lost: 0 bytes in 0 blocks
==1127011==      possibly lost: 0 bytes in 0 blocks
==1127011==    still reachable: 0 bytes in 0 blocks
==1127011==         suppressed: 0 bytes in 0 blocks
==1127011==
==1127011== For lists of detected and suppressed errors, rerun with: -s
==1127011== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

### MPI code

`memoryleak_mpi.c`:

-   A simple MPI version of `memoryleak.c`, but with a larger memory
    block size.

To build:

```
$ cc -g -O0 -o memoryleak_mpi memoryleak_mpi.c
```

To run, start a batch job to run the MPI application on a compute
node.

```
$ salloc -C cpu -N 1 -q debug -t 10       # Start an interactive batch job
...
```

Run the app under Valgrind with each MPI rank's output sent to a
separate file. To distinguish among the output files, we can use a
format specifier, `%q{FOO}` which will be replaced by the contents
of the environment variable `FOO`.

```
$ srun -n 8 valgrind --leak-check=yes --log-file=mc_%q{SLURM_JOB_ID}.%q{SLURM_PROCID}.out ./memoryleak_mpi

$ ls -l
-rw-------   1 elvis elvis   5481  Jun  23  08:56  mc_27100535.0.out
-rw-------   1 elvis elvis   5481  Jun  23  08:56  mc_27100535.1.out
...
-rw-------   1 elvis elvis   5481  Jun  23  08:56  mc_27100535.7.out
```

## Suppressing errors

Memcheck occasionally produces false positives or errors in the
system library codes that  you cannot control or don't care.

Use `--gen-suppressions=all` to show all the errors in the correct
form representing backtrace patterns, which can be used for suppressing
them. The command below saves output in the file `errors`:

```
$ srun -n 1 valgrind --leak-check=full --gen-suppressions=yes --log-file=errors ./memoryleak_mpi
```

Get lines not starting with `==` and save them into a suppression
file. Then edit the file to contain only the errors you want to
suppress.

```
$ grep -v -e '^==' errors > my.supp
$ vi my.supp          # Edit by hand
$ cat my.supp
.
.
.
{
   mysupp3
   Memcheck:Cond
   fun:add_entry
   fun:darshan_get_exe_and_mounts
   ...
   fun:main
}
.
.
.
```

-  Here, `mysupp3` is a name of the suppresion.
-  `Memcheck:Cond` means that this is for the `Memcheck` tool and
   the error is associated with an undefined value being used in a
   conditional.
-   The next lines are for a calling context. The error occurs in
    function `add_entry` which is called by function
    `darshan_get_exe_and_mounts`, etc. Note that `...` (three dots)
    is a frame-level wildcard, which matches zero or more frames.

To use the suppression file, run as follows:

```
$ srun -n 8 valgrind --leak-check=full --suppressions=my.supp --log-file=ml.%q{SLURM_JOB_ID}.%q{SLURM_PROCID} ./memoryleak_mpi

$ cat ml.32034480.0.out
...
==544049== LEAK SUMMARY:
...
==544049== ERROR SUMMARY: 18 errors from 6 contexts (suppressed: 7 from 1)
```

The output file says that 7 errors were suppressed from one suppression
pattern.

## Valgrind4hpc

Valgrind4hpc is a HPE tool that aggregates duplicate Valgrind
messages across MPI processes. With the tool, we can avoid duplication
of messages and individual output files. Also the tool uses suppression
files for the errors associated with HPE software.

The tool works only for Memcheck, Helgrind and DRD.

Run as follows. Note that the Valgrind arguments are passed with
`--valgrind-args=...` (or `-v ...`).

```
$ module load valgrind4hpc
$ valgrind4hpc -n 8 --valgrind-args="--leak-check=yes" ./memoryleak_mpi
RANKS: <0..7>
Invalid write of size 4
  at f (in memoryleak_mpi.c:7)
  by main (in memoryleak_mpi.c:16)
Address is 0 bytes after a block of size 40 alloc'd
  at malloc (in vg_replace_malloc.c:393)
  by f (in memoryleak_mpi.c:6)
  by main (in memoryleak_mpi.c:16)

RANKS: <0..7>
40 bytes in 1 blocks are definitely lost
  at malloc (in vg_replace_malloc.c:393)
  by f (in memoryleak_mpi.c:6)
  by main (in memoryleak_mpi.c:16)

RANKS: <0..7>

HEAP SUMMARY:
  in use at exit: 40 bytes in 1 blocks

LEAK SUMMARY:
   definitely lost: 40 bytes in 1 blocks
   indirectly lost: 0 bytes in 0 blocks
     possibly lost: 0 bytes in 0 blocks
   still reachable: 0 bytes in 0 blocks

ERROR SUMMARY: 1 errors from 1 contexts (suppressed 601)
```

Note that 601 errors were suppressed.

## Memory profiling with Execution trees

An executaion tree ("xtree") is made of a set of stack traces, each
stack trace is associated with some resource consumptions or event
counts. We can use them to get a graphical or textual representation
of the heap usage.

### Heap usage from execution trees

The following is to get xtree output in Callgrind or Massif format
and then process it with a Callgrind or Massif tool to generate an
annotated profiling result. Note that we use the --xtree-memory=full

```
$ module rm darshan

$ cc -g -O0 -o memoryleak_mpi memoryleak_mpi.c

$ srun -n 8 valgrind --xtree-memory=full --xtree-memory-file=xtmemory.%q{SLURM_JOB_ID}.%q{SLURM_PROCID}.kcg ./memoryleak_mpi
```

-   `--xtree-memory=full`: The memory execution tree gives 6 different
    measurements: the current number of allocated bytes and blocks
    (`curB` and `curBk` respectively), the total number of allocated
    bytes and blocks (`totB` and `totBk`) and the total number of
    freed bytes and blocks (`totFdB` and `totFdBk`).
-   `--xtree-memory-file=...`: Output file name

You can process the data by running the following command.  The
tool sorts data by `curB`, `curBk`, etc. in that order. The number
after the colon is a threshold and the tool prints data up to the
given threshold cumulative percentage. So `curB:100` means prints
every data (until the cumulative sum reaches 100%).

```
$ callgrind_annotate --inclusive=yes --sort=curB:100,curBk:100,totB:100,totBk:100,totFdB:100,totFdBk:100 xtmemory.32282016.0.kcg
...
--------------------------------------------------------------------------------
curB             curBk        totB               totBk        totFdB             totFdBk
--------------------------------------------------------------------------------
195,957 (100.0%) 602 (100.0%) 1,932,992 (100.0%) 789 (100.0%) 1,737,035 (100.0%) 187 (100.0%)  PROGRAM TOTALS

--------------------------------------------------------------------------------
curB             curBk        totB               totBk        totFdB             totFdBk       file:function
--------------------------------------------------------------------------------
195,957 (100.0%) 602 (100.0%) 1,911,132 (98.87%) 758 (96.07%) 1,719,355 (98.98%) 163 (87.17%)  memoryleak_mpi.c:main
100,000 (51.03%)   1 ( 0.17%)   100,000 ( 5.17%)   1 ( 0.13%)         0            0           memoryleak_mpi.c:f
 95,957 (48.97%) 601 (99.83%) 1,815,312 (93.91%) 762 (96.58%)    42,727 ( 2.46%)  95 (50.80%)  UnknownFile???:MPIR_Init_thread
 95,957 (48.97%) 601 (99.83%) 1,815,292 (93.91%) 761 (96.45%)    42,727 ( 2.46%)  95 (50.80%)  UnknownFile???:PMPI_Init
 95,109 (48.54%) 597 (99.17%)   123,965 ( 6.41%) 629 (79.72%)    28,856 ( 1.66%)  32 (17.11%)  UnknownFile???:MPIR_T_env_init
...
--------------------------------------------------------------------------------
-- Auto-annotated source: memoryleak_mpi.c
--------------------------------------------------------------------------------
curB             curBk        totB               totBk        totFdB             totFdBk
...<snipped>...
      .            .                  .            .                  .           .           void f(void)
      .            .                  .            .                  .           .           {
100,000 (51.03%)   1 ( 0.17%)   100,000 ( 5.17%)   1 ( 0.13%)         0           0              int* x = malloc(25000 * sizeof(int));
      .            .                  .            .                  .           .              x[25000] = 0;     // problem 1: heap block overrun
      .            .                  .            .                  .           .           }                    // problem 2: memory leak -- x not freed
      .            .                  .            .                  .           .
      .            .                  .            .                  .           .           int main(int argc, char **argv)
      .            .                  .            .                  .           .           {
      .            .                  .            .                  .           .              int nproc, me;
 95,957 (48.97%) 601 (99.83%) 1,811,132 (93.70%) 757 (95.94%)    42,727 ( 2.46%) 93 (49.73%)     MPI_Init(&argc, &argv);
      .            .                  .            .                  .           .              MPI_Comm_size(MPI_COMM_WORLD, &nproc);
      .            .                  .            .                  .           .              MPI_Comm_rank(MPI_COMM_WORLD, &me);
100,000 (51.03%)   1 ( 0.17%)   100,000 ( 5.17%)   1 ( 0.13%)         0           0              f();
      0            0                  0            0          1,676,628 (96.52%) 70 (37.43%)     MPI_Finalize();
      .            .                  .            .                  .           .              return 0;
      .            .                  .            .                  .           .           }
```

If you generate output in Massif format, you need to use the
`ms_print` tool.

You can use Valgrind4hpc instead of Valgrind:

```
$ valgrind4hpc -n 8 -v "--xtree-memory=full" -o xtmemory.kcg ./memoryleak_mpi
```

Separate output files (`xtmemory.kcg.<pid>`) will be generated for
different MPI tasks.

### Memory leaks from execution trees

You can get a similar profile result for memory leaks. For this you
use the `--xtree-leak=yes` flag:

```
$ module rm darshan

$ srun -n 8 valgrind --xtree-leak=yes --xtree-leak-file=xtleak.%q{SLURM_JOB_ID}.%{SLURM_PROCID}.kcg ./memoryleak_mpi

$ callgrind_annotate --inclusive=yes --sort=RB:100,PB:100,IB:100,DB:100 xtleak.32282016.0.kcg
```

Here, `RB`, `PB`, `IB`, `DB`, etc. are for Reachable Bytes, Possibly
lost Bytes, Indirectly lost Bytes, Definitely lost Bytes, etc.

You can use Valgrind4hpc for this, too:

```
$ valgrind4hpc -n 8 -v "--xtree-leak=yes" -o xtleak.kcg ./memoryleak_mpi
```
