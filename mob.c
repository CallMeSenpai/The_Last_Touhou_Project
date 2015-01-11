#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "mob.h"
#include "projectile.h"
#include <math.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#define PI 3.14159265
//!!!!! include mob.c into the compile makefile


mob* summon(){
  if (!mobs){
    mobs = calloc(1,sizeof(mob));
  }else{
    mob* mob_buf=mobs;
    while(mob_buf){
      if (mob_buf->next==NULL){
	mob* new = calloc(1,sizeof(mob));
	mob_buf->next=new;
	new->prev=mob_buf;
	return new;
      }
      mob_buf=mob_buf->next;
    }
  }
}
/* just a test function to get us started */
void set_default_values_m(mob* m){
  m->x=w_width/3;
  m->y=w_height/5;
  printf("location %f, %f\n",m->x,m->y);
  m->speed=0;
  m->hp=40;
  m->angle=0;
}

void set_values_m(mob* m,int x, int y,double speed,int hp){
  m->x=x;
  m->y=y;
  m->speed=speed;
  m->hp=hp;
}
void do_action_m(mob* m){
  m->x += m->speed*cos(m->angle/180.0*PI);
  m->y -= m->speed*sin(m->angle/180.0*PI);
  if(m->x < 1 || m->y < 0 ||
     m->x > w_width || m->y > w_height || m->hp<0){//or hp=0
    if (m == mobs){//is head
      mobs=mobs->next;
      free(m);
    }else{
      m->prev->next=m->next;
      if (m->next)
	m->next->prev=m->prev;
      free(m);
    }
  }/* if bounds or hp */
  

}
