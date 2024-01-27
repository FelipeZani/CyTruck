#include "sflag_tools.h"

int main ()
{

	int header = 1; //indicates if the header file of the original documents exist
	int loop = 1; //controls the functionment of thr loop
	int mode = 1; //the mode used by some functions may be different accprding the state of this variable
	int balance = 0; //valance variable used by the avl functions in order to keep the balance of the nodes
	
	
	//the following code is only the declaration of file pointers that will be used later 
	FILE * datacsv = NULL; 
	FILE * save_data = NULL;
	FILE * read_ToSort = NULL;
	FILE * export_savedData = NULL;
	
	
	//Code bellow: variables in charge of stocking the data from the data file  
	
	Driverstts driver_row; // variable used to stock a piece of data from the original file in this structure
	
	DatadS routedS; // variable used to stock the data from the treated data
	
	pAVLNode computingTree = NULL; 
	pAVLNode sortingTree = NULL;
	
	//initiating structure in order to keep va
	driver_row.RouteID = 0;
	driver_row.step = 0;
	driver_row.distance = 0;

	
	datacsv = fopen("../data/data.csv", "r"); 
	save_data = fopen("../temp/sflag_data.csv", "w");
	
	
	
	if(datacsv == NULL||save_data == NULL) //error check
	{
		fprintf(stderr,"System failled to open files");
		exit(1);
	}
	
	
	driver_row = readDataFile(datacsv,&loop,&header); 
	computingTree = createNode(driver_row);
	
	/*the code sortingTreeelow will read the original datafile
	 *and stock the data inside a structure which will sortingTreee insert
	 * in a AVL tree that will compute the max/distance distance and etc.
	*/
	while(loop) 
	{
		driver_row = readDataFile(datacsv,&loop,&header); //extracts the current row and stock in the driver structure
		if(driver_row.RouteID != 0) 
			computingTree = updateFieldsAVL(computingTree,driver_row,&balance);
		
	}
	
	saveFile(computingTree,save_data,mode);//function that records the new data in a file

	fclose(datacsv); //freeing memory
	fclose(save_data);
	
	//update the mode of functions
	mode = 2;
	loop = 1;
	
/*  the code bellow will function almost in the same way as the one for the computingTree:
	the program will read the data from the computed data file and insert in a AVL,
	but this time the AVL will be used in order to sort the values by the distance variation: dS
	due to the rebalance of the AVL Tree
	*/
	 
	read_ToSort = fopen("../temp/sflag_data.csv", "r");
	export_savedData =  fopen("../temp/sflag_Sorted_data.csv","w");
	
	//initiating variables
	routedS = readData2Sort(read_ToSort,&loop);
	sortingTree = createNode2SortdS(routedS);
	
	while(loop) // read data from extracted datafile and import it in a AVL tree
	{
		routedS = readData2Sort(read_ToSort,&loop);
		
		if(routedS.dS > 0) //avoiding having distance variation equals 0, because it may trick the results
			sortingTree = sortNodesRoute(sortingTree,routedS,&balance);

	}

	
	
 	saveFile(sortingTree,export_savedData,mode); //saving the data from the sortingTree
	
	//memory liberation section
	freeMemoryAVL(computingTree);
	freeMemoryAVL(sortingTree);
	fclose(read_ToSort); 
	
	return 0;
}
