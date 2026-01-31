#!/bin/bash

dir=$1
dst=.data/opt_results

dir_base=$(basename "$dir")

for path in "$dir"/*; do

    read -r line < "$path"
    opt=$(echo "$line" | cut -d' ' -f1)

    if [ $opt -eq 0 ]; then
        echo "Invalid optimization result in $dir_base/$(basename "$path")"
        mkdir -p .data/unknown_results/$dir_base
        # cp .data/$dir_base/$(basename "$path").mwcnf .data/unknown_results/$dir_base/
    # else
    #     echo $(basename "$path") >> "$dir"/"$dir_base"-opt.txt
    #     echo $line >> "$dir"/"$dir_base"-opt.txt
    fi

done