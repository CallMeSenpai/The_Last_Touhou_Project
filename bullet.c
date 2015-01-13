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
void set_values_b(bullet* b,int x, int y){
  b->x = x;
  b->y=y;
}
void set_angle(bullet* b, short angle){
  b->angle = angle;
}
void set_speed(bullet* b,double speed){
  b->speed=speed;
}
void do_action_b(bullet* b){
  b->x += (int)(b->speed*cos(b->angle/180.0*PI));
  b->y -= (int)(b->speed*sin(b->angle/180.0*PI));
  printf("angle for this bullet is %d. \n",b->angle);
  printf("x is now %d.\n",b->x);
}
void interact_b(bullet* b){
  //bullet kills character.
  if (fabs(c->x-16 - b->x-6)<6+16 && 
      fabs(c->y-16 - b->y-6)<6+16){
    //barak hasnt centered the character sprite yet
    puts("died");
    lives--;
    if (lives>=0){
      c->x=w_width/3;
      c->y=w_height*7/8;
    }
  }
  //to remove
  if(b->x < 0 || b->y < 0 ||
     b->x > w_width*5/8 || b->y > w_height){
    if (b == bullets){
      bullets=bullets->next;
      free(b);
    }else{
      b->prev->next=b->next;
      if (b->next)
	b->next->prev=b->prev;
      free(b);
    }
  }
}
