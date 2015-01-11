#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#include "include/SDL_image.h"
extern char state,bombs,lives,grazes;
extern char menu_options, menu_options;
extern unsigned long score;
extern int c_height;
extern int c_width;//character dimensions
extern int w_height;
extern int w_width;//window dimensions
extern unsigned long time;

//add functions from main,c
void key_down(SDL_Event);
void key_up(SDL_Event);
void start();
void title();
void levels();

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
  //we might not need to make homing because it is time consuming
  char homing;
  struct projectile* next;
  struct projectile* prev;
} projectile;
extern projectile* projectiles;

typedef struct mob{
  int hp;//char might be too low
  double x,y; //mob.c:33.3 also changed
  double speed;
  short angle;
  struct mob* next;
  struct mob* prev;
  unsigned long spawn_time;
} mob;
extern mob* mobs;
typedef struct bullet{
  int hp, x, y;
  double speed;
  short angle;
  struct bullet* next;
  struct bullet* prev;
  char width,height;
  //might want different size bullets?
} bullet;
extern bullet* bullets;

#endif
