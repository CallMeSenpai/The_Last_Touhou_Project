#ifndef BULLET_H
#define BULLET_H
#include "include/SDL.h"

bullet* create();
void set_default_values_b(bullet*,int,int);
void set_values_b(bullet*,int,int);
void set_angle(bullet*,short);
void set_speed(bullet*,double);

#endif
