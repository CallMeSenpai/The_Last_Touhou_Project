#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "projectile.h"
#include <math.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#define PI 3.14159265


void set_default_values_p(projectile* p){
  p->x=c->x;
  p->y=c->y;
  p->speed = w_height/20;
  p->homing = 0;
  p->angle = 90;
}
void set_values_p(projectile* p,int x, int y, double speed, short angle, char homing){
  p->x=x;
  p->y=y;
  p->speed=speed;
  p->angle=angle;
  p->homing=homing;
}
void do_action_p(projectile* p){
  p->x += p->speed*cos((double)p->angle/180.0*PI);
  p->y -= p->speed*sin((double)p->angle/180.0*PI);
  if(p->x < 0 || p->y < 0 ||
     p->x > w_width*5/8 || p->y > w_height){//or hp=0
    if (p == projectiles){//is head
      projectiles=projectiles->next;
      free(p);
    }else{
      p->prev->next=p->next;
      if (p->next)
	p->next->prev=p->prev;
      free(p);
    }
  }
}
