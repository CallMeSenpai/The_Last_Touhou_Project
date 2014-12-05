all: main.o
	gcc -o a main.o #other dependent files, etc
main.o : structs.h
	gcc -c main.c
