all:
	gcc main.c character.c projectile.c sprite.c mob.c bullet.c level.c input.c fade.c $(shell sdl2-config --cflags --libs) -lSDL2_image -lm -lSDL2_ttf
	#gcc main.c character.c projectile.c sprite.c mob.c bullet.c level.c input.c fade.c -Llib
