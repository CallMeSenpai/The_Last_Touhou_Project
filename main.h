#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#include "include/SDL_image.h"

extern char state,bombs,lives,grazes;
extern unsigned long score;
extern int c_height;
extern int c_width;//character dimensions
extern int w_height;
extern int w_width;//window dimensions
extern unsigned long time;

//add functions from main,c

typedef struct sprite{
  int frames;
  int current_frame;
  SDL_Rect* clip;
  SDL_Texture* texture;
} sprite;

typedef struct character{
  char lives,bombs;
  int x,y;
  double speed;
  //booleans
  char left,right,up,down;
  char shoot,focus,bomb;
  sprite sprite;
} character; 
extern character* c;

typedef struct projectile{
  int x,y;
  double speed;
  short angle;
  //boolean: is this bullet a homing/targetting projectile?
  char homing;
  struct projectile* next;
  struct projectile* prev;
} projectile;
extern projectile* projectiles;

typedef struct mob{
  int hp;//char might be too low
  int x,y;
  double speed;
  short angle;
  struct mob* next;
  struct mob* prev;
  unsigned long spawn_time;
} mob;
extern mob* mobs;

#endif
