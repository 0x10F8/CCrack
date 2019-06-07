#include <stdlib.h>
#include <string.h>
#include "permutations.h"

Permutations *makePermutations(unsigned char *charset, unsigned short maxLength, unsigned short currentLength)
{
    Permutations *p = malloc(sizeof(Permutations) + currentLength * sizeof(unsigned short));
    p->charset = charset;
    p->currentString = malloc(sizeof(unsigned char) * currentLength);
    p->charsetLength = strlen(p->charset);

    p->maxLength = maxLength;
    p->currentLength = currentLength;
    for (unsigned short i = 0; i < p->currentLength; i++)
    {
        p->indexes[i] = 0;
    }
    next(p);
    return p;
}

void incrementIndexes(Permutations *permutations)
{
    for (unsigned short i = 0; i < permutations->currentLength; i++)
    {
        if (permutations->indexes[i] == permutations->charsetLength - 1)
        {
            permutations->indexes[i] = 0;
        }
        else
        {
            permutations->indexes[i] = permutations->indexes[i] + 1;
            return;
        }
    }
}

int isMaxIndexes(Permutations *permutations)
{
    int maxIndexes = 1;
    for (unsigned short i = 0; i < permutations->currentLength && maxIndexes; i++)
    {
        if (permutations->indexes[i] < permutations->charsetLength - 1)
        {
            maxIndexes = 0;
        }
    }
    return maxIndexes;
}

int hasNext(Permutations *permutations)
{
    return permutations->currentLength <= permutations->maxLength;
}

void next(Permutations *permutations)
{
    for (unsigned short i = 0; i < permutations->currentLength; i++)
    {
        permutations->currentString[i] = permutations->charset[permutations->indexes[i]];
    }
}
