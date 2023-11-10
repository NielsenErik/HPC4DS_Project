#!/bin/bash

#PBS -l select=1:ncpus=2:mem=2gb

#set max execution time
#PBS -l walltime=0:10:00

#imposta la coda di esecuzione
#PBS -q short_cpuQ

module load mpich-3.2 # Load the required module
mpirun.actual -n 2 ./executables/GTO_parallel_