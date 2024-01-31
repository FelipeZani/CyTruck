# CyTruck
## A functional app to manage the data of your truck company
This app provides you a set of tools to have an accurate data visualisation

### Prerequisites:
* Compiler: GCC 11.4+
*  Gnuplot 5.4+
* Make 4.3+
### Instructions to launch the program
* Clone the repository : git clone
* Create a folder named "data" where you will insert your data file, in a .csv extension, inside your file
* Open your terminal and insert sh main.sh data [option]
### Options
* -h: open the help menu
* -d1: creates a horizontal histogram listing the top 10 drivers with greatest number of trips
* -d2: creates a horizontal histogram listing the top 10 drivers with the greatest distance traveled
* -l: create a histogram listing the top 10 longest trips
* -t: create a histogram listing the top 10 most visited cities
* -s: create an error bar diagram listing the top 50 routes with greatest variation of distance dS
