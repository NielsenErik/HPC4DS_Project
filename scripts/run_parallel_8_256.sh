#!/bin/bash

export OMP_NESTED=TRUE
export OMP_PLACES=threads
mpiexec -np 8 ./executables/GTO_parallel_256