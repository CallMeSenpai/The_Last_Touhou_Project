#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "main.h"
//#include <time.h>
int c_height=32;
int c_width=32;

//consider using the chrono library for getting
//milliseconds to manipulate time
#include "character.h"
#include "projectile.h"
#include "sprite.h"
//#include "mob.h"

int c_height;
int c_width;
int w_width;
int w_height;
projectile* projectiles;
mob* mobs;
int full;
character* c;
unsigned long time;//time in degrees or 1/60th of a second.

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, 0, &dst );
}

//Made for animation.
/** this has been moved to sprite.c, awaiting deletion
void renderClip(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, SDL_Rect* clip){
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, clip, &dst );
}
**/

int main(){
  time=0;
  /***** INIT SDL AND WINDOW *****/
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  SDL_Window* window;
  FILE* f=NULL;
  f=fopen("config","r");
  if (!f){
    puts("settings not found! using default settings.");
    window=SDL_CreateWindow(": ^ )",0,0,0,0,
			    SDL_WINDOW_FULLSCREEN_DESKTOP|
			    SDL_WINDOW_SHOWN);
    SDL_GetWindowSize(window,&w_width,&w_height);
    printf("Window is %d by %d\n",w_width,w_height);
  }else{
    char* settings = calloc(1,256);
    char* setting;
    fread(settings,256,1,f);
    fclose(f);
    while(setting=strsep(&settings,"\n")){
      while (setting[0] && setting[0]==10)
	setting = strsep(&settings,"\n");
      if (setting[0])
	if (setting[0] != '#'){
	  char* setting_buffer;
	  char* dimension;
	  setting_buffer=strsep(&setting,"=");
	  dimension=strsep(&setting,"=");
	  if (! strcmp("width",setting_buffer)){
	    w_width=atoi(dimension);
	  }else if(! strcmp("height",setting_buffer)){
	    w_height=atoi(dimension);
	  }else if(! strcmp("fullscreen",setting_buffer)){
	    full=atoi(dimension);
	  }
	}
    }
    free(settings);
    window=SDL_CreateWindow("Touhou- stuy version",0,0,
			    w_width,w_height,
			    full*SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  if (! window){
    printf("Unable to create window.\n%s\n",SDL_GetError());
    exit(-1);
  }

  /***** INIT ALL IMAGES *****/
  SDL_Renderer* renderer;
  renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
  SDL_Surface* bg_surface;
  SDL_Texture* bg_texture;
  SDL_RWops* rwop=SDL_RWFromFile("background.png", "rb");
  bg_surface=IMG_LoadPNG_RW(rwop);
  bg_texture=SDL_CreateTextureFromSurface(renderer, bg_surface);
  //edit bg to not have blank space in the upper black region
  //white region in the middle
  SDL_Texture* dw = IMG_LoadTexture(renderer,"dw.png");
  //we will need sprites rather than
  //dependencies on a gif.
  SDL_Texture* p_tex = IMG_LoadTexture(renderer,"star.gif");

  /***** INIT GAME VARIABLES *****/
  c = calloc(1,sizeof(character));
  /***** animation test part 2 *****/
  init_reimu_test(&c->sprite, renderer);
  /*
  c->sprite.frames = 4;
  c->sprite.clip = calloc(4, sizeof(SDL_Rect));
  c->sprite.current_frame = 0;
  c->sprite.texture = IMG_LoadTexture(renderer,"reimu_test_sheet.png");
  */
  //inaccurate, testing
  /*
  c->sprite.clip[0].x = 0;
  c->sprite.clip[0].y = 0;
  c->sprite.clip[0].w = 31;
  c->sprite.clip[0].h = 42;

  c->sprite.clip[1].x = 31;
  c->sprite.clip[1].y = 0;
  c->sprite.clip[1].w = 31;
  c->sprite.clip[1].h = 42;

  c->sprite.clip[2].x = 62;
  c->sprite.clip[2].y = 0;
  c->sprite.clip[2].w = 31;
  c->sprite.clip[2].h = 42;

  c->sprite.clip[3].x = 93;
  c->sprite.clip[3].y = 0;
  c->sprite.clip[3].w = 31;
  c->sprite.clip[3].h = 42;
  */
  //call projectls and mobs by extern variable 
  set_default_values_c(c);
  while (1){
    time++;
    SDL_Event e;
    if (SDL_PollEvent(&e)){
      switch (e.type){//add escape and enter keys lmao
      case SDL_QUIT:
	goto end;
      case SDL_KEYDOWN:
	key_down(c,e);
	break;
      case SDL_KEYUP:
	key_up(c,e);
	break;
      }
    }
    handle_input(c);
    SDL_RenderClear(renderer); 

    renderSprite(c->sprite.texture,renderer,c->x-16,c->y-21,31,42,&c->sprite.clip[c->sprite.current_frame]);
    if (time%10==0){
      c->sprite.current_frame++;
      c->sprite.current_frame = c->sprite.current_frame % c->sprite.frames;
    }
    projectile* p_buffer = projectiles;
    while(p_buffer){
      renderTexture(p_tex,renderer,
		    p_buffer->x-16,p_buffer->y-16,32,32);      
      do_action_p(p_buffer);
      p_buffer=p_buffer->next;
    }
    SDL_RenderCopy(renderer, bg_texture, 0, 0);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);//approx 60 FPS
  }
 end:
  SDL_DestroyTexture(bg_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();  
  return 0; 
}
