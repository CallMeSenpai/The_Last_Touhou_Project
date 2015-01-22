#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#include "include/SDL_image.h"
extern char state,bombs,lives,grazes;
extern char menu_options, menu_index;
extern unsigned long score;
extern int c_height;
extern int c_width;//character dimensions
extern int w_height;
extern int w_width;//window dimensions
extern unsigned long time,last_death;
extern SDL_Renderer* renderer;
void start();
void title();
void levels();
void clear(char);
typedef struct sprite{
  int frames;
  int current_frame;
  SDL_Rect* clip;
  SDL_Texture* texture;
} sprite;

typedef struct character{
  char lives,bombs;
  double x,y;
  double speed;
  //booleans
  char left,right,up,down;
  char shoot,focus,bomb;
  sprite sprite;
} character; 
extern character* c;

typedef struct projectile{
  double x,y;
  double speed;
  short angle;
  //we might not need to make homing because it is time consuming
  char homing;
  struct projectile* next;
  struct projectile* prev;
} projectile;
extern projectile* projectiles;

typedef void (*behavior)(double,double); 
//extern behavior* test_shoot;
typedef struct mob{
  int hp;
  double x,y; 
  double speed;
  double dv; //out of 100
  short angle;
  struct mob* next;
  struct mob* prev;
  unsigned long spawn_time;
  char id;
  //int function pointer for shoot()
  long unsigned last_shot;
  char delay;
  int load_time; //delay before first shot
  char set;//the num of shots each boss shoots in a "set"
  behavior behavior;
} mob;
extern mob* mobs;

typedef struct bullet{
  int hp;
  double x, y;
  double speed;
  double dv;
  double angle;
  double da;
  struct bullet* next;
  struct bullet* prev;
  char width,height;
  //might want different size bullets?
} bullet;
extern bullet* bullets;

typedef struct fade{
  float x,y;//the percentage of the window's width height
  //the # frames that stays the texture is shown in parabolic form
  short frames,max_frames;
  SDL_Texture* tex;
  struct fade* next;
  struct fade* prev;
} fade;
extern fade* fades;

#endif
