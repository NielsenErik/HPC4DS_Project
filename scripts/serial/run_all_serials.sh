#!/bin/bash
for ((N = 1; N <= 32; N *= 2)); do # Loop for N with powers of 2
    for ((DIM = 1; DIM <= 1024; DIM *= 2)); do # Loop for DIM with powers of 2
        qsub ./scripts/run_serial/run_serial_${N}_${DIM}.sh # Submit the job
    done
done