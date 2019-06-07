/* Define a struct to hold the current state */
typedef struct
{
    unsigned char *currentString;
    unsigned char *charset;
    unsigned short charsetLength;
    unsigned short maxLength, currentLength;
    unsigned short indexes[];
} Permutations;

void incrementIndexes(Permutations *permutations);
int isMaxIndexes(Permutations *permutations);
void next(Permutations *permutations);
int hasNext(Permutations *permutations);
Permutations *makePermutations(unsigned char *charset, unsigned short maxLength, unsigned short currentLength);