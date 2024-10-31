# Sanitizers4hpc with GPU codes

Example code:

-   `main.cc` and `memcheck_demo.cu`: Santizers4hpc with Compute Sanitzer's Memcheck example code

## `main.cc` and `memcheck_demo.cu`

This is a simple MPI adaptation with a Nvidia's Compute Sanitizer
example code,
[`memcheck_demo.cu`](https://github.com/NVIDIA/compute-sanitizer-samples/blob/master/Memcheck/memcheck_demo.cu).
The code is for using Compute Sanitizer's Memcheck tool.

```
$ salloc -A <project> -C gpu -N 1 --gpus-per-node=4 -q debug -t 10 ...
...

$ cc -fsanitize=thread -g -O1 -fopenmp buggyreduction_mpiomp.c -o buggyreduction_mpiomp
$ CC -c -g main.cc
$ nvcc -Xcompiler -rdynamic -lineinfo -c memcheck_demo.cu
$ CC -o memcheck_demo main.o memcheck_demo.o

$ module load sanitizers4hpc

$ sanitizers4hpc -l "-n 4 -c 32 --cpu-bind=cores --gpus-per-task=1 --gpu-bind=none" -m ${CUDA_HOME}/compute-sanitizer/compute-sanitizer -f -- ./memcheck_demo
RANKS: <2,3>
...
Saved host backtrace up to driver entry point at error
    #0 0x2eae6f in /usr/local/cuda-12.2/compat/libcuda.so.1
    #1 0xd8f0 in /home/jenkins/src/gtlt/cuda/gtlt_cuda_query.c:325:gtlt_cuda_pointer_type /opt/cray/pe/lib64/libmpi_gtl_cuda.so.0
...
RANKS: <0-1>
...
Saved host backtrace up to driver entry point at error
    #0 0x2eae6f in /usr/local/cuda-12.2/compat/libcuda.so.1
    #1 0xd8f0 in /home/jenkins/src/gtlt/cuda/gtlt_cuda_query.c:325:gtlt_cuda_pointer_type /opt/cray/pe/lib64/libmpi_gtl_cuda.so.0
...
```

The `-f` flag is needed to bypass the `santizers4hpc`'s  requirement
that the executable is instrumented for a LLVM Sanitizer. This
example code is basically a CUDA code that is not instrumented for
a LLVM Sanitizer.

Aggregation of output will improve in CPE/24.07.
