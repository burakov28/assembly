nasm -f elf64 memcpyNasm.nasm
g++ -Wall -std=c++14 -Wextra -g test.cpp memcpyNasm.o -o test
