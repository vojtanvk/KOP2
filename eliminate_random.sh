#!/bin/bash

path=$1

dir=$(echo "$path" | cut -d'/' -f1-2)

filename=$(basename "$path")
file=${filename:0:-6}
grep_arg=${filename:1:-6}

output_dir=calibs/random/"$file"
rng_file="$output_dir"/rng_state

mkdir -p "$output_dir"

cp rng_state $rng_file

loops=1000

output_file="$output_dir"/rand_"$grep_arg"_"$loops".stats
rm -f "$output_file"

for _ in $(seq 0 $loops); do
    opt=$(grep -w $grep_arg "$dir"-opt.dat | cut -d' ' -f2)

    simulated_output=$(./build/annealer_sim -file "$path" -rng_start $rng_file -rng_save $rng_file -temp 200 -inner_iters 500 -min_temp 0.01)

    simmulated_opt=$(echo "$simulated_output" | tail -n 1 | cut -d' ' -f1)

    stats=$(echo "$simulated_output" | head -n 1 | cut -d' ' -f3)

    if [ "$opt" != "$simmulated_opt" ]; then
        echo "Failure for file $path: Expected optimal solution $opt but found $simmulated_opt"
        echo "$stats,0" >> $output_file
    else
        echo "Success for file $path: Expected optimal solution $opt and found $simmulated_opt"
        echo "$stats,1" >> $output_file
    fi
done

rm -fr "$output_dir"/rng_state_"$grep_arg"_"$loops"