#!/bin/bash

#PBS -l select=1:ncpus=8:mem=2gb -l place=pack:excl

#set max execution time
#PBS -l walltime=0:10:00

#imposta la coda di esecuzione
#PBS -q short_cpuQ

module load mpich-3.2 # Load the required module
mpiexec -np 8 ./HPC4DS_Project/executables/GTO_parallel_512