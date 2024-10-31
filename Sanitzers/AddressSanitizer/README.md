# AddressSanitizer (ASAN)

Example codes:

-   `illegalmemoryaccess.cpp`
-   `use-after-free.c`: from [https://github.com/google/sanitizers/wiki/AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer)
-   `example_UseAfterFree.cc`: from [https://clang.llvm.org/docs/AddressSanitizer.html](https://clang.llvm.org/docs/AddressSanitizer.html)

## `illegalmemoryaccess.cpp`

This code attempts to write outside of the allocated block ("heap
over"). In this example, the GNU compiler is used, but any compiler
that supports ASAN can be used. Build with the `-fsanitize=address`
flag:

```
$ g++ -O0 -g -fsanitize=address -o illegalmemoryaccess illegalmemoryaccess.cpp
```

Run:

```
$ ./illegalmemoryaccess
=================================================================
==2267569==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x604000000038 at pc 0x0000004009df bp 0x7ffe9e373680 sp 0x7ffe9e373678
WRITE of size 4 at 0x604000000038 thread T0
    #0 0x4009de in main /pscratch/sd/e/elvis/addresssanitizer/illegalmemoryaccess.cpp:7
    #1 0x7fbf17c3c24c in __libc_start_main (/lib64/libc.so.6+0x3524c)
    #2 0x4008b9 in _start ../sysdeps/x86_64/start.S:120

0x604000000038 is located 0 bytes to the right of 40-byte region [0x604000000010,0x604000000038)
allocated by thread T0 here:
    #0 0x7fbf188bba88 in operator new[](unsigned long) (/usr/lib64/libasan.so.8+0xbba88)
    #1 0x40097e in main /pscratch/sd/e/elvis/addresssanitizer/illegalmemoryaccess.cpp:4
    #2 0x7fbf17c3c24c in __libc_start_main (/lib64/libc.so.6+0x3524c)

SUMMARY: AddressSanitizer: heap-buffer-overflow /pscratch/sd/e/elvis/addresssanitizer/illegalmemoryaccess.cpp:7 in main
Shadow bytes around the buggy address:
  0x0c087fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c087fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c087fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c087fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c087fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c087fff8000: fa fa 00 00 00 00 00[fa]fa fa fa fa fa fa fa fa
  0x0c087fff8010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c087fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c087fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c087fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c087fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  ...
  Right alloca redzone:    cb
==2267569==ABORTING
```

-   Heap-buffer-overflow for attempting to write 4 bytes outside
    of the allocated memory block at line 7 of `illegalmemoryaccess.cpp`
-   Memory block in question: 40 byte region
    `[0x604000000010,0x604000000038)`, allocated at line 4
-   Shadow bytes
    -   Mapped to `[0xc087fff8002,0xc087fff8007)` via `Shadow = (Mem >> 3) + 0x7fff8000`
    -   `00 00 00 00 00`: 5*8 bytes = 40 bytes
    -   `[fa]`: attempted to write to the heap left redzone (`fa`)

