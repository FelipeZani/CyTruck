#!/bin/bash



checkFile() {
    if [ ! -f "$1" ]; then
        echo "Data file not found"
        exit 1
    fi
}

checkDir() {
    if [ ! -d "$(dirname "$1")" ]; then
        echo "Data directory not found"
        exit 1
    fi

    csv_file=$1
    checkFile "$csv_file"
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


#-d1 flag will display the top 10 drivers with the greatest number of trips "stepID"
#the function will look for the the first of each trip stepID in the whole data file, 
#and count the driver with the most stepIDs 
d1Flag() {
    grep '^[^;]*;1;' data/data.csv |
    awk -v OFS=';' -F';' '{ journey_count[$6]++ } END {for (driver in journey_count) print driver, journey_count[driver] } ' | 
    sort -t';' -nrk2,2 | 
    head -n10 > temp/temp_d1flag.csv
    
    # Create histogram data file
    awk -v OFS=';' -F';' '{print $2, $1}' temp/temp_d1flag.csv > temp/histogram_data.csv

    # Generate horizontal histogram using gnuplot
    gnuplot <<-EOF
	set terminal png size 600,1500
	set output 'images/histogram_d1.png'
	set label "Option -d1: Nb of routes = f(Driver)" at -1.5,95 rotate by 90 #In this line we had to set the exact coordinate and rotate for the title to be in the right place
	set ylabel "Number of Routes" offset 64
	set xlabel "Driver Names" rotate by 180
	set style data histograms
	set style fill solid border -1
	set yrange [0:250]
	set boxwidth 0.8
	# In the coming lines we are rotating all the tics to be readable after the final rotate
    	set xtic rotate 90
    	set ytic rotate 90 offset 61
    	set ytic 50
    
	plot 'temp/histogram_data.csv' using 1:xtic(2) notitle
	# xdg -openhistogram_d1.png 
EOF
	# Here we rotate the final picture, so it's going to be a horizontal histogram
    convert -rotate 90 images/histogram_d1.png images/histogram_d1.png
    
}


d2Flag() # this function displays the 10 drivers with the longest rides
{
    cut -d';' -f5,6 data/data.csv |
    awk -v OFS=';' -F';'  'NR>1 {journey_lengths_sum[$2]+=$1} END {for (driver in journey_lengths_sum) print driver, journey_lengths_sum[driver]}' |
    sort -t';' -nrk2,2 |
    head -n10 > temp/temp_d2flag.csv
    
    # Create histogram data file
    awk -v OFS=';' -F';' '{print $2, $1}' temp/temp_d2flag.csv > temp/histogram_d2_data.csv

    # Generate horizontal histogram using gnuplot
    gnuplot <<-EOF
        set terminal png size 600,1500
        set output 'images/histogram_d2.png'
	    set label "Option -d2: Nb of routes = f(Driver)" at -1.5,58000 rotate by 90
        set ylabel "Total Distance (km)" offset 64
        set xlabel "Driver Names" rotate by 180
        set style data histograms
        set style fill solid border -1
        set yrange [0:150000]
        set boxwidth 0.8
        set xtic rotate 90
    	set ytic rotate 90 offset 61
    	set ytic 30000
        plot 'temp/histogram_d2_data.csv' using 1:xtic(2) notitle
EOF
	convert -rotate 90 images/histogram_d2.png images/histogram_d2.png
}

tFlag()
{
    cut -d';' -f3,4 data/data.csv |
    awk -v OFS=';' -F ';' 'NR>1 {town_count[$1]++; town_count[$2]++; start_town_count[$1]++} END { for (town in town_count) print town, town_count[town], start_town_count[town];}' > temp/temp_tflag_1.csv

    cd progc
    make -s compile 
    ./tflag ../temp/temp_tflag_1.csv ../temp/temp_tflag_2.csv
    
}

    
#the function bellow will display the top10 longest trips by "Route ID" as well as their distance
#First the system use awk to sum the total distance of each parcours "Route ID"
#sort in order to have the values in the descending order and get the first 10 greatest values 
#finally the histogram bars diagram is created using gnuplot

lFlag() 
{
    cut -d';' -f1,5 data/data.csv |
    awk -v OFS=';' -F';' 'NR>1 { route_lengths_sum[$1]+= $2;} END {for (route_id in route_lengths_sum) {print route_id, route_lengths_sum[route_id];}}' |
    sort -t";" -k2nr |
    head -n10 > temp/temp_lflag.csv

# Create histogram data file
    awk -v OFS='; ' -F';' '{print $2, $1}' temp/temp_lflag.csv > temp/histogram_l_data.csv

    # Set up the histogram configurations and plot the histogram
    gnuplot <<-EOF
    set terminal png size 1500,600
    set output 'images/histogram_l.png'
    set title " Option -l: Distance = f(Route) "
    set ylabel "Total Distance(km)
    set xlabel "Route ID"
    set style data histograms
    set style fill solid border -1
    set yrange [0:3000]
    set boxwidth 0.8
    set ytic 600
    plot 'temp/histogram_l_data.csv' using 1:xtic(2) notitle
    
    
    
EOF
  
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
	head -n 50 sflag_Sorted_data.csv > diagram_s_data.csv
}

help()
{ 
echo "  -h                       displays the help menu"
echo "  -d1                      displays a histogram of the top 10 drivers with the biggest number of trips"
echo "  -d2                      displays a histogram of the top 10 drivers with the longest"
echo "                                 distance in their trips"
echo "  -l                       displays a histogram of the top 10 longest trips"
echo "  -t                       displays a histogram of the top 10 most visited cities"
echo "  -s                       displays an error bar diagram with the statistics about the distance's"
echo "                                 varaition of each trip, it will be displayed the top 50 routes"
echo "                                 with the greatest distance variation of each route"

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
else
    find images -mindepth 1 -delete
fi

#management of arguments
all_args=$*
input_dir=$1

checkDir $input_dir #verify the existence of the data directory



for i in $all_args #print the help list and launch the other flags
do
	case $i in

	"-h") 
	
        help
        
    	exit 0;;
    esac
done



for i in $all_args #print the help list and launch the other flags
do
	case $i in
      
	"-d1")
	
    	strt_time=$(getTime)
	echo "-d1 flag in progress..."    
    	
    	d1Flag
	
	echo "-d1 completed"
    	
    	echo "Duration of the task's execution: $(taskDuration $strt_time) seconds"
	
	echo "\n"
	
     	;;
     	
	"-d2") 
    
    	strt_time=$(getTime)
	
	echo "-d2 flag in progress..."        
	
	d2Flag
    
    	echo "Duration of the task's execution: $(taskDuration $strt_time) seconds"
	
	echo "-d2 flag completed"        
	
	echo "\n"
        ;;


	"-t") 
	echo "-t flag in progress..."        
	strt_time=$(getTime)

	tFlag

	echo "-t flag completed"        

	echo "Duration of the task's execution: $(taskDuration $strt_time) seconds"
	
	echo "\n"
	;;	


    	"-l")

	strt_time=$(getTime)
	
	echo "-l flag in progress..."        

	lFlag

	echo "-l flag completed"        		

	echo "Duration of the task's execution: $(taskDuration $strt_time) seconds"

	echo "\n"
	;;
	
	"-s")
    	strt_time=$(getTime)
    	echo "-s flag in progress..."        
        
	sFlag
	
    	echo "-s flag completed"
	
    	echo "Duration of the task's execution: $(taskDuration $strt_time) seconds"

    	;;		

	*)
	echo "flag not found, please try again"
	exit 1
    esac
done
