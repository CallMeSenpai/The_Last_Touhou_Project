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
    //bullet* bullet_buf = bullets;
    /*
    while (bullet_buf){
      if (bullet_buf->next==NULL){
	bullet* new = calloc(1,sizeof(bullet));
	bullet_buf->next=new;
	new->prev=bullet_buf;
	return new;
      }
      bullet_buf = bullet_buf->next;
      }*/
    bullet* new = calloc(1,sizeof(bullet));
    bullets->prev = new;
    new->next=bullets;
    bullets=new;
    return bullets;
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
void set_values_b(bullet* b,double x, double y){
  b->x = x;
  b->y = y;
  b->spawn_time = time;
}
void set_angle(bullet* b, double angle){
  b->angle = angle;
}
void set_speed(bullet* b,double speed){
  b->speed=speed;
}
void set_dv(bullet* b,double dv) {
  b->dv=dv;
}
void set_da(bullet* b,double da) {
  b->da=da;
}
/* doesnt let you change vector and angualr acceleration
   after the limit parameter */
void dv_until(bullet* b,double limit){
  if ( (b->dv > 0 && b->speed < limit) ||
       (b->dv < 0 && b->speed > limit) ){
    b->speed += b-> dv;
  }else
    b->dv=0;
}
/* targets the character */
void target(bullet* b){
  double dx = fabs(b->x - c->x);
  double dy = fabs(b->y - c->y);
  double ref_angle = atan((double)dy/(double)dx)/PI*180.0;
  if (c->x < b->x)
    if (c->y > b->y)
      ref_angle+=180;
    else 
      ref_angle=180-ref_angle;
  else
    if (c->y > b->y)
      ref_angle = 360-ref_angle;
  b->angle=ref_angle;
}
void do_action_b(bullet* b){
  b->x += b->speed * cos((double)((double)b->angle/180) * PI);
  b->y -= b->speed * sin((double)((double)b->angle/180) * PI);

  if (b->movement)
    b->movement(b);
  /* temp */

  /* end temp */

  //all dv,da,angle will be modded by MOVEMENT behavior
  //and we will call it from there
  //b->speed += b->dv;
  /*!!!! new line to use: b->id_num();
    id_num given by level.c interpreter */
  b->angle += b->da;
  b->angle = fmod(b->angle,(double)360);
  
}
void interact_b(bullet* b){
  //------------don't use constants
  if (time-last_death > 180 && fabs(c->x - b->x)<5 && 
      fabs(c->y - b->y)<5){
    last_death=time;//5 second death cooldown
    lives--;
    if (lives>=0){
      c->x=w_width/3;
      c->y=w_height*7/8;
    } //else, end game
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
