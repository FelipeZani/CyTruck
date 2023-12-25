#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_MOST_VISITED_TOWNS 10
#define DATA_FILE_PATH argv[1]
#define OUTPUT_FILE_PATH argv[2]

#define MIN2(i, j) (((i) < (j)) ? (i) : (j))
#define MAX2(i, j) (((i) > (j)) ? (i) : (j))

#define MIN3(i, j, k) (((i) < (j)) ? (((i) < (k)) ? (i) : (k)) : (((j) < (k)) ? (j) : (k)))
#define MAX3(i, j, k) (((i) > (j)) ? (((i) > (k)) ? (i) : (k)) : (((j) > (k)) ? (j) : (k)))


typedef struct Town {
    char name[30];
    int total;
    int first_step;
} Town;

typedef struct Town* pTown;

typedef struct AVL
{
    pTown town;
    struct AVL* ls;
    struct AVL* rs;
    int eq;
} AVL;

typedef struct AVL* pAVL;

pTown townCreation(char name[], int total, int first_step)
{
	pTown newTown = malloc(sizeof(Town));
	if(newTown == NULL)
	{
		fprintf(stderr, "ERROR : Memory allocation\n");
		exit(EXIT_FAILURE);
	}

	strcpy(newTown->name, name);
	newTown->total = total;
	newTown->first_step = first_step;

	return newTown;
}

pAVL AVLcreation(pTown town)
{
    pAVL node = malloc(sizeof(AVL));
    node->town = town;
    node->ls = NULL;
    node->rs = NULL;
    node->eq = 0;

    return node;
}

pAVL leftRotation(pAVL a)
{
    pAVL pivot = a->rs;
    a->rs = pivot->ls;
    pivot->ls = a;

    int eq_a = a->eq;
    int eq_p = pivot->eq;

    a->eq = eq_a - MAX2(eq_p, 0) - 1;
    pivot->eq = MIN3(eq_a-2, eq_a+eq_p-2, eq_p-1);
    a = pivot;

    return a;
}

pAVL rightRotation(pAVL a)
{
    pAVL pivot = a->ls;
    a->ls = pivot->rs;
    pivot->rs = a;

    int eq_a = a->eq;
    int eq_p = pivot->eq;

    a->eq = eq_a - MIN2(eq_p, 0) + 1;
    pivot->eq = MAX3(eq_a+2, eq_a+eq_p+2, eq_p+1);
    a = pivot;

    return a;
}

pAVL doubleleftRotation(pAVL a)
{
    a->rs = rightRotation(a->rs);
    return leftRotation(a);
}

pAVL doubleRotationDroite(pAVL a)
{
    a->ls = leftRotation(a->ls);
    return rightRotation(a);
}

pAVL balanceAVL(pAVL a)
{
    if(a->eq >=2 )
    {
        if(a->rs->eq >= 0)
        {
            return leftRotation(a);
        }
        else
        {
            return doubleleftRotation(a);
        }
    }
    else if(a->eq <= -2)
    {
        if(a->ls->eq <=0)
        {
            return rightRotation(a);
        }
        else
        {
            return doubleRotationDroite(a);
        }
    }
    return a;
}

pAVL AVLInsertionByTotal(pAVL a, pTown town, int* ph)
{
    if(a == NULL)
    {
        *ph = 1;
        return AVLcreation(town);
    }
    else if(town->total < a->town->total)
    {
        a->ls = AVLInsertionByTotal(a->ls, town, ph);
        *ph = -(*ph);
    }
    else if(town->total > a->town->total)
    {
        a->rs = AVLInsertionByTotal(a->rs, town, ph);
    }
    else
    {
        *ph = 0;
        return a;
    }

    if (*ph != 0 )
    {
        a->eq += *ph;
        a = balanceAVL(a);
        if(a->eq == 0)
        {
            *ph = 0;
        }
        else
        {
            *ph = 1;
        }
    }
    return a;
}

pAVL AVLInsertionByName(pAVL a, pTown town, int* ph)
{
    if(a == NULL)
    {
        *ph = 1;
        return AVLcreation(town);
    }
    else if(strcmp(town->name, a->town->name) > 0)
    {
        a->ls = AVLInsertionByName(a->ls, town, ph);
        *ph = -(*ph);
    }
    else if(strcmp(town->name, a->town->name) < 0)
    {
        a->rs = AVLInsertionByName(a->rs, town, ph);
    }
    else
    {
        *ph = 0;
        return a;
    }

    if (*ph != 0 )
    {
        a->eq += *ph;
        a = balanceAVL(a);
        if(a->eq == 0)
        {
            *ph = 0;
        }
        else
        {
            *ph = 1;
        }
    }
    return a;
}

void RNL_printf(pAVL AVL, int* pCount)
{
    if(AVL != NULL && *pCount > 0)
    {
        RNL_printf(AVL->rs, pCount);
        if(*pCount > 0) {printf("%s %d %d\n", AVL->town->name, AVL->town->total, AVL->town->first_step); (*pCount)--;}
        RNL_printf(AVL->ls, pCount);
    }
}

void RNL_fprintf(pAVL AVL, FILE* outputFile)
{
    if(AVL != NULL)
    {
        RNL_fprintf(AVL->rs, outputFile);
        fprintf(outputFile, "%s;%d;%d\n", AVL->town->name, AVL->town->total, AVL->town->first_step);
        RNL_fprintf(AVL->ls, outputFile);
    }
}

void RNL_AVLInsertionByName(pAVL source_AVL, pAVL* destination_AVL, int *ph, int* pCount)
{
    if(source_AVL != NULL && *pCount > 0)
    {
        RNL_AVLInsertionByName(source_AVL->rs, destination_AVL, ph, pCount);
        if(*pCount > 0) 
        {
                *destination_AVL = AVLInsertionByName(*destination_AVL, townCreation(source_AVL->town->name, source_AVL->town->total, source_AVL->town->first_step), ph);
                (*pCount)--;
        }
        RNL_AVLInsertionByName(source_AVL->ls, destination_AVL, ph, pCount);
    }
}

void freeAVL(pAVL AVL)
{
    if(AVL->ls != NULL) {freeAVL(AVL->ls);}
    if(AVL->rs != NULL) {freeAVL(AVL->rs);}
    free(AVL);
}

int main(int argc, char *argv[])
{

    if (argc != 3) 
    {
        fprintf(stderr, "Usage: %s <data_file.csv> <output_file.csv>\n", argv[0]);
        return 1;
    }    

    
    pAVL AVLByTotal = NULL;
    int h1 = 0;


    FILE* dataFile = fopen(DATA_FILE_PATH, "r");

    if(dataFile == NULL)
	{
		fprintf(stderr, "Error: Can't open data file\n");
		return 1;
	}

    char nameBuffer[30];
	int totalBuffer, firstStepBuffer;


    while(fscanf(dataFile, "%[^;];%d;%d\n", nameBuffer, &totalBuffer, &firstStepBuffer) == 3)
    {
        AVLByTotal = AVLInsertionByTotal(AVLByTotal, townCreation(nameBuffer, totalBuffer, firstStepBuffer), &h1);
    }
    fclose(dataFile);





    int count = N_MOST_VISITED_TOWNS, h2 = 0;
    pAVL AVLByName = NULL;
    
    RNL_AVLInsertionByName(AVLByTotal, &AVLByName, &h2, &count);
    
    count = N_MOST_VISITED_TOWNS;
    //RNL_printf(AVLByName, &count);
    

    FILE* outputFile = fopen(OUTPUT_FILE_PATH, "w");

    RNL_fprintf(AVLByName, outputFile);

    fclose(outputFile);

    freeAVL(AVLByTotal);
    freeAVL(AVLByName);

    return 0;
}

