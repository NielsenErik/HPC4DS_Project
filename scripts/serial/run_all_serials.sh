#!/bin/bash
for ((DIM = 256; DIM <= 1024; DIM *= 2)); do # Loop for DIM with powers of 2
    qsub ./scripts/serial/run_serial_${DIM}.sh # Submit the job
done