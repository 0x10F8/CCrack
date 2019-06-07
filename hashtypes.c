#include <openssl/md5.h>
#include <openssl/sha.h>
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

int testSHA1(char* text, unsigned short length, char* expected) {
    unsigned char result[SHA_DIGEST_LENGTH];
    SHA1(text, length, result);
    unsigned char sha1String[41];
    for(int i = 0; i < 20; i++) {
         sprintf(&sha1String[i*2], "%02x", (unsigned int)result[i]);
    }
    return strcmp(sha1String, expected) == 0;
}