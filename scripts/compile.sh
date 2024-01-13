#!/bin/bash
scripts/del.sh
module load mpich-3.2 # Load the required module

for ((DIM = 256; DIM <= 1024; DIM *= 2)); do
    # mpicc -g -Wall -o ./executables/GTO_serial_${DIM} -DDIM=$DIM ./C_files/GTO_serial.c ./C_files/common_functions.c ./C_files/optimization_functions.c -lm
    # cp ./scripts/serial/run_serial_template.sh ./scripts/serial/run_serial_${DIM}.sh # Create a new file based on the template
    # echo -n "${DIM}" >> ./scripts/serial/run_serial_${DIM}.sh

    for ((processes = 2; processes <= 64; processes *= 2)); do
        mpicc -g -Wall -o ./executables/GTO_parallel_${DIM} -DDIM=$DIM ./C_files/GTO_parallel.c ./C_files/common_functions.c ./C_files/optimization_functions.c -lm -fopenmp
        cp ./scripts/parallel_${processes}/run_parallel_template_${processes}.sh ./scripts/parallel_${processes}/run_parallel_${processes}_${DIM}.sh # Create a new file based on the template
        echo -n "${DIM}" >> ./scripts/parallel_${processes}/run_parallel_${processes}_${DIM}.sh
    done
done