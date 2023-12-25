#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_MOST_VISITED_TOWNS 10
#define DATA_FILE_PATH argv[1]
#define OUTPUT_PATH "../temp/temp_tflag_2.csv"

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

typedef struct Avl
{
    pTown town;
    struct Avl* ls;
    struct Avl* rs;
    int eq;
} Avl;

typedef struct Avl* pAvl;

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

pAvl avlcreation(pTown town)
{
    pAvl node = malloc(sizeof(Avl));
    node->town = town;
    node->ls = NULL;
    node->rs = NULL;
    node->eq = 0;

    return node;
}

pAvl leftRotation(pAvl a)
{
    pAvl pivot = a->rs;
    a->rs = pivot->ls;
    pivot->ls = a;

    int eq_a = a->eq;
    int eq_p = pivot->eq;

    a->eq = eq_a - MAX2(eq_p, 0) - 1;
    pivot->eq = MIN3(eq_a-2, eq_a+eq_p-2, eq_p-1);
    a = pivot;

    return a;
}

pAvl rightRotation(pAvl a)
{
    pAvl pivot = a->ls;
    a->ls = pivot->rs;
    pivot->rs = a;

    int eq_a = a->eq;
    int eq_p = pivot->eq;

    a->eq = eq_a - MIN2(eq_p, 0) + 1;
    pivot->eq = MAX3(eq_a+2, eq_a+eq_p+2, eq_p+1);
    a = pivot;

    return a;
}

pAvl doubleleftRotation(pAvl a)
{
    a->rs = rightRotation(a->rs);
    return leftRotation(a);
}

pAvl doubleRotationDroite(pAvl a)
{
    a->ls = leftRotation(a->ls);
    return rightRotation(a);
}

pAvl balanceAvl(pAvl a)
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

pAvl avlInsertionByTotal(pAvl a, pTown town, int* ph)
{
    if(a == NULL)
    {
        *ph = 1;
        return avlcreation(town);
    }
    else if(town->total < a->town->total)
    {
        a->ls = avlInsertionByTotal(a->ls, town, ph);
        *ph = -(*ph);
    }
    else if(town->total > a->town->total)
    {
        a->rs = avlInsertionByTotal(a->rs, town, ph);
    }
    else
    {
        *ph = 0;
        return a;
    }

    if (*ph != 0 )
    {
        a->eq += *ph;
        a = balanceAvl(a);
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

pAvl avlInsertionByName(pAvl a, pTown town, int* ph)
{
    if(a == NULL)
    {
        *ph = 1;
        return avlcreation(town);
    }
    else if(strcmp(town->name, a->town->name) > 0)
    {
        a->ls = avlInsertionByName(a->ls, town, ph);
        *ph = -(*ph);
    }
    else if(strcmp(town->name, a->town->name) < 0)
    {
        a->rs = avlInsertionByName(a->rs, town, ph);
    }
    else
    {
        *ph = 0;
        return a;
    }

    if (*ph != 0 )
    {
        a->eq += *ph;
        a = balanceAvl(a);
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

void RNL_printf(pAvl avl, int* pCount)
{
    if(avl != NULL && *pCount > 0)
    {
        RNL_printf(avl->rs, pCount);
        if(*pCount > 0) {printf("%s %d %d\n", avl->town->name, avl->town->total, avl->town->first_step); (*pCount)--;}
        RNL_printf(avl->ls, pCount);
    }
}

void RNL_fprintf(pAvl avl, FILE* outputFile)
{
    if(avl != NULL)
    {
        RNL_fprintf(avl->rs, outputFile);
        fprintf(outputFile, "%s;%d;%d\n", avl->town->name, avl->town->total, avl->town->first_step);
        RNL_fprintf(avl->ls, outputFile);
    }
}

void RNL_avlInsertionByName(pAvl source_avl, pAvl* destination_avl, int *ph, int* pCount)
{
    if(source_avl != NULL && *pCount > 0)
    {
        RNL_avlInsertionByName(source_avl->rs, destination_avl, ph, pCount);
        if(*pCount > 0) {*destination_avl = avlInsertionByName(*destination_avl, source_avl->town, ph);  (*pCount)--;}
        RNL_avlInsertionByName(source_avl->ls, destination_avl, ph, pCount);
    }
}

int main(int argc, char *argv[])
{

    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <data_file.csv>\n", argv[0]);
        return 1;
    }    

    
    pAvl avlByTotal = NULL;
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
        avlByTotal = avlInsertionByTotal(avlByTotal, townCreation(nameBuffer, totalBuffer, firstStepBuffer), &h1);
    }
    fclose(dataFile);





    int count = N_MOST_VISITED_TOWNS, h2 = 0;
    pAvl avlByName = NULL;
    
    RNL_avlInsertionByName(avlByTotal, &avlByName, &h2, &count);
    
    count = N_MOST_VISITED_TOWNS;
    RNL_printf(avlByName, &count);
    

    FILE* outputFile = fopen(OUTPUT_PATH, "w");

    RNL_fprintf(avlByName, outputFile);

    fclose(outputFile);


    return 0;
}

