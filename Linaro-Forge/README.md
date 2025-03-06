# Linaro Forge training materials

See

- `/global/cfs/cdirs/training/2025/linaro-forge-training`
- `/global/cfs/cdirs/training/2024/Forge_Mar2024`

# Build

## Debugging

```
$ cd correctness

$ cd core-files
$ make -f core.makefile clean
$ make -f core.makefile

$ cd ..

$ cd debug
$ make clean
$ make

$ cd ..

$ cd gpu-nvidia-mmult
$ make clean
$ make

$ cd ../..
```

## Profiling

```
$ cd performance
$ ml python
$ make -f mmult_py.makefile clean
$ make -f mmult_py.makefile
```
