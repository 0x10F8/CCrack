#include <openssl/md5.h>
#include <stdio.h>
#include <string.h>

int testMD5(char* text, unsigned short length, char* expected) {
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(text, length, result);
    unsigned char mdString[33];
    for(int i = 0; i < 16; i++) {
         sprintf(&mdString[i*2], "%02x", (unsigned int)result[i]);
    }
    return strcmp(mdString, expected) == 0;
}