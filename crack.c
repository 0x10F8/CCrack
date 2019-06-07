#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "permutations.h"
#include "hashtypes.h"

/* 
 * Memory efficent generic hash cracking tool.
 *
 * c0g (0x10F8)
 * 
 * TODO: 
 *  - More hash types
 *  - Multi-threading
 * 
 */

int main(int argc, char *argv[])
{
    // Expected arguments ./crack <charset> <maxlength> <hashtype> <expectedhash>
    if (argc != 5)
    {
        printf("usage: ./crack <charset> <maxlength> <hashtype> <expectedhash>\n");
        return 1;
    }

    // Gather arguments
    unsigned char *charset = argv[1];
    unsigned short maxLength = atoi(argv[2]);
    unsigned char *hashType = argv[3];
    unsigned char *expected = argv[4];
    int (*testFunc)(char *, unsigned short, char *);

    // Setup the test function depending on the hashtype required
    if (strcmp(hashType, "md5") == 0)
    {
        testFunc = &testMD5;
    }
    else if (strcmp(hashType, "sha1") == 0)
    {
        testFunc = &testSHA1;
    }
    else
    {
        printf("Unknown hashtype %s\n", hashType);
    }
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