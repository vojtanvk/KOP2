#!/bin/bash

path=$1

dir=$(echo "$path" | cut -d'/' -f1-2)
echo "$dir"

filename=$(basename "$path")
grep_arg=${filename:1:-6}

grep -w $grep_arg "$dir"-opt.dat | cut -d' ' -f2-
simulated_output=$(./build/annealer_sim -file "$path" -rng_start rng_state -rng_save rng_state)
echo "$simulated_output" | tail -n 1
