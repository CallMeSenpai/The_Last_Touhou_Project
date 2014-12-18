#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "structs.h"
/* we are using a local edited lib */
#include "include/SDL.h"
#include "character.h"
//timed movements are important
#include <time.h>

//we should be able to access these variables
//from any scope
int window_height,window_width,full;

int main(){  
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  SDL_Window* window;
  FILE* f=NULL;
  f=fopen("config","r");
  if (!f){
    puts("settings not found! using default settings.");
    window=SDL_CreateWindow(": ^ )",0,0,0,0,
			    SDL_WINDOW_FULLSCREEN_DESKTOP|
			    SDL_WINDOW_SHOWN);
    SDL_GetWindowSize(window,&window_width,&window_height);
    printf("Window is %d by %d\n",window_width,window_height);
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
	    window_width=atoi(dimension);
	  }else if(! strcmp("height",setting_buffer)){
	    window_height=atoi(dimension);
	  }else if(! strcmp("fullscreen",setting_buffer)){
	    full=atoi(dimension);
	  }
	}
    }
    free(settings);
    window=SDL_CreateWindow("Touhou- stuy version",0,0,
			    window_width,window_height,
			    full*SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  if (! window){
    printf("Unable to create window.\n%s\n",SDL_GetError());
    exit(-1);
  }

  SDL_Renderer* renderer;
  renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
  SDL_Texture *bitmapTex;
  SDL_Surface *bitmapSurface;
  bitmapSurface = SDL_LoadBMP("bg.bmp");
  bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
  SDL_FreeSurface(bitmapSurface);
 
  //game stuff
  character c;
  set_default_values(c,window_width,window_height);
  while (1) {
    SDL_Event e;
    if (SDL_PollEvent(&e)){
      switch (e.type){
	//put cases here and call functions to modify in
	//character.c
      case SDL_QUIT:
	goto end;
      case SDL_KEYDOWN:
	key_down(c,e);
	break;
      case SDL_KEYUP:
	key_up(c,e);
	break;
      }
      //character_event(c,e);
      //    case SDL_KeyDown:
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bitmapTex, 0, 0);
    SDL_RenderPresent(renderer);
    SDL_Delay(25);//40 FPS
  }
 end:
  //SDL_Delay(3000);
  SDL_DestroyTexture(bitmapTex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();  
  return 0; 
}
