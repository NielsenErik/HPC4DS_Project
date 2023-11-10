#!/bin/bash
scripts/del.sh
module load mpich-3.2 # Load the required module

for ((N=1; N<=32; N*=2)); do
    for ((DIM=1; DIM<=1024; DIM*=2)); do
        mpicc -g -Wall -o ./executables/GTO_serial_${N}_${DIM} -DN=$N -DDIM=$DIM ./C_files/GTO_serial.c ./C_files/common_functions.c ./C_files/optimization_functions.c -lm
        cp ./scripts/serial/run_serial_template.sh ./scripts/serial/run_serial_${N}_${DIM}.sh # Create a new file based on the template
        echo -n "${N}_${DIM}" >> ./scripts/serial/run_serial_${N}_${DIM}.sh

        if ((N >= 2)); then
            mpicc -g -Wall -fopenmp -o ./executables/GTO_parallel_${N}_${DIM} -DN=$N -DDIM=$DIM ./C_files/GTO_parallel.c ./C_files/common_functions.c ./C_files/optimization_functions.c -lm
            cp ./scripts/parallel_2/run_parallel_template_2.sh ./scripts/parallel_2/run_parallel_2_${N}_${DIM}.sh # Create a new file based on the template
            echo -n "${N}_${DIM}" >> ./scripts/parallel_2/run_parallel_2_${N}_${DIM}.sh

            if ((N >= 4)); then
                cp ./scripts/parallel_4/run_parallel_template_4.sh ./scripts/parallel_4/run_parallel_4_${N}_${DIM}.sh # Create a new file based on the template
                echo -n "${N}_${DIM}" >> ./scripts/parallel_4/run_parallel_4_${N}_${DIM}.sh

                if ((N >= 8)); then
                    cp ./scripts/parallel_8/run_parallel_template_8.sh ./scripts/parallel_8/run_parallel_8_${N}_${DIM}.sh # Create a new file based on the template
                    echo -n "${N}_${DIM}" >> ./scripts/parallel_8/run_parallel_8_${N}_${DIM}.sh

                    if ((N >= 16)); then
                        cp ./scripts/parallel_16/run_parallel_template_16.sh ./scripts/parallel_16/run_parallel_16_${N}_${DIM}.sh # Create a new file based on the template
                        echo -n "${N}_${DIM}" >> ./scripts/parallel_16/run_parallel_16_${N}_${DIM}.sh
                    fi
                fi
            fi
        fi
    done
done