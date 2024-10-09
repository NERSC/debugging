# Sanitizers

See NERSC docs page, [Sanitizers and Sanitizers4hpc](https://docs.nersc.gov/tools/debug/sanitizers/).

## Introduction

LLVM Sanitizers are a group of debugging tools for detecting various
kinds of bugs in C and C++ codes. There are multiple tools, including
AddressSanitizer, LeakSanitizer, ThreadSanitizer, MemorySanitizer,
each with a specific debugging capability.

A sanitizer consists of a compiler instrumentation module and a
runtime library. To use a sanitizer, you first build an executable
instrumented for the sanitizer, by specifying a compile flag. When
the instrumented executable is run, the runtime intercepts relevant
operations and inspects them. When it detects a problem, it generates
a warning message.

Because of the instrumentation and the way how the debugging work
is played out, memory usage can become several times bigger and the
instrumented code can run several times slower. Therefore, it is
important to rebuild your code without instrumentation after debugging
is complete.

### Supported Compilers

These tools can be used with more than just LLVM compilers: they
are compatible with all compilers provided on Perlmutter, except
the Nvidia compiler.

You don't need to change the way you compile your MPI code in order
to use these tools (i.e., you can still use the Cray compiler
wrappers `cc`/`CC`/`ftn` as normal).  For a non-MPI code, the
following C/C++ [base compilers](../../../development/compilers/base.md)
can be used, too.

| GNU | Cray | Intel | AOCC | LLVM |
|:---:|:----:|:-----:|:----:|:----:|
| `gcc`/`g++` | `craycc`/`craycxx` | `icx`/`icpx` | `clang`/`clang++` | `clang`/`clang++` |

Note that Intel's `icc` and `icpc` do not work for the sanitizer
tools as they are not Clang-based.

### Sanitizer Flags

These compilers accept many LLVM sanitizer compile flags. Use the
ones for your needs. For example, you don't have to instrument the
entire code. Instead, you can exclude certain functions or source
files from instrumentation with the `-fsanitize-blacklist=` or
`-fsanitize-ignorelist=` option.

Runtime behavior of a tool can be controlled by setting the santizer
environment variable to certain runtime flags. The variable is
`ASAN_OPTIONS` for AddressSantizer, `LSAN_OPTIONS` for LeakSanitizer,
`TSAN_OPTIONS` for ThreadSantizer, `MSAN_OPTIONS` for MemorySanitizer,
etc.

You can find compile and runtime flags at the following web pages:

- [AddressSanitizer
  Flags](https://github.com/google/sanitizers/wiki/AddressSanitizerFlags)
- [ThreadSanitizer
  Flags](https://github.com/google/sanitizers/wiki/ThreadSanitizerFlags)
- [Sanitizer Common
  Flags](https://github.com/google/sanitizers/wiki/SanitizerCommonFlags)

## Sanitizers4hpc

HPE's `Sanitizers4hpc` is an aggregation tool to collect and analyze
LLVM Sanitizer output from a distributed-memory parallel (e.g.,
MPI) code at scale.  It makes sanitizer's result easier to understand,
by presenting output by group of MPI tasks sharing the same pattern.

Currently it supports

- AddressSanitizer
- LeakSanitizer
- ThreadSanitizer

with the Cray and the GNU compilers. It also supports Nvidia Compute
Sanitizer's Memcheck tool for CUDA codes (an example below).

To run an app with the tool, load the `sanitizers4hpc` module and
then launch as follows:

```
sanitizers4hpc <sanitizers4hpc options> -- ./a.out <application arguments>
```

