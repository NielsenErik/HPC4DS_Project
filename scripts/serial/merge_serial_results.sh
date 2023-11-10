#!/bin/bash
output_file="serial_results.txt"  # Name of the output file
[ -f "$output_file" ] && rm "$output_file" # Remove the existing output file if it exists

for ((N = 1; N <= 32; N *= 2)); do # Loop for N with powers of 2
    for ((DIM = 1; DIM <= 1024; DIM *= 2)); do # Loop for DIM with powers of 2
        for file in run_serial_${N}_${DIM}.sh.o*; do
            [ -e "$file" ] && cat "$file" >> "$output_file"
        done
    done
done

echo "All 'run_serial' files combined into $output_file"