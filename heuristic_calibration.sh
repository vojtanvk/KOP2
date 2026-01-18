#!/bin/sh

dir=$1

echo "$dir"

for path in "$dir"/*.mwcnf; do

    filename=$(basename "$path")
    grep_arg=${filename:1:-6}


    opt=$(grep -w $grep_arg .data/wuf20-71-M-opt.dat | cut -d' ' -f2)

    for coef in $(seq 0 )

    for min_temp

    for init_temp

    for inner_loops

    simmulated_opt=$(./build/annealer_sim -file "$path" -rng_state "rng_state" -rng_save "rng_state" ${@:2} | cut -d' ' -f1)

    if [ "$opt" != "$simmulated_opt" ]; then
        echo "Failure for file $path: Expected optimal solution $opt but found $simmulated_opt"
    else
        echo "Success for file $path: Expected optimal solution $opt and found $simmulated_opt"
    fi
done