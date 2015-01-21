#ifndef FADE_H
#define FADE_H

#include "include/SDL.h"

void create_fade(SDL_Texture*,short,float,float);
float function(float);
int render_fade(fade*);

#endif
