#!/bin/bash
for ((N = 2; N < 64; N *= 2)); do # Loop for N with powers of 2
    for ((DIM = 256; DIM <= 1024; DIM *= 2)); do # Loop for DIM with powers of 2
        qsub ./scripts/parallel_${N}/run_parallel_${N}_${DIM}.sh # Submit the job
    done
done