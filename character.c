#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "character.h"
/* we need this lib for shooting */
#include "projectile.h"
/* we are using a local edited lib */
#include "include/SDL.h"

/* accepts the width / height & initializes */
void set_default_values_c(character* c){
  c->lives=3;
  c->bombs=3;
  c->x=w_width/3;
  c->y=w_height*7/8;
  c->left=0;
  c->up=0;
  c->down=0;
  c->right=0;
  c->speed=8;
  c->shoot=0;
  c->focus=0;
  c->bomb=0;
}

void handle_input(character* c){
  if(c->x > c_width + (w_width/80*3) && c->left)
    c->x-=c->speed;
  if(c->x < (w_width/8*5) - c_width&& c->right)
    //use window bounds as an extern var
    c->x+=c->speed;
  if(c->y > c_height + (w_height/20)&& c->up)
    c->y-=c->speed;
  if(c->y < w_height - c_height - (w_height/20) && c->down)
    c->y+=c->speed;
  if (time % 6 == 0){
    if(c->shoot){
      if(projectiles){
	/*
	projectile* p = projectiles;
	while(p->next){
	  p=p->next;
	}
	projectile* new = calloc(1,sizeof(projectile));
	set_default_values_p(new);
	new->prev=p;
	p->next=new;*/
	projectile* new = calloc(1,sizeof(projectile));
	set_default_values_p(new);
	projectiles->prev = new;
	new->next = projectiles;
	projectiles = new;
      }else{
	projectiles = calloc(1,sizeof(projectile));
	set_default_values_p(projectiles);
      }
      //cooldown?
    }
  }
}
