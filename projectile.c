#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "projectile.h"
#include <math.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#define PI 3.14159265

//returns 0 or 1 if it is in the square
char interact(mob* m, projectile* p,int radius){
  if (fabs(m->x - p->x) < radius && fabs(m->y - p->y) < radius){
    return 1;
  }
  return 0;
}

void set_default_values_p(projectile* p){
  p->x=c->x;
  p->y=c->y;
  p->speed = w_height/40;
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
     p->x > w_width || p->y > w_height){//or hp=0
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
  /* loop through mobs and check if objects are near */
  mob* m_buf = mobs;
  while (m_buf){
    projectile* p_buf = projectiles;
    while (p_buf){
      if (interact(m_buf,p_buf,30)){
	//radius accepted will be image height/width!
	if (p == projectiles){
	  projectiles=projectiles->next;
	  free(p);
	}else{
	  p->prev->next=p->next;
	  if (p->next)
	    p->next->prev=p->prev;
	  free(p);
	}
	m_buf->hp--;
	printf("mob hp decreased to %d\n",m_buf->hp);
      }
      p_buf=p_buf->next;
    }
    m_buf=m_buf->next;
  }
}

//__ is a _ programmer who envisions creating _ and wants _ and needs 
