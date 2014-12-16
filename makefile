all:
	gcc main.c $(shell sdl2-config --cflags --libs)
