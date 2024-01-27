#ifndef SFLAG_TOOLS
#define SFLAG_TOOLS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1024

typedef struct driverstts //RouteStatus
{
	int RouteID;
	int step;
	float distance;
}Driverstts;

typedef struct dataExtracted//data extracted from the data file that will be used to compute the information we need
{
    float min;
    float max;
    float sum;
	int highest_step;
	int RouteID;

}DataExtracted;

typedef struct datadS // final structure of data which will be sorted by the AVL
{ 
	int RouteID;
	float max; //max_distance
	float min; //min_distance
	float dS; //dS = max_distance - min_distance distance variation
	float average;//average_distance
	
	
}DatadS;

typedef struct avlNode
{
	int balance;
	
	union Data
	{
			 DataExtracted routeInfo;
			 DatadS routedS;
	}data;
   
    struct avlNode * left;  
    struct avlNode * right; 
    
}AVLNode;

typedef AVLNode * pAVLNode;

//AVL functions

pAVLNode createNode(Driverstts driver);
pAVLNode updateFieldsAVL(pAVLNode a, Driverstts drv, int * balance);
pAVLNode leftRot(pAVLNode a);
pAVLNode rightRot(pAVLNode a);
pAVLNode doubleLeftRot(pAVLNode a);
pAVLNode doubleRightRot( pAVLNode a);
pAVLNode createNode2SortdS(DatadS route);
pAVLNode sortNodesRoute(pAVLNode a, DatadS route, int * balance);

void parcour(pAVLNode a);



// file related functions
Driverstts readDataFile(FILE * file, int * loop, int * header);
DatadS readData2Sort(FILE * file, int * loop);

void saveFile(pAVLNode a, FILE * f,int mode); //record the data after the computation in a file
void freeMemoryAVL(pAVLNode a);

// other tools 
float min_value(float a, float b);
float max_value(float a, float b);

void checkPointerAllocation( void * p);


#endif
