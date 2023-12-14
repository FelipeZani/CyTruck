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

checkEOut() #verify if the executable exists
{

    cur_dir=`dirname pwd`/progc
    E_out=0 
	
    cd $cur_dir
    
    for entry in * #search for an executable inside the progc folder
    do
        if [ -x $entry  ]
        then
            E_out=1
        fi
    done
	
    if [ $E_out -eq 0 ] 
    then
        `make`
		return_make=$?
		if [ $return_make -ne 0 ]#verification of the return of make command
		then
			echo "Program failled to build an executable"
			exit 1
		fi
    fi
	
    cd ..
	
}

getTime() #this function gets the local time of the computer and transforms it in seconds
{

	hour=`date +%H`
	minutes=`date +%M`
	secondes=`date +%S`
	tot_sec=$((${hour}*3600 + ${minutes}*60 + ${secondes}))
	
	echo "$tot_sec"

}

taskDuration()#this function will receive an initial time passed by argument and computate the duration of execution of a certain task
{

	init_time=$1
	end_time=$(getTime)
	dur_exec=$((${end_time} - ${init_time}))
	
	echo $dur_exec
}

if [ $# -eq 0 ]; then #check if any parameter was passed
    echo "No arguments provided"
    exit 1
fi

all_args=$*
input_dir=$1

checkDir $input_dir
checkEOut 

for i in $all_args #print the help list
do
    if [ "$i" = "-h" ]
    then
        echo "future: command list"
        exit 0
    fi
done





