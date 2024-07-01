- LeakSanitizer as a stand-alone sanitizer
    ```
    $ clang -fsanitize=leak -g -O0 memory-leak.c

    $ ./a.out
    ```

- LeakSanitizer run under AddressSanitizer:
    ```
    $ clang -fsanitize=address -g memory-leak.c

    $ ASAN_OPTIONS=detect_leaks=1 ./a.out
    ```
