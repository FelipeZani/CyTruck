#include "sflag_tools.h"

int main ()
{

	FILE * datacsv = fopen("../data/data.csv", "r"); 
	FILE * save_data = fopen("../temp/sflag_data.csv", "w");  
	/*
	 * the code above indicates where the files: the path of the original data file
	 * the path of the treated data should be placed
	 * */
	 
	if(datacsv == NULL||save_data == NULL) //error check
	{
		fprintf(stderr,"Files failed to open");
		exit(1);
	}
	
	int header = 1; //indicates if the header file of the original documents exist
	int loop = 1; //controls the functionment of thr loop
	
	
	extractrow(datacsv,&loop,&header); //extracts header line
	
	Driverstts driver_row = extractrow(datacsv,&loop,&header); 
	pAVLNode a = createNode(driver_row);
	
    /*
     * the lines above willl initializate the structure containing the data 
     * of a lambda driver along with the AVL's initialization  
     * */
	
	while(loop)
	{
		driver_row = extractrow(datacsv,&loop,&header); //extracts the current row and stock in the driver structure
		if(driver_row.RouteID != 0) 
			a = insertNode(a,driver_row,&(a->balance));
		
	}
	saveFile(a,save_data);//function that records the new data in a file
	
	
	//memory liberation section
	fclose(save_data);
	fclose(datacsv); 
	freeMemoryAVL(a);
	
	return 0;
}
