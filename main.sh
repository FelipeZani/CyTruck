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
		if [ $return_make -ne 0 ] #verification of the return of make command
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

taskDuration() #this function will receive an initial time passed by argument
#and computate the duration of execution of a certain task
{

	init_time=$1
	end_time=$(getTime)
	dur_exec=$((${end_time} - ${init_time}))
	
	echo $dur_exec
}

d1Flag() #this function will display the top 10 drivers who have highest number
 #of trips
{
    #count the occurences(of trips) of each driver and redirect it to the temp file
	awk -F ';' '{count[$6]++} END {for (i in count) print i ";" count[i]}' data.csv > temp.csv
    
    #concatenate the two colums inside and redirects it to temp2
    awk -F ';' '{ print $1 " " $2 }' temp.csv > temp2.csv
    
    #sort according the third field and in the decreasing order
    sort -k3,3 -rn temp2.csv | head -n10
    
    mv temp.csv temp2.csv ../temp #clean the folder
}

if [ $# -eq 0 ]; then #check if any parameter was passed
    echo "No arguments provided"
    exit 1
fi

# remove the temp folder
rm -rf temp

#check if the temp folder exists, otherwise creates it
if [ ! -d temp ];
then
	mkdir temp
fi

#check if the folder images exist and if it doesn't, create it
if [ ! -d images ]; 
then
	mkdir images
fi

all_args=$*
input_dir=$1

checkDir $input_dir
checkEOut 

for i in $all_args #print the help list
do
	case $i in
	"-d1") cd data
	
	 (d1Flag)
	 
	 cd ..;;
	
    "-h") echo "future: command list"
    
        exit 0;;
    esac
done





