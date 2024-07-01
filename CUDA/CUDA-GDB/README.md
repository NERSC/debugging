CUDA-GDB example codes in the CUDA-GDB User Manual:

-   `bitreverse.cu`
    ```
    $ nvcc -g -G -o bitreverse bitreverse.cu

    $ cuda-gdb ./bitreverse
    ```
-   `autostep.cu`
    ```
    $ nvcc -g -G -o autostep.cu -o autostep

    $ cuda-gdb ./autostep
    ```
