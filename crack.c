#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

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

/* Define a struct to hold the current state */
typedef struct  {
    unsigned char* currentString;
    unsigned char* charset;
    unsigned short charsetLength;
    unsigned short maxLength, currentLength;
    unsigned short indexes[];
} Permutations;

/* Declare functions */
void incrementIndexes(Permutations *permutations);
int isMaxIndexes(Permutations *permutations);
void next(Permutations *permutations);
int hasNext(Permutations *permutations);
int testMD5(Permutations *permutations, char* expected);
Permutations* makePermutations(unsigned char* charset, unsigned short maxLength, unsigned short currentLength);

int main(int argc, char* argv[]) {

    // Expected arguments ./crack <charset> <maxlength> <hashtype> <expectedhash>
    if (argc != 5) {
        printf("usage: ./crack <charset> <maxlength> <hashtype> <expectedhash>\n");
        return 1;
    }

    // Gather arguments
    unsigned char* charset = argv[1];
    unsigned short maxLength = atoi(argv[2]);
    unsigned char* hashType = argv[3];
    unsigned char* expected = argv[4];
    int (*testFunc)(Permutations*, char*);

    // Setup the test function depending on the hashtype required
    if (strcmp(hashType, "md5") == 0) {
        testFunc = &testMD5;
    } else {
        printf("Unknown hashtype %s\n", hashType);
    }
    // Build the first permutations (length 1)
    Permutations* permutations = makePermutations(charset, maxLength, 1);

    // Begin crack loop
    while(hasNext(permutations)) {

        // If the test function found a match then we're done
        if (testFunc(permutations, expected)) {
            printf("Found: %s (%s)\n", permutations->currentString, expected);
            return 0;
        }
        // If we reached the maximum indexes for this current length then increment the length
        if (isMaxIndexes(permutations)) {
            // Increment length, free the current struct and build a new one
            unsigned short currentLength = permutations->currentLength+1;
            free(permutations);
            permutations = makePermutations(charset, maxLength, currentLength);
        } else {
            // If we didn't reach the max indexes for this length then just increment the indexes by 1
            incrementIndexes(permutations);
        }
        // Calculate the next current string for these indexes
        next(permutations);
    }
    return 0;
}

int testMD5(Permutations *permutations, char* expected) {
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(permutations->currentString, permutations->currentLength, result);
    unsigned char mdString[33];
    for(int i = 0; i < 16; i++) {
         sprintf(&mdString[i*2], "%02x", (unsigned int)result[i]);
    }
    return strcmp(mdString, expected) == 0;
}

Permutations* makePermutations(unsigned char* charset, unsigned short maxLength, unsigned short currentLength) {
    Permutations* p = malloc(sizeof(Permutations)+currentLength*sizeof(unsigned short));
    p->charset = charset;
    p->currentString = malloc(sizeof(unsigned char) * currentLength);
    p->charsetLength = strlen(p->charset);
    
    p->maxLength = maxLength;
    p->currentLength = currentLength;
    for (unsigned short i = 0; i < p->currentLength; i++) {
         p->indexes[i] = 0;
    }
    next(p);
    return p;
}

void incrementIndexes(Permutations *permutations) {
    for (unsigned short i = 0; i < permutations->currentLength; i++) {
        if (permutations->indexes[i] == permutations->charsetLength-1) {
            permutations->indexes[i] = 0;
        } else {
            permutations->indexes[i] = permutations->indexes[i]+1;
            return;
        }
    }
}

int isMaxIndexes(Permutations *permutations) {
    int maxIndexes = 1;
    for (unsigned short i = 0; i < permutations->currentLength && maxIndexes; i++) {
        if (permutations->indexes[i] < permutations->charsetLength-1) {
            maxIndexes = 0;
        }
    }
    return maxIndexes;
}

int hasNext(Permutations *permutations) {
    return permutations->currentLength <= permutations->maxLength;
}

void next(Permutations *permutations) {
    for (unsigned short i = 0; i < permutations->currentLength; i++) {
        permutations->currentString[i] = permutations->charset[permutations->indexes[i]];
    }
}
