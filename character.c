#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "character.h"
/* we need this lib for shooting */
#include "projectile.h"
/* we are using a local edited lib */
#include "include/SDL.h"

/* accepts the width / height & initializes */
void set_default_values_c(character* c){
  c->lives=3;
  c->bombs=3;
  c->x=w_width/2;
  c->y=w_height/2;
  c->left=0;
  c->up=0;
  c->down=0;
  c->right=0;
  c->speed=5;
  c->shoot=0;
  c->focus=0;
  c->bomb=0;
}
/* split into key down, key up and put the keydowns into main.c */
void key_down(character* c,SDL_Event e){
  switch (e.key.keysym.sym){
  case SDLK_LEFT:  
    if(!c->left)
      c->left=1;
    break;
  case SDLK_RIGHT: 
    if(!c->right)
      c->right=1;
    break;
  case SDLK_UP:    
    if(!c->up)
      c->up=1;
    break;
  case SDLK_DOWN:  
    if(!c->down)
      c->down=1;
    break;
  case SDLK_LSHIFT:
    if(!c->focus)
      c->focus=1;
    break;
  case SDLK_z:
    if(!c->shoot)
      c->shoot=1;
    break;
  case SDLK_x:
    if(!c->bomb)
      c->bomb=1;
    break;
  }/* switch e.key.keysym */
}
void key_up(character* c, SDL_Event e){
  switch (e.key.keysym.sym){
  case SDLK_LEFT:  
    if(c->left)
      c->left=0;
    break;
  case SDLK_RIGHT: 
    if(c->right)
      c->right=0;
    break;
  case SDLK_UP:    
    if(c->up)
      c->up=0;
    break;
  case SDLK_DOWN:  
    if(c->down)
      c->down=0;
    break;
  case SDLK_LSHIFT:
    if(c->focus)
      c->focus=0;
    break;
  case SDLK_z:
    if(c->shoot)
      c->shoot=0;
    break;
  case SDLK_x:
    if(c->bomb)
      c->bomb=0;
    break;
  }/* switch e.key.keysym- up */
}
void handle_input(character* c){
  if(c->x > c_width && c->left)
    c->x-=c->speed;
  if(c->x < w_width - c_width && c->right)
    //use window bounds as an extern var
    c->x+=c->speed;
  if(c->y > c_height && c->up)
    c->y-=c->speed;
  if(c->y < w_height - c_height && c->down)
    c->y+=c->speed;
  if(c->shoot){
    if(projectiles){
      projectile* p = projectiles;
      int index=0;
      while(p->next){
	p=p->next;
	index++;
      }
      printf("there are %d projectiles\n",index);
      projectile* new = calloc(1,sizeof(projectile));
      set_default_values_p(new);
      new->prev=p;
      p->next=new;
    }else{
      projectiles = calloc(1,sizeof(projectile));
      set_default_values_p(projectiles);
    }
    //cooldown?
  }
}
