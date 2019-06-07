# CCrack
Simple hash cracker written in C

Compile:
```
gcc *.c -o crack -lcrypto
```
 
Requirements: libssl-dev

Example usage:
```
usage:  ./crack [bruteforce | dictionary] [options] [hash]
        ./crack bruteforce <charset> <maxlength> <hashtype> <expectedhash>
        ./crack dictionary <wordlist> <hashtype> <expectedhash>
```
