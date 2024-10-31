# MemorySanitizer (MSAN)

Example codes:

-  `umr.cc`: From [https://github.com/google/sanitizers/wiki/MemorySanitizer](https://github.com/google/sanitizers/wiki/MemorySanitizer)
-  `umr2.cc`: To track origins of unitialized values; from [Clang MemorySanitizer page](https://clang.llvm.org/docs/MemorySanitizer.html)

## `umr.cc`

The code doesn't initialize `a[1]` but uses its value in a conditional
statement.

The GNU compilers don't support MSAN, so use a different compiler.
This example uses `PrgEnv-cray`. Compile with `-fsanitize=memory`.

```
$ CC -fsanitize=memory -g -O1 -o umr umr.cc

$ export MSAN_OPTIONS="allow_addr2line=true"

$ ./umr         # no argument here so argc in the program is 1
==578284==WARNING: MemorySanitizer: use-of-uninitialized-value
    #0 0x2cf202 in main /pscratch/sd/e/elvis/sanitizers/umr.cc:6:7
    #1 0x7fc4fa63e24c in __libc_start_main (/lib64/libc.so.6+0x3524c)
    #2 0x24e4b9 in _start /home/abuild/rpmbuild/BUILD/glibc-2.31/csu/../sysdeps/x86_64/start.S:120

SUMMARY: MemorySanitizer: use-of-uninitialized-value /pscratch/sd/e/elvis/sanitizers/umr.cc:6:7 in main
Exiting
```

The `MSAN_OPTIONS` environment variable is set as the source line
info in error messages is not displayed in this programming
environment.
