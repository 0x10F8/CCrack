#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashtypes.h"
#include "wordlistcrack.h"
#include "bruteforcecrack.h"

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

#define BRUTEFORCE "bruteforce"
#define DICTIONARY "dictionary"
static char *hashTypes[] = {"md5", "sha1"};

typedef int (*testFunc)(char *, unsigned short, char *);
int printUsage();
int hashTypeExists(char *hashType);
testFunc getTestFunction(char *hashType);
char *strlwr(char *str);

int main(int argc, char *argv[])
{

    if (argc > 2 && (strcmp(BRUTEFORCE, argv[1]) == 0 || strcmp(DICTIONARY, argv[1]) == 0))
    {
        if (strcmp(BRUTEFORCE, argv[1]) == 0 && argc == 6)
        {
            // Gather arguments
            unsigned char *charset = argv[2];
            unsigned short maxLength = atoi(argv[3]);
            unsigned char *hashType = strlwr(argv[4]);
            unsigned char *expected = argv[5];

            if (!hashTypeExists(hashType))
            {
                printf("Unknown hashtype %s\n", hashType);
                return 1;
            }

            testFunc function = getTestFunction(hashType);
            return crackHashByCharset(charset, maxLength, function, expected);
        }
        if (strcmp(DICTIONARY, argv[1]) == 0 && argc == 5)
        {
            // Gather arguments
            unsigned char *wordList = argv[2];
            unsigned char *hashType = strlwr(argv[3]);
            unsigned char *expected = argv[4];

            if (!hashTypeExists(hashType))
            {
                printf("Unknown hashtype %s\n", hashType);
                return 1;
            }

            testFunc function = getTestFunction(hashType);
            return crackHashByWordList(wordList, function, expected);
        }
    }
    return printUsage();
}

int printUsage()
{
    printf("usage:  ./crack [bruteforce | dictionary] [options] [hash]\n");
    printf("        ./crack bruteforce <charset> <maxlength> <hashtype> <expectedhash>\n");
    printf("        ./crack dictionary <wordlist> <hashtype> <expectedhash>\n");
    printf("        Available hash types:\n");
    size_t n = sizeof(hashTypes) / sizeof(hashTypes[0]);
    for (unsigned short i = 0; i < n; i++)
    {
        printf("           %s\n", hashTypes[i]);
    }
    return 1;
}

char *strlwr(char *str)
{
    size_t n = strlen(str);
    char* lower = malloc(n+1 * sizeof(unsigned char));
    for (unsigned short i = 0; i < n; i++)
    {
        lower[i] = tolower(str[i]);
    }
    lower[n] = '\0';
    return lower;
}

int hashTypeExists(char *hashType)
{
    int exists = 0;
    size_t n = sizeof(hashTypes) / sizeof(hashTypes[0]);
    for (unsigned short i = 0; i < n && !exists; i++)
    {
        if (strcmp(hashType, hashTypes[i]) == 0)
        {
            exists = 1;
        }
    }
    return exists;
}

testFunc getTestFunction(char *hashType)
{
    testFunc function;
    // Setup the test function depending on the hashtype required
    if (strcmp(hashType, "md5") == 0)
    {
        function = &testMD5;
    }
    else if (strcmp(hashType, "sha1") == 0)
    {
        function = &testSHA1;
    }
    return function;
}