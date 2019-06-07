#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wordlistcrack.h"

int crackHashByWordList(char *wordlistFileName, int (*testFunc)(char *, unsigned short, char *), char *expected)
{
    FILE *file = fopen(wordlistFileName, "r");
    if (file != NULL)
    {
        int found = 0;
        char line[256];
        while (fgets(line, sizeof line, file) != NULL && !found)
        {
            // remove the newline
            line[strlen(line) - 1] = '\0';
            if (testFunc(line, strlen(line), expected))
            {
                printf("Found: %s (%s)\n", line, expected);
                found++;
            }
        }
    }
    else
    {
        perror(wordlistFileName);
    }
    return 0;
}