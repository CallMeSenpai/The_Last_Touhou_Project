#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "mob.h"
#include "bullet.h"
#include <math.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#define PI 3.14159265

bullet* create(){
  bullet* new = calloc(1,sizeof(bullet));
  
  return new;
}

void set_default_values_b(bullet* b,int x, int y){
  b->x = x;
  b->y = y;
  b->speed = 5;
  b->width=32;
  b->height=32;
  b->angle=270;
  //make sure to kill bullets or mobs heading off the screen
}
void set_values_b(bullet* b,int x, int y,short angle,char speed){
  b->x = x;
  b->y=y;
  b->angle=angle;
}

//set dimensions of bullet? or static vars as defined vars
