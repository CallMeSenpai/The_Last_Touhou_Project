#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "mob.h"
#include "projectile.h"
#include <math.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#define PI 3.14159265

mob* summon(){
  if (!mobs){
    mobs=calloc(1,sizeof(mob));
    return mobs;
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
  m->speed=0;
  m->hp=40;
  m->angle=0;
}

void set_values_m(mob* m,int x, int y,double speed,int hp,short angle){
  m->x=x;
  m->y=y;
  m->speed=speed;
  m->hp=hp;
  m->angle = angle;
}
void do_action_m(mob* m){
  m->x += m->speed*cos(m->angle/180.0*PI);
  m->y -= m->speed*sin(m->angle/180.0*PI);
  /* if bounds or hp */
  if (time- m->last_shot > m->delay){
    m->behavior(m->x,m->y,270,5);
    m->last_shot=time;
  }
  
  projectile* p = projectiles;
  while ( p ){
    if (fabs(m->x - p->x) < w_width/20 && fabs(m->y - p->y ) < w_height/20){
      if (p == projectiles){
	projectiles=projectiles->next;
	free(p);
      }else{
	p->prev->next=p->next;
	if (p->next)
	  p->next->prev=p->prev;
	free(p);
      }
      m->hp--;
    }
    p=p->next;
  }
}
void check_remove(mob* m){
  if(m->x < 0 || m->y < 0 ||
     m->x > w_width*5/8 || m->y > w_height || m->hp<0){//or hp=0
    if (m == mobs){//is head
      mobs=mobs->next;
      free(m);
    }else{
      m->prev->next=m->next;
      if (m->next)
	m->next->prev=m->prev;
      free(m);
    }
  }
}
