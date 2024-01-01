#ifndef SFLAG_TOOLS
#define SFLAG_TOOLS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1024

typedef struct driverstts
{
	int RouteID;
	int step;
	float distance;
}Driverstts;

typedef struct data
{
    float min;
    float max;
    float sum;
	int highest_step;

}Data;

typedef struct avlNode 
{
	int RouteID;
	int balance;
	
    Data routeInfo;
    struct avlNode * left;  
    struct avlNode * right; 
    
}AVLNode;

typedef AVLNode * pAVLNode;

//AVL functions

pAVLNode createNode(Driverstts driver);
pAVLNode insertNode(pAVLNode a, Driverstts drv, int * balance);
pAVLNode leftRot(pAVLNode a);
pAVLNode rightRot(pAVLNode a);
pAVLNode doubleLeftRot(pAVLNode a);
pAVLNode doubleRightRot( pAVLNode a);

void inorderTvlAVL(pAVLNode a);


// file related functions
Driverstts extractrow(FILE * file, int * loop, int * header);

void saveFile(pAVLNode a, FILE * f);
void freeMemoryAVL(pAVLNode a);

// other tools 
float min_value(float a, float b);
float max_value(float a, float b);



#endif
