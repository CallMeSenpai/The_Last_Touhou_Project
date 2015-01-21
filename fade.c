#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "fade.h"
void create_fade(SDL_Texture* tex,short frames,float x, float y){
  fade* new = calloc(1,sizeof(fade));
  if (fades == NULL){
    fades=new;
  }else{
    fades->prev=new;
    new->next = fades;
    fades = new;
  }
  new->max_frames=frames;
  new->tex = tex;  
  new->x = x;
  new->y = y;
}
float function(float x){
  return 1-(x-1)*(x-1);
}
int render_fade(fade* fade){
  //render alpha of f(frames) here
  float alpha = function(2.0*fade->frames/fade->max_frames);
  SDL_SetTextureAlphaMod(fade->tex,(int)(alpha*255));
  //printf("orig is %f\talpha is %d\n",alpha,(int)(alpha*255));
  printf("printing at %d, %d\n",(int)fade->x,(int)fade->y);
  
  renderTexture(fade->tex,renderer,w_width/3,w_height/3,w_width/6,w_height/12);
  if (state==2)
    fade->frames++;
  
  if (fade->frames >= fade->max_frames){
    if (fade==fades){
      fades=fades->next;
      free(fade);
    }else{
      fade->prev->next=fade->next;
      if (fade->next)
	fade->next->prev = fade->prev;
      free(fade);
    }
  }
  //parabola with range 0~1. solutions/zeroes at 0 and MAX_FRAMES
  
}
