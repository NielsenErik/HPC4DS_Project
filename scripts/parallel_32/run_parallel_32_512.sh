#!/bin/bash

#PBS -l select=1:ncpus=32:mem=2gb -l place=pack:excl

#set max execution time
#PBS -l walltime=0:20:00

#imposta la coda di esecuzione
#PBS -q short_cpuQ

module load mpich-3.2 # Load the required module
export OMP_NESTED=TRUE
export OMP_PLACES=threads
export OMP_SCHEDULE="auto"
mpiexec -np 32 HPC4DS_Project/executables/GTO_parallel_512