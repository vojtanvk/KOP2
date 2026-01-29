#!/bin/bash

dir=$1

for path in "$dir"/*.mwcnf; do
    (
        path_base=$(basename $path)
        path_clean=${path_base:0:-6}

        dst=opt/$(basename "$dir")
        mkdir -p "$dst"
        cp rng_state "$dst"/rng-"$path_clean"
        
        my_rng="$dst"/rng-"$path_clean"
        
        opt=-1

        echo $path

        for _ in $(seq 0 70); do
            output=$(./build/annealer_sim -file $path -rng_start $my_rng -temp 10 -inner_iters 500 -cooling 0.9 -min_temp 0.05 | tail -n 1)
            found_opt=$(echo $output | cut -d' ' -f1)

            if [ $opt -lt $found_opt ]; then
                echo $output > $dst/$path_clean
            fi
        done

        rm -fr $my_rng
    ) 
done