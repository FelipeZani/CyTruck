#include "sflag_tools.h"



pAVLNode createNode(Driverstts driver)
{

	pAVLNode a = malloc(sizeof(AVLNode));
	
	if(a == NULL)
	{
		printf("Memory allocation failed");
		exit(1);
	}
	
	a->routeInfo.max = driver.distance;
	a->routeInfo.min = driver.distance;
	a->routeInfo.highest_step = driver.step;
	a->routeInfo.sum = driver.distance;
	a->RouteID = driver.RouteID;

	a->balance = 0;
	
	a->right = NULL;
	a->left = NULL;
	
	return a;
}

pAVLNode leftRot(pAVLNode a)
{
	int blc_a, blc_p; //balances
	
	pAVLNode pvt = a->right;
	
	a->right = pvt->left;
	pvt->left = a;
	
	blc_a = a->balance;
	blc_p = pvt->balance;
	
	a->balance = blc_a - max_value(blc_p,0) - 1;
	pvt->balance = min_value(min_value(blc_a-2,blc_a+blc_p-2), blc_p-1);
	
	a = pvt;
	
	return a;
}

pAVLNode rightRot(pAVLNode a)
{
	pAVLNode pvt = a->left;
	int blc_a,blc_p;
	
	
	a->left = pvt->right;
	pvt->right = a;
	
	blc_a = a->balance;
	blc_p = pvt->balance;
	
	a->balance = blc_a - min_value(blc_p,0)+1;
	pvt->balance = max_value(max_value(blc_a+2, blc_a+blc_p+2), blc_p+1);
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

pAVLNode insertNode(pAVLNode a, Driverstts drv, int * balance) //insert a node in the avl if it doesn't exist
//otherwise the function will update the existent node with new data: min,max,etc.
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
	else if(a->RouteID > drv.RouteID)
	{
		a->left = insertNode(a->left, drv, balance);
		*balance *= -1;
	}
	else if(a->RouteID < drv.RouteID)
	{
		a->right = insertNode(a->right, drv, balance);
	}
	else //updating the existent node
	{
		*balance = 0;
		a->routeInfo.max = max_value(a->routeInfo.max,drv.distance);
		a->routeInfo.min = min_value(a->routeInfo.min,drv.distance);
		a->routeInfo.highest_step = max_value(a->routeInfo.highest_step,drv.step); 
		a->routeInfo.sum += drv.distance;
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

Driverstts extractrow(FILE *file, int *loop, int *header) //this function will read line bt line of the .csv document
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



void saveFile(pAVLNode a, FILE * f) //record the data after the computation in a file
{
	if(a != NULL)
	{	
		saveFile(a->left, f); 
		fprintf(f,"%d;%f;%f;%f;%f\n", a->RouteID, a->routeInfo.min, a->routeInfo.max,
		(a->routeInfo.sum/a->routeInfo.highest_step),(a->routeInfo.max-a->routeInfo.min));
		saveFile(a->right, f);
		
	} //inorder transversal all the nodes and recording one by one
}

void freeMemoryAVL(pAVLNode a) {
    if (a != NULL) 
    {
        freeMemoryAVL(a->left);
        freeMemoryAVL(a->right);
        free(a);
    }
}

float max_value(float a, float b)
{
	if(a > b)
		return a;
	return b;
}

float min_value(float a, float b)
{
	if(a < b)
		return a;
	return b;
}

