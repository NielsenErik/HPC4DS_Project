#!/bin/bash
for ((N = 1; N <= 32; N *= 2)); do # Loop for N with powers of 2
    for ((DIM = 1; DIM <= 1024; DIM *= 2)); do # Loop for DIM with powers of 2
        qsub ./scripts/parallel_${N}/run_parallel_${N}_2_${DIM}.sh # Submit the job
    done
done