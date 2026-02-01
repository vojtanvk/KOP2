#!/bin/bash

dir=$1

dst=black_box/$(basename "$dir")
mkdir -p "$dst"
rm -fr "$dst"/results.stats

for path in "$dir"/*.mwcnf; do
    (
        # echo $path

        path_base=$(basename $path)
        path_clean=${path_base:0:-6}
        grep_arg=${path_base:1:-6}

        
        cp rng_state "$dst"/rng-"$path_clean"
        
        my_rng="$dst"/rng-"$path_clean"

        opt_dir=opt/$(basename "$dir")

        opt=$(cat $opt_dir/$path_clean | cut -d' ' -f1)
        output=$(./build/annealer_sim -file $path -rng_start $my_rng -rng_save $my_rng -opt $opt | tail -n 2)
        reps=($(echo "$output" | cut -d' ' -f3,5))
        found_opt=$(echo $output | cut -d' ' -f6)
        
        echo "$dir"-opt.dat,"$grep_arg","$opt","$path_clean","$found_opt","${reps[0]}","${reps[1]}"

        if [ "$opt" -gt "$found_opt" ]; then
            # echo "Failure for file $path: Expected optimal solution $opt but found $found_opt"
            echo "$output" $path_clean
            echo "${grep_arg},${reps[0]},${reps[1]},0" >> "$dst"/results.stats
        else
            # echo "Success for file $path: Expected optimal solution $opt and found $found_opt"
            echo "${grep_arg},${reps[0]},${reps[1]},1" >> "$dst"/results.stats
        fi


        rm -fr $my_rng
    ) &
done
