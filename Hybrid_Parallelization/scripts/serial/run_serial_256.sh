#!/bin/bash

#PBS -l select=1:ncpus=1:mem=2gb -l place=pack:excl

#set max execution time
#PBS -l walltime=1:00:00

#imposta la coda di esecuzione
#PBS -q short_cpuQ

module load mpich-3.2 # Load the required module
mpirun.actual -n 1 HPC4DS_Project/executables/GTO_serial_256