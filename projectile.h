#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "include/SDL.h"

char interact(mob*,projectile*,int);
void set_default_values_p(projectile*);
void manage_projectiles(projectile**,int);
void set_values(projectile*);
void do_action_p(projectile* p);

#endif
