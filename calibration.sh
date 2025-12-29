#!/bin/sh

path=$1

filename=$(basename "$path")
grep_arg=${filename:1:-6}

opt=$(grep -w $grep_arg .data/wuf20-71-M-opt.dat | cut -d' ' -f2)

simmulated_opt=$(./build/annealer_sim --file "$path" -rng_state "rng_state" -rng_save "rng_state" ${@:2} | cut -d' ' -f1)
while [ $simmulated_opt != $opt ]; do
    echo "Retrying $filename: simulated $simmulated_opt vs opt $opt"
    simmulated_opt=$(./build/annealer_sim --file "$path" -rng_state "rng_state" -rng_save "rng_state" ${@:2} | cut -d' ' -f1)
done