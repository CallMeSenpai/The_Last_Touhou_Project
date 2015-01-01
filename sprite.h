#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "character.h"
/* we need this lib for shooting */
#include "projectile.h"
/* we are using a local edited lib */
#include "include/SDL.h"

void renderSprite(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, SDL_Rect* clip);

#endif
