#ifndef CHARACTER_H
#define CHARACTER_H

#include "include/SDL.h"

void set_default_values_c(character*);
void key_up(character*,SDL_Event);
void key_down(character*,SDL_Event);
void handle_input(character*);

#endif
