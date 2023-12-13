#!/bin/bash

checkDir() #function which will verify if the directory indicated by the user exists
{

    if [ ! -d $1 ]; then
        echo "Directory not found"
        exit 1
    fi
    csv_file=$1
    if [ ! -f "$csv_file/data.csv" ]
    then
        echo "File not found"
    fi
   
}
checkEOut()
{
    cur_dir=`dirname pwd`/progc
    E_out=0 #does exist .out?
    cd "$cur_dir"
    
    for entry in *
    do
        if [ -x "$entry"  ]
        then
            E_out=1
        fi
    done
    if [ $E_out -eq 0 ]
    then
        `make`
    fi
    cd ..
}

if [ "$#" -eq 0 ]; then #check if any parameter was passed
    echo "No arguments provided"
    exit 1
fi

all_args=$* 
input_dir=$1


checkDir "$input_dir"
checkEOut 

for i in $all_args
do
    if [ "$i" = "-h" ]  #je comprends pas comme pq -eq et == marche pas mais = magiquement marche
    then
        echo "all future command list"
        exit 0
    fi
done








