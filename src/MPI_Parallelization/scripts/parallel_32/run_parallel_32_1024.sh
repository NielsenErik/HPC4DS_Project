#!/bin/bash

#PBS -l select=4:ncpus=8:mem=2gb -l place=pack:excl

#set max execution time
#PBS -l walltime=0:20:00

#imposta la coda di esecuzione
#PBS -q short_cpuQ

module load mpich-3.2 # Load the required module
mpirun.actual -n 32 ./HPC4DS_Project/executables/GTO_parallel_1024