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

void summon(){
  if (!mobs){
    mobs = calloc(1,sizeof(mob));
  }else{
    mob* mob_buf=mobs;
    while(mob_buf){
      if (mob_buf->next==NULL){
	mob* new = calloc(1,sizeof(mob));
	mob_buf->next=new;
	new->prev=mob_buf;
	set_default_values_m(new);
	return;
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
  m->hp=420;

}

void set_values_m(mob* m,int x, int y,double speed,int hp){
  m->x=x;
  m->y=y;
  m->speed=speed;
  m->hp=hp;
}
