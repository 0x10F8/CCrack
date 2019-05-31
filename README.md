# CCrack
Simple hash cracker written in C

Compile: gcc crack.c -o crack -lcrypto
Requirements: libssl-dev

Example usage: 
./crack abcdefghijklmnopqrstuvwxyz 5 md5 098f6bcd4621d373cade4e832627b4f6

Found: test (098f6bcd4621d373cade4e832627b4f6)
