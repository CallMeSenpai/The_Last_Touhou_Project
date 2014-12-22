#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "projectile.h"
/* we are using a local edited lib */
#include "include/SDL.h"

void set_default_values_p(projectile* p,character* c,char homing){
  p->x=c->x;
  p->y=c->y;
  p->speed = 3.5;
  p->homing = 0;
}
void manage_projectiles(projectile** proj_ptrs,int size){
  int i=0;
  projectile* p;
  while(i<size){
    p=proj_pts[i];
    //move the projectiles up p->
    
    i++;
  }


}
