#!/bin/bash -l

#SBATCH -J linaro-forge-hands-on
#SBATCH -o stdout.%J.out
#SBATCH -e stderr.%J.err
#SBATCH -A ntrain7
#SBATCH -C cpu
#SBATCH --time=00:30:00
#SBATCH --nodes=1
#SBATCH -c 32
#SBATCH --reservation=forge_cpu

export SLURM_CPU_BIND="cores"

SIMPLEPATH=$FORGE_TRAINING/correctness/debug/simple
MMULTPATH=$FORGE_TRAINING/performance/mmult.py

module load forge

# Debug a simple MPI program
ddt --offline -o offline-debugging.html --break-at=simple.c:32 --break-at=simple.c:41 srun -n 4 $SIMPLEPATH

# Profile matrix multiplication example
#cd `dirname $MMULTPATH`
#map --profile srun -n 8 python3 $MMULTPATH -s 3072
