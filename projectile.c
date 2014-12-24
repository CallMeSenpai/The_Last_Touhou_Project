#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "projectile.h"
#include <math.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#define PI 3.14159265

void set_default_values_p(projectile* p){
  p->x=c->x;
  p->y=c->y;
  p->speed = 3.5;
  p->homing = 0;
  p->angle = 90;
}
void set_values_p(projectile* p,int x, int y, double speed, short angle, char homing){
  //etc
  p->x=x;
  p->y=y;
  p->speed=speed;
  p->angle=angle;
  p->homing=homing;
  
}
void do_action(projectile* p){
  while(p){
    p->x += p->speed*cos(p->angle/180*PI);
    p->y -= p->speed*sin(p->angle/180*PI);

  }
}

/*void manage_projectiles(projectile** proj_ptrs,int size){
  int i=0;
  projectile* p;
  while(i<size){
    p=proj_pts[i];
    //move the projectiles up p->
    
    i++;
  }
  }*/
