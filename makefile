all:
	gcc -c main.c
	gcc -c character.c
	gcc main.o character.o $(shell sdl2-config --cflags --libs) -lSDL2_image
