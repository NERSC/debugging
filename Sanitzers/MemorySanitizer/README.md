MemorySantizer example codes:

-  `umr.cc`: From [https://github.com/google/sanitizers/wiki/MemorySanitizer](https://github.com/google/sanitizers/wiki/MemorySanitizer)
    ```
    $ clang -fsanitize=memory -fPIE -pie -fno-omit-frame-pointer -g -O2 umr.cc
    ```
-  `umr2.cc`: To track origins of unitialized values; from [Clang MemorySanitizer page](https://clang.llvm.org/docs/MemorySanitizer.html)
    ```
    $ clang -fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer -g -O2 umr2.cc
    ```
