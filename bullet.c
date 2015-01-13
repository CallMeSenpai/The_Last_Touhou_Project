#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "mob.h"
#include "bullet.h"
#include <math.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#define PI 3.14159265

//homing missiles to character/ aiming? requires arc trig

bullet* create(){
  if (bullets == NULL){
    bullets = calloc(1,sizeof(bullet));
    return bullets;
  }else{
    bullet* bullet_buf = bullets;
    while (bullet_buf){
      if (bullet_buf->next==NULL){
	bullet* new = calloc(1,sizeof(bullet));
	bullet_buf->next=new;
	new->prev=bullet_buf;
	return new;
      }
      bullet_buf = bullet_buf->next;
    }
  }
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
void do_action_b(bullet* b){
  b->x += b->speed*cos(b->angle/180.0*PI);
  b->y -= b->speed*sin(b->angle/180.0*PI);


}
//set dimensions of bullet? or static vars as defined vars
