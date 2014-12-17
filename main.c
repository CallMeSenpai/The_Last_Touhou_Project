#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "structs.h"
/* we are using a local edited lib */
#include "include/SDL.h"
//timed movements are important
#include <time.h>

int main(){

  /*
   *all our vars are going to be readable
   *because machine code is gonna reduce
   *the size anyway
   */
  
  char* settings = calloc(1,256);
  char* setting;
  //char newline = '\n';
  FILE* f=NULL;
  f=fopen("config","r");
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  SDL_Window* window;
  int window_height,window_width;
  if (!f){
    puts("settings not found! using default settings.");
    window=SDL_CreateWindow(": ^ )",0,0,0,0,
			    SDL_WINDOW_FULLSCREEN_DESKTOP|
			    SDL_WINDOW_SHOWN);
    SDL_GetWindowSize(window,&window_width,&window_height);
    printf("Window is %d by %d\n",window_width,window_height);
  }else{
    fread(settings,256,1,f);
    fclose(f);
    while(setting=strsep(&settings,"\n")){
      while (setting[0] && setting[0]==10)
	setting = strsep(&settings,"\n");
      if (setting[0])
	if (setting[0] != '#'){
	  char* setting_buffer;
	  setting_buffer=strsep(&setting,"=");
	  printf("string_buffer: %s\n",setting_buffer);
	  setting_buffer=strsep(&setting,"=");
	  printf("string_buffer: %s\n",setting_buffer);
	}
      
    }
    window_height=600;
    window_width=800;
    window=SDL_CreateWindow("Touhou- stuy version",0,0,
			    window_width,
			    window_height,0);
  }
  if (! window){
    printf("Unable to create window.\n%s\n",SDL_GetError());
    exit(-1);
  }

  
  SDL_Renderer* renderer;
  renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
  SDL_Texture *bitmapTex;
  SDL_Surface *bitmapSurface;
  bitmapSurface = SDL_LoadBMP("dw.bmp");
  bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
  SDL_FreeSurface(bitmapSurface);
 

  //game stuff
  character c;
  while (1) {
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
	break;
      }
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bitmapTex, 0, 0);
    SDL_RenderPresent(renderer);
    //We will use a 40FPS?
    SDL_Delay(25);
    //puts("hi man");
  }
  
  //SDL_Delay(3000);
  SDL_DestroyTexture(bitmapTex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();  
  return 0; 
}
