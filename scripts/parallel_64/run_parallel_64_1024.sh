#!/bin/bash

#PBS -l select=4:ncpus=16:mem=2gb

#set max execution time
#PBS -l walltime=0:20:00

#imposta la coda di esecuzione
#PBS -q short_cpuQ

module load mpich-3.2 # Load the required module
export OMP_PLACES=threads
mpirun.actual -n 64 HPC4DS_Project/executables/GTO_parallel_1024