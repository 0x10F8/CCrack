#include <stdio.h>
#include <stdlib.h>
#include "permutations.h"

int crackHashByCharset(unsigned char *charset, unsigned short maxLength, int (*testFunc)(char *, unsigned short, char *), char *expected)
{
    // Build the first permutations (length 1)
    Permutations *permutations = makePermutations(charset, maxLength, 1);

    int found = 0;

    // Begin crack loop
    while (hasNext(permutations) && !found)
    {

        // If the test function found a match then we're done
        if (testFunc(permutations->currentString, permutations->currentLength, expected))
        {
            printf("Found: %s (%s)\n", permutations->currentString, expected);
            found++;
        }
        // If we reached the maximum indexes for this current length then increment the length
        if (isMaxIndexes(permutations))
        {
            // Increment length, free the current struct and build a new one
            unsigned short currentLength = permutations->currentLength + 1;
            free(permutations);
            permutations = makePermutations(charset, maxLength, currentLength);
        }
        else
        {
            // If we didn't reach the max indexes for this length then just increment the indexes by 1
            incrementIndexes(permutations);
        }
        // Calculate the next current string for these indexes
        next(permutations);
    }
    return 0;
}