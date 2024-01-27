#include "sflag_tools.h"


//this function will receive a structure and return a node to an avl containing the information of this structure and return it
pAVLNode createNode(Driverstts driver) 
{
	pAVLNode a = malloc(sizeof(AVLNode));
	
	checkPointerAllocation(a);

	a->data.routeInfo.max = driver.distance;
	a->data.routeInfo.min = driver.distance;
	a->data.routeInfo.highest_step = driver.step;
	a->data.routeInfo.sum = driver.distance;
	a->data.routeInfo.RouteID = driver.RouteID;
	
	a->balance = 0;
	a->right = NULL;
	a->left = NULL;
	
	return a;
}
//this function will receive a structure and return a node to an avl containing the information of this structure and return it
pAVLNode createNode2SortdS(DatadS route)
{
	pAVLNode a = malloc(sizeof(AVLNode));
	
	checkPointerAllocation(a);
	
	a->data.routedS.RouteID = route.RouteID;
	a->data.routedS.average = route.average;
	a->data.routedS.dS = route.dS;
	a->data.routedS.max = route.max;
	a->data.routedS.min = route.min;
	
	a->balance = 0;
	a->right = NULL;
	a->left = NULL;
	
	return a;
	 
}
//this function does a left rotation
pAVLNode leftRot(pAVLNode a)
{
	int blc_a, blc_p; //balances
	
	pAVLNode pvt = NULL;
	
	pvt = a->right;
	a->right = pvt->left;
	pvt->left = a;
	
	blc_a = a->balance;
	blc_p = pvt->balance;
	
	a->balance = blc_a - max_value(blc_p,0) - 1;
	pvt->balance = min_value(min_value(blc_a-2,blc_a+blc_p-2), blc_p-1);
	
	a = pvt;
	
	return a;
}
//the following functions are function that will keep the balance of the AVLs
pAVLNode rightRot(pAVLNode a)
{
	pAVLNode pvt = NULL;

	int blc_a,blc_p;
	
	pvt = a->left;
	a->left = pvt->right;
	pvt->right = a;
	
	blc_a = a->balance;
	blc_p = pvt->balance;
	
	a->balance = blc_a - min_value(blc_p,0)+1;
	pvt->balance = max_value(max_value(blc_a+2,blc_a+blc_p+2), blc_p+1);
	
	a = pvt;
	
	return a;
}

pAVLNode doubleLeftRot(pAVLNode a)
{
	
	a->right = rightRot(a->right);
	
	return leftRot(a);
}

pAVLNode doubleRightRot( pAVLNode a)
{
	a->left = leftRot(a->left);
	
	return rightRot(a);
}

pAVLNode rebalanceAVL(pAVLNode a)
{
	if(a->balance >= 2)
	{
		if(a->right->balance >= 0)
			return leftRot(a);
		else
			return doubleLeftRot(a);
	}
	else if( a->balance <= -2)
	{
		if(a->left->balance <= 0)
			return rightRot(a);
		else
			return doubleRightRot(a);
	}
	return a;
}
pAVLNode updateFieldsAVL(pAVLNode a, Driverstts drv, int * balance) //insert a node in the avl if it doesn't exist
//otherwise the function will update the existent node with new data: min_distance,max_distance,etc.
{
	
	if(drv.RouteID <= 0 || drv.step <= 0 || drv.distance <= 0)
	{
		printf("Corrupted file, InsertNode");
		printf("%d,%d,%f",drv.RouteID, drv.step, drv.distance);
		exit(1);
	} else if(balance == NULL)
	{
		fprintf(stderr,"Error: balance pointer is NULL");
		exit(1);
	}
	//code above : handling errors
	
	if(a == NULL) //insertion of a node in a AVL
	{
		*balance = 1;
		return createNode(drv);
	}
	else if(a->data.routeInfo.RouteID > drv.RouteID)
	{
		a->left = updateFieldsAVL(a->left, drv, balance);
		*balance *= -1;
	}
	else if(a->data.routeInfo.RouteID  < drv.RouteID)
	{
		a->right = updateFieldsAVL(a->right, drv, balance);
	}
	else //updating the existent node with the new values
	{
		*balance = 0;
		
		a->data.routeInfo.max = max_value(a->data.routeInfo.max,drv.distance);
		a->data.routeInfo.min = min_value(a->data.routeInfo.min,drv.distance);
		a->data.routeInfo.highest_step = max_value(a->data.routeInfo.highest_step,drv.step); 
		a->data.routeInfo.sum += drv.distance;
		
		return a;
		/*
		 * the sum of distances and highest step of a route  are computed 
		 *in order to provide an average distance in the route*/
	}
	if(*balance != 0)
	{
		a->balance += *balance;
		
		a = rebalanceAVL(a);
		
		if(a->balance == 0)
		{
			*balance = 0;
		}
		else
		{	
			*balance = 1;
			return a;
		}
	}
	return a;
}
//nodeDS
pAVLNode sortNodesRoute(pAVLNode a, DatadS route, int * balance) //insert a node in the avl if it doesn't exist
//otherwise it will just return the whole tree. The goal of this function is to sort the dS each time the tree will be rebalanced
// therefore the system can keep tracking the greatest values of the trip
{

	
	if(route.RouteID <= 0||route.max <= 0||route.min <= 0||route.dS <= 0||route.average <= 0 )
	{
		printf("%d;%f;%f;%f;%f",route.RouteID,route.max,route.min,route.dS,route.average);
		printf("Corrupted file, sortNodesRoute");
		exit(1);
	} else if(balance == NULL)
	{
		fprintf(stderr,"Error: balance pointer is NULL");
		exit(1);
	}
	//code above : handling errors
	
	if(a == NULL) //insertion of a node in a the AVL
	{
		*balance = 1;
		return createNode2SortdS(route);
	}
	else if(a->data.routedS.dS > route.dS)
	{
		a->left = sortNodesRoute(a->left, route, balance);
		*balance *= -1;
	}
	else if(a->data.routedS.dS  < route.dS)
	{
		a->right = sortNodesRoute(a->right, route, balance);
	}
	else
	{
		*balance = 0;
		return a;

	} //if a route with a repeated distance already exists , the program will just ignore the node
	if(*balance != 0)
	{
		a->balance += *balance;
		
		a = rebalanceAVL(a);
		
		if(a->balance == 0)
		{
			*balance = 0;
		}
		else
		{	
			*balance = 1;
			return a;
		}
	}
	return a;
}


Driverstts readDataFile(FILE *file, int *loop, int *header) //this function will read line bt line of the .csv document
{
    if (file == NULL) //error handling
    {
        perror("Error opening file");
        exit(1);
    }
    
	int result = 0; // result of sscanf
    char buffer[MAX_LINE_SIZE]; 

    Driverstts driver_node;
    driver_node.RouteID = 0;
    driver_node.step = 0;
    driver_node.distance = 0;

    if (*header == 1) //check if the header line exists
    {
        if (fgets(buffer, MAX_LINE_SIZE, file) == NULL)
        {
            fprintf(stderr, "Error reading header line\n");
            exit(1);
        }
		memset(buffer,0,sizeof(buffer)); //cleans the buffer
        *header = 0;
    }

    if (fgets(buffer, MAX_LINE_SIZE, file) == NULL)
    {
        if (feof(file))// verify if reached end of file
        {
            *loop = 0; 
        }
        else //verify if fgets returned NULL
        {
            perror("Error reading line from file");
        }

        return driver_node;
    }
	/*the code above will read the current line in the data.csv and stock
	 * it in a buffer string , fgets also skip to the next line
	*/
	
    result = sscanf(buffer, "%d;%d;%*[^;];%*[^;];%f", &driver_node.RouteID, &driver_node.step, &driver_node.distance);
	//transfer the required data to the driver_node struct
	//csv file is delimited by ; then %*[^;] : ignore the following string until reached the next ;
	
	memset(buffer,0,sizeof(buffer)); //cleans the buffer
	
    if (result != 3)
    {
        fprintf(stderr, "Error parsing line. Expected 3 values, but parsed %d\n", result);
        // Handle the return of a sscanf and verifies in case of error.
    }
    if (driver_node.RouteID <= 0 || driver_node.step <= 0 || driver_node.distance <= 0) // Handle any unexpected value
    {
        fprintf(stderr, "Corrupted file. RouteID, step, or distance is not valid.\n");
        exit(1);
    }

    return driver_node;
}



void saveFile(pAVLNode a, FILE * f,int mode) //record the data after the computation in a file
{
	if(a != NULL)
	{	if(mode == 1)
		{
			saveFile(a->left, f, mode);
			 
			fprintf(f,"%d;%f;%f;%f;%f\n", a->data.routeInfo.RouteID, a->data.routeInfo.min, a->data.routeInfo.max,
			(a->data.routeInfo.sum/a->data.routeInfo.highest_step),(a->data.routeInfo.max-a->data.routeInfo.min));
			
			saveFile(a->right, f, mode);
		}
		else 
		{
			saveFile(a->right, f, mode);
			
			fprintf(f,"%d;%f;%f;%f\n",a->data.routedS.RouteID,a->data.routedS.min,
										 a->data.routedS.max,a->data.routedS.average);
			
			saveFile(a->left, f, mode); 

		}
	} //inorder transversal all the nodes and recording one by one
}


DatadS readData2Sort(FILE * file, int * loop) //this function will read the data from a file, and get it ready in order to sort
{
	DatadS route;
	
	route.min = 0;
	route.max = 0;
	route.average = 0;
	route.RouteID = 0;
	route.dS = 0;


	if (file == NULL) //error handling
    {
        perror("Error opening file");
        exit(1);
    }
    
    int result = 0; // result of sscanf
    char buffer[MAX_LINE_SIZE]; 
    
    if (fgets(buffer, MAX_LINE_SIZE, file) == NULL)
    {
        if (feof(file))// verify if reached end of file
        {
            *loop = 0; 
        }
        else //verify if fgets returned NULL
        {
            perror("Error reading line from file, in readData2Sort");
            exit(1);
        }

        return route;
    }
	/*the code above will read the current line in the data.csv and stock
	 * it in a buffer string , fgets also skip to the next line
	*/

    result = sscanf(buffer, "%d;%f;%f;%f;%f", &route.RouteID, &route.min, &route.max, &route.average,&route.dS);
	//transfer the required data to the driver_node struct
	//csv file is delimited by ; then %*[^;] : ignore the following string until reached the next ;
	
	memset(buffer,0,sizeof(buffer)); //cleans the buffer
	
    if (result != 5)
    {
        fprintf(stderr, "Error parsing line. Expected 5 values, but parsed %d\n", result);
        // Handle the return of a sscanf and verifies in case of error.
        exit(1);
    }

	if(route.min <= 0 || route.max <= 0 || route.RouteID <= 0 || route.average <= 0 || route.dS < 0)
	{
		printf("%d;%f;%f;%f;%f",route.RouteID, route.min, route.max, route.average,route.dS );
        fprintf(stderr, "Corrupted file. RouteID, step, or distance is not valid.\n");
        exit(1);	
	}
	
    return route;
}


void freeMemoryAVL(pAVLNode a) //free the memory allocated to handle a AVL memory
{
    if (a != NULL) 
    {
		freeMemoryAVL(a->left);
		freeMemoryAVL(a->right);
		free(a);
    }
    
}

//the following code will return the max value of two intengers
float max_value(float a, float b) 
{
	if(a > b)
		return a;
	return b;
}

float min_value(float a, float b) // same but for min values
{
	if(a < b)
		return a;
	return b;
}
void checkPointerAllocation(void * p) //very if memory is allocated

{
	if(p == NULL)
	{
		printf("Memory allocation failed");
		exit(1);
	}
}

