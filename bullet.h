#ifndef BULLET_H
#define BULLET_H
#include "include/SDL.h"

bullet* create();
void set_default_values_b(bullet*,int,int);
void set_values_b(bullet*,double,double);
void set_angle(bullet*,double);
void set_speed(bullet*,double);
void set_dv(bullet*,double);
void set_da(bullet*,double);
void interact_b(bullet* b);
#endif
