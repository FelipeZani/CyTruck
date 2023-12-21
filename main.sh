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

	tot_sec=$(date +%s)
	
	echo "$tot_sec"

}

taskDuration() #this function will receive an initial time passed by argument
#and computate the duration of execution of a certain task
{

	init_time=$1
	end_time=$(date +%s)
	dur_exec=$((${end_time} - ${init_time}))
	
	echo $dur_exec
}

d1Flag() #this function will display the top 10 drivers who have highest number
 #of trips
{
    grep ";1;" data.csv>temp.csv # this line take all the first step of all trips and send them to temp.csv
    awk -F ';' '{ count[$6]++ } END { for (i in count) print i" " count[i] }' temp.csv > temp2.csv #  this line counts the number of occurrences of each unique value in the sixth column of the CSV file temp.csv, then writes these counts to a new file called temp2.csv
    sort -k3,3 -rn temp2.csv | head -n10 # this line print ce ten drivers with the highest number of trips of the temp.csv file
    
    mv temp.csv temp2.csv ../temp #clean the folder
}

d2Flag() # this function displays the 10 drivers with the longest rides
{
    awk -F ';' '{sum[$6]+=$5} END {for (i in sum) print i" "sum[i]}' data.csv > temp.csv #this line sum up the kilometers for every drivers ans sand them to temp.csv
    sort -k3,3 -rn temp.csv | head -n10 # this line print ce ten drivers with the longest rides of the temp.csv file
    
    mv temp.csv ../temp #clean the folder
}

lFlag() #this function will display the top10 longest trips by Route ID and then the distance
{
    awk -F';' '{ 
        sum[$1]+= $5;
      }
      END{
        for (driver in sum){
          print driver , sum[driver];
        }
      }
    ' data.csv | sort -t" " -k2nr | head -n10 > templFlag.csv
    cat templFlag.csv
    mv templFlag.csv ../temp
    #the block of code above will compute the distance of each trip, storing them in a array and then increasing the distance
    # if a similar route ID is found and finally the code will display each driver in the trip and the distance from the array
    
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

	"-h") echo "future: command list"
    		exit 0;;

      
	"-d1") cd data
	
    		strt_time=$(getTime)
    
    		d1Flag
    
        	echo "Duration of the task's execution: `taskDuration $strt_time`seconds"
	
        	exit 0;;

     
	"-d2") cd data
    
    		strt_time=$(getTime)
    
    		d2Flag
    
    		echo "Duration of the task's execution: `taskDuration $strt_time`seconds"
    
    		cd ..;;
    "-l") cd data
	
            strt_time=$(getTime)
			
            lFlag
			
	        echo "Duration of the task's execution: `taskDuration $strt_time`seconds"
    esac
done





