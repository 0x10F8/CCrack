#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef int (*testFunc)(char *, unsigned short, char *);
int printUsage();
testFunc getTestFunction(char *hashType);

int printUsage()
{
    printf("usage:  ./crack [bruteforce | dictionary] [options] [hash]\n");
    printf("        ./crack bruteforce <charset> <maxlength> <hashtype> <expectedhash>\n");
    printf("        ./crack dictionary <wordlist> <hashtype> <expectedhash>\n");
    return 1;
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
    else
    {
        printf("Unknown hashtype %s\n", hashType);
        perror(hashType);
    }
    return function;
}

int main(int argc, char *argv[])
{

    if (argc > 2 && (strcmp(BRUTEFORCE, argv[1]) == 0 || strcmp(DICTIONARY, argv[1]) == 0))
    {
        if (strcmp(BRUTEFORCE, argv[1]) == 0 && argc == 6)
        {
            // Gather arguments
            unsigned char *charset = argv[2];
            unsigned short maxLength = atoi(argv[3]);
            unsigned char *hashType = argv[4];
            unsigned char *expected = argv[5];
            testFunc function = getTestFunction(hashType);
            return crackHashByCharset(charset, maxLength, function, expected);
        }
        if (strcmp(DICTIONARY, argv[1]) == 0 && argc == 5)
        {
            // Gather arguments
            unsigned char *wordList = argv[2];
            unsigned char *hashType = argv[3];
            unsigned char *expected = argv[4];
            testFunc function = getTestFunction(hashType);
            return crackHashByWordList(wordList, function, expected);
        }
    }
    return printUsage();
}