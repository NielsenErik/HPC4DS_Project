#!/bin/bash

#PBS -l select=1:ncpus=64:mem=2gb

#set max execution time
#PBS -l walltime=0:20:00

#imposta la coda di esecuzione
#PBS -q short_cpuQ

module load mpich-3.2 # Load the required module
export OMP_NESTED=TRUE
mpiexec -np 64 HPC4DS_Project/executables/GTO_parallel_1024