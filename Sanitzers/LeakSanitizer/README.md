# LeakSanitizer (LSAN)

Example code:

-   `memory-leak.c`

## LeakSanitizer as a stand-alone sanitizer

`memory-leak.c` has a memory leak of 7 bytes as the memory block
pointed to by the pointer `p` is not freed before setting it to
`NULL` (0).

Here we try the `clang` compiler in the `PrgEnv-aocc` environment
to demonstrate that the tool works in the environment but you are
free to use a different compiler that supports LSAN. To build and
run:

```
$ clang -fsanitize=leak -g -O0 -o memory-leak memory-leak.c

$ ./memory-leak
=================================================================
==2335900==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 7 byte(s) in 1 object(s) allocated from:
    #0 0x55966653a842 in malloc /.../nersc/nersc-user-env/prgenv/llvm_src_17.0.6/compiler-rt/lib/lsan/lsan_interceptors.cpp:75:3
    #1 0x559666565898 in main /pscratch/sd/e/elvis/addresssanitizer/memory-leak.c:4:7
    #2 0x7efe8f83e24c in __libc_start_main (/lib64/libc.so.6+0x3524c) (BuildId: ddc393ac74ed8f90d4fdfff796432fbafd281e1b)

SUMMARY: LeakSanitizer: 7 byte(s) leaked in 1 allocation(s)
```

## LeakSanitizer run under AddressSanitizer:

LeakSanitizer can be combined with AddressSanitizer to get both
memory error and leak detection, too. Build with `-fsanitize=address`
but run the executable with the environment variable `ASAN_OPTIONS`
set to `detect_leaks=1`:

```
$ clang -fsanitize=address -g -o memory-leak memory-leak.c

$ ASAN_OPTIONS=detect_leaks=1 ./memory-leak
=================================================================
==2339511==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 7 byte(s) in 1 object(s) allocated from:
    #0 0x56040740afde in malloc /.../nersc/nersc-user-env/prgenv/llvm_src_17.0.6/compiler-rt/lib/asan/asan_malloc_linux.cpp:69:3
    #1 0x560407447a68 in main /pscratch/sd/e/elvis/addresssanitizer/memory-leak.c:4:7
    #2 0x7fdab443e24c in __libc_start_main (/lib64/libc.so.6+0x3524c) (BuildId: ddc393ac74ed8f90d4fdfff796432fbafd281e1b)

SUMMARY: AddressSanitizer: 7 byte(s) leaked in 1 allocation(s)
```
