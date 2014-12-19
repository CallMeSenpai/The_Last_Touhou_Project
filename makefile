1all:
	gcc main.c character.c $(shell sdl2-config --cflags --libs) -lSDL2_image
