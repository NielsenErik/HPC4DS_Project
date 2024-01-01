#!/bin/bash

#PBS -l select=2:ncpus=4:mem=2gb

#set max execution time
#PBS -l walltime=1:00:00

#imposta la coda di esecuzione
#PBS -q short_cpuQ

module load mpich-3.2 # Load the required module
mpirun.actual -n 8 ./HPC4DS_Project/executables/GTO_parallel_