#!/bin/bash

path=$1
loops=$2

dir=$(echo "$path" | cut -d'/' -f1-2)
echo "$dir"

filename=$(basename "$path")
grep_arg=${filename:1:-6}

for _ in $(seq 0 $loops); do
    opt=$(grep -w $grep_arg "$dir"-opt.dat | cut -d' ' -f2)

    simulated_output=$(./build/annealer_sim -file "$path" -rng_start rng_state -rng_save rng_state)

    simmulated_opt=$(echo "$simulated_output" | tail -n 1 | cut -d' ' -f1)

    stats=$(echo "$simulated_output" | head -n 1 | cut -d' ' -f3)

    if [ "$opt" != "$simmulated_opt" ]; then
        echo "Failure for file $path: Expected optimal solution $opt but found $simmulated_opt"
        echo "-1, -1" >> calibs/random/rand_"$grep_arg"_"$loops".stat
    else
        echo "Success for file $path: Expected optimal solution $opt and found $simmulated_opt"
        echo "$stats" >> calibs/random/rand_"$grep_arg"_"$loops".stat
    fi
done