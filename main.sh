#!/bin/bash


checkDir() #function which will verify if the directory indicated by the user exists
{

    if [ ! -d $1 ]; then
        echo "Data directory not found"
        exit 1
    fi
	
    csv_file=$1
    
	if [ ! -f "$csv_file/data.csv" ]
    then
        echo "Data file not found"
	exit 1
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



d1Flag() #this function will display the top 10 drivers who have highest number of trips
{
    grep '^[^;]*;1;' data/data.csv | 
    awk -v OFS=';' -F';' '{ journey_count[$6]++ } END {for (driver in journey_count) print driver, journey_count[driver] } ' | 
    sort -t';' -nrk2,2 | 
    head -n10 > temp/temp_d1flag.csv  
}

d2Flag() # this function displays the 10 drivers with the longest rides
{
    cut -d';' -f5,6 data/data.csv |
    awk -v OFS=';' -F';'  'NR>1 {journey_lengths_sum[$2]+=$1} END {for (driver in journey_lengths_sum) print driver, journey_lengths_sum[driver]}' |
    sort -t';' -nrk2,2 |
    head -n10 > temp/temp_d2flag.csv
}

tFlag()
{
    echo caca
}
    
lFlag() #this function will display the top10 longest trips by Route ID and then the distance
{
    cut -d';' -f1,5 data/data.csv |
    awk -v OFS=';' -F';' 'NR>1 { route_lengths_sum[$1]+= $2;} END {for (route_id in route_lengths_sum) {print route_id, route_lengths_sum[route_id];}}' |
    sort -t";" -k2nr |
    head -n10 > temp/temp_lflag.csv
}






# Main

if [ $# -eq 0 ]; then #check if any parameter was passed
    echo "No arguments provided"
    exit 1
fi

#check if the temp folder exists, if so empty it, otherwise create it
if [ ! -d temp ];
then
	mkdir temp
else
    find temp -mindepth 1 -delete
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

	"-h") 
    
        echo "future: command list"

    	exit 0;;

      
	"-d1")
	
    	strt_time=$(getTime)
    
    	d1Flag

    	echo "Duration of the task's execution: `taskDuration $strt_time` seconds"

    	exit 0;;

     
	"-d2") 
    
    	strt_time=$(getTime)
    
		d2Flag
    
    	echo "Duration of the task's execution: `taskDuration $strt_time` seconds"

        exit 0;;


    "-t") 

        strt_time=$(getTime)
    
		tFlag
    
    	echo "Duration of the task's execution: `taskDuration $strt_time` seconds"

        exit 0;;


    "-l")

        strt_time=$(getTime)
			
        lFlag
			
	    echo "Duration of the task's execution: `taskDuration $strt_time` seconds"

        exit 0;;

    esac
done
