- `main.cc` and `memcheck_demo.cu`: Santizers4hpc with Compute Sanitzer's Memcheck example code
    ```
    CC -c -g main.cc
    nvcc -Xcompiler -rdynamic -lineinfo -c memcheck_demo.cu
    CC ${sopts} main.o memcheck_demo.o
    ```
