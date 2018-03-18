nasm -f elf64 wordsNasm.nasm
g++ -std=c++14 -Wall -Wextra -g test.cpp wordsNasm.o -o test
