#!/bin/bash

checkDir() #function which will verify if the directory indicated by the user exists
{
    dir=$(dirname "$1")

    if [ ! -d "$dir" ]; then
        echo "Directory not found"
        exit 1
    fi
}



all_args=$* 
input_dir=$1


checkDir "$input_dir"

for i in $all_args
do
    if [ "$i" = "-h" ]  #je comprends pas comme pq -eq et == marche pas mais = magiquement marche
    then
        echo "all future command list"
        exit 0
    fi
done


