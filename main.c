#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_ttf.h"
#include "main.h"
#include "character.h"
#include "projectile.h"
#include "sprite.h"
#include "mob.h"

#define WHITE (SDL_Color){255,255,255}

int level;
unsigned long score;
char bombs,lives,grazes;
//state: 0=main menu, 1=game, 2=chat?, 3 = paused
char state;

int c_height=32;
int c_width=32;
int w_width;
int w_height;
projectile* projectiles;
mob* mobs;
int full;
character* c;
unsigned long time;//60fps time, 1/60th of a second

SDL_Window* window;
SDL_Renderer* renderer;
FILE* f;
TTF_Font* font;
enum textquality {solid, shaded, blended};
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, 0, &dst );
}

void create_window(){
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
}
/* 
   clears screen and removes stuff for the next level
   accepts 0 or 1 depending on removal of the character 
   i might change the behavior of the clear-screen funct
*/
void clear(char bool){
  mob* m_buf = mobs;
  while (m_buf){
    if (m_buf==mobs)
      mobs=mobs->next;
    mob* to_free = m_buf;
    free(to_free);
    m_buf=m_buf->next;
  }
  projectile* p_buf = projectiles;
  while (p_buf){
    if (p_buf==projectiles)
      projectiles=projectiles->next;
    projectile* to_free = p_buf;
    free(to_free);
    p_buf=p_buf->next;
  }
  if (bool)
    free(c);
}

int main(){
  time=0;
  /***** INIT SDL AND WINDOW *****/
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  create_window();
  if (TTF_Init() < 0)
    puts("error initializing");
  font = TTF_OpenFont("whitrabt.ttf", 20);
  if (font == NULL) {
    printf("Unable to load font: %s %s \n", "test", TTF_GetError());
    exit(1);
  }
  /***** INIT ALL IMAGES *****/
  //we will put bg/in-game related images in a separate
  //function later
  renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
  SDL_Texture* bg_texture = IMG_LoadTexture(renderer,"images/background.png");
  SDL_Texture* dw = IMG_LoadTexture(renderer,"images/dw.png");
  SDL_Texture* p_tex = IMG_LoadTexture(renderer,"images/star.gif");
  SDL_Texture* shade = IMG_LoadTexture(renderer,"images/transparency.png");
  SDL_Texture* score_tex = IMG_LoadTexture(renderer,"images/score.png");
  SDL_Texture* lives_tex = IMG_LoadTexture(renderer,"images/lives.png");
  SDL_Texture* grazes_tex = IMG_LoadTexture(renderer,"images/lives.png");
  SDL_Texture* bombs_tex = IMG_LoadTexture(renderer,"images/bombs.png");
  SDL_Texture* mainmenu_tex = IMG_LoadTexture(renderer,"images/mainmenu.png");
  SDL_Texture* continue_tex = IMG_LoadTexture(renderer,"images/continue.png");
  SDL_Texture* restart_tex = IMG_LoadTexture(renderer,"images/restart.png");
  SDL_Texture* start_tex = IMG_LoadTexture(renderer,"images/start.png");
  SDL_Texture* options_tex = IMG_LoadTexture(renderer,"images/options.png");
  SDL_Texture* exit_tex = IMG_LoadTexture(renderer,"images/exit.png");
  
  //continue,restart level.
  
  /***** INIT GAME VARIABLES *****/
  c = calloc(1,sizeof(character));
  set_default_values_c(c);
  /** test **/
  state=1;
  mob* m =summon();
  set_default_values_m(m);
  init_reimu_test(&c->sprite, renderer);
  /** end test **/

  while (1){
    time++;
    //printf("state: %d\n",state);
    SDL_Event e;
    if (SDL_PollEvent(&e)){
      switch (e.type){//add enter key lmao
      case SDL_QUIT:
	goto end;
      case SDL_KEYDOWN:
	key_down(e);
	break;
      case SDL_KEYUP:
	key_up(e);
	break;
      }
    }
    SDL_RenderClear(renderer); 
    /***** sprites *****/
    if (state == 1){
      if (time%10==0){
	c->sprite.current_frame++;
	c->sprite.current_frame = c->sprite.current_frame % c->sprite.frames;
      }
      handle_input(c);
    }/* if state == 1 */
    renderSprite(c->sprite.texture,renderer,c->x-16,c->y-21,31,42,&c->sprite.clip[c->sprite.current_frame]);
    /***** projectiles *****/
    projectile* p_buffer = projectiles;
    while(p_buffer){
      renderTexture(p_tex,renderer,p_buffer->x-16,p_buffer->y-16,32,32);
      if (state==1){
	do_action_p(p_buffer);
      }
      p_buffer=p_buffer->next;
      
    }
    /***** mobs *****/
    mob* m_buffer = mobs;
    while (m_buffer){
      do_action_m(m_buffer);
      renderTexture(dw,renderer,m_buffer->x-16,m_buffer->y-16,32,32);
      m_buffer=m_buffer->next;
    }
    /***** background *****/
    SDL_RenderCopy(renderer, bg_texture, 0, 0);
    
    if (state==3){
      SDL_RenderCopy(renderer, shade,0,0);
      renderTexture(continue_tex,renderer,w_width/3,w_height/3,w_width/5,w_height/15);
      renderTexture(restart_tex,renderer,w_width/3,w_height/2,w_width*7/45,w_height/15);
      renderTexture(mainmenu_tex,renderer,w_width/3,w_height/3*2,w_width/5,w_height/15);
    }else{
      
    }
    
    SDL_RenderPresent(renderer);
    SDL_Delay(16);//approx 60 FPS
    
    //time ends:
    if (time > 300){//5 sec removal
      clear(0);
      time=0;//reset
    }

  }
 end:
  SDL_DestroyTexture(bg_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return 0; 
}
