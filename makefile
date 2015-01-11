all:
	gcc main.c character.c projectile.c sprite.c mob.c level.c $(shell sdl2-config --cflags --libs) -lSDL2_image -lm -lSDL2_ttf
