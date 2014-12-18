all:
	gcc main.c character.c $(shell sdl2-config --cflags --libs)
