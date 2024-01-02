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


getTime() #this function gets the local time of the computer and transforms it in seconds
{

	tot_sec=$(date +%s)

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



d1Flag() {
    grep '^[^;]*;1;' data/data.csv |
    awk -v OFS=';' -F';' '{ journey_count[$6]++ } END {for (driver in journey_count) print driver, journey_count[driver] } ' | 
    sort -t';' -nrk2,2 | 
    head -n10 > temp/temp_d1flag.csv
    
    # Create histogram data file
    awk -v OFS=';' -F';' '{print $2, $1}' temp/temp_d1flag.csv > temp/histogram_data.csv

    # Generate horizontal histogram using gnuplot
    gnuplot <<-EOF
        set terminal png size 1500,600
        set output 'images/histogram_d1.png'
        set title "Top 10 Drivers with Highest Number of Trips"
        set xlabel "Number of Trips"
        set ylabel "Driver Names"
        set style data histograms
        set style fill solid border -1
        set yrange [0:300]
        set xrange [0:10] reverse
        set boxwidth 0.8
        plot 'temp/histogram_data.csv' using 1:xtic(2) notitle
EOF
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
    cut -d';' -f3,4 data/data.csv |
    awk -v OFS=';' -F ';' 'NR>1 {town_count[$1]++; town_count[$2]++; start_town_count[$1]++} END { for (town in town_count) print town, town_count[town], start_town_count[town];}' > temp/temp_tflag_1.csv

    cd progc
    make -s compile 
    ./tflag ../temp/temp_tflag_1.csv ../temp/temp_tflag_2.csv
}

lFlag() #this function will display the top10 longest trips by Route ID and then the distance
{
    cut -d';' -f1,5 data/data.csv |
    awk -v OFS=';' -F';' 'NR>1 { route_lengths_sum[$1]+= $2;} END {for (route_id in route_lengths_sum) {print route_id, route_lengths_sum[route_id];}}' |
    sort -t";" -k2nr |
    head -n10 > temp/temp_lflag.csv
}

sFlag () #this function will create a .csv file with longest and shortest distance of a trip as well as its average for each trip (Route ID)  
{
	cd progc
	$(make buildSflag)
	return_make=$?
	if [ $return_make -ne 0 ]
	then
        echo "Program failled to build an executable"
		exit 1
    fi
	./sflag #execute the program which will computate the max and min distances and the average
	cd ../temp
	sort -t";" -k5,5 -rn sflag_data.csv > sflag_data2graphic.csv
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
    "-s")
        strt_time=$(getTime)

        sFlag

        echo "Duration of the task's execution: `taskDuration $strt_time` seconds"

        exit 0;;		


    esac
done
