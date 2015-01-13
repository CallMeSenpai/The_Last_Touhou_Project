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
#include "bullet.h"
#include "level.h"

#define WHITE (SDL_Color){255,255,255}

int level;
unsigned long score;
char bombs,lives,grazes;
//state: 0=main, 1 =selections, 2 = game, 3 = paused, 4=dialogue
char state;
SDL_Event e;
int c_height=32;
int c_width=32;
int w_width;
int w_height;
projectile* projectiles;
mob* mobs;
bullet* bullets;
int full;
character* c;
unsigned long time;//60fps time, 1/60th of a second
char menu_index,menu_options;

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
/***** clears screen *****/
void clear(char bool){
  //bool: do we want to remove the character?
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

void key_down(SDL_Event e){
  if (state==2){
    switch (e.key.keysym.sym){
    case SDLK_LEFT:  
      if (!c->left){ c->left=1; }
      break;
    case SDLK_RIGHT: 
      if (!c->right){ c->right=1; }
      break;
    case SDLK_UP:    
      if(!c->up){ c->up=1; }
      break;
    case SDLK_DOWN:  
      if(!c->down){ c->down=1; }
      break;
    case SDLK_LSHIFT:
      if(!c->focus){
	c->focus=1;
	c->speed=2;
      }
      break;
    case SDLK_z:
      if(!c->shoot){ c->shoot=1; }
      break;
    case SDLK_x:
      if(!c->bomb){ c->bomb=1; }
      break;
    }/* switch e.key.keysym */
  }/* if state==2 */
}
void key_up(SDL_Event e){
  if (state==0 || state==1 || state==3){
    switch (e.key.keysym.sym){
    case SDLK_DOWN:
      menu_index=(menu_index+1)%menu_options;
      break;
    case SDLK_UP:
      menu_index=(menu_index-1+menu_options)%menu_options;
      break;
    }
  }
  if (state==0){
    switch (e.key.keysym.sym){
    case SDLK_RETURN:
      levels();
      break;
    }
  }else if ( state == 1 ){
    switch (e.key.keysym.sym){
    case SDLK_RETURN:
      //cases with exit and start,
      if ( menu_index == 0 ){ start(); } // start on ez
      if ( menu_index == 1 ){ start(); }//start on insane
      if (menu_index==2){ title(); }//back to title?
      break;
    case SDLK_ESCAPE:
      title();
      break;
    }
  }else if (state==3){
    switch (e.key.keysym.sym){
    case SDLK_ESCAPE:
      c->up=0;
      c->down=0;
      c->right=0;
      c->left=0;
      c->shoot=0;
      state=2;
      break;
    case SDLK_RETURN:
      if (menu_index==0){
        c->up=0;
	c->down=0;
	c->right=0;
	c->left=0;
	c->shoot=0;
	state=2;
	break;
      }else if (menu_index==1){//restart
	state=2;
	score=0;
	//clear(1);
	set_default_values_c(c);
	break;
      }else if (menu_index==2){ 
	clear(1);
	title(); 
	//bugs, double free @ game->title
      }
    }
  }else if (state==2){
    switch (e.key.keysym.sym){
    case SDLK_LEFT: 
      if(c->left){ c->left=0; }
      break;
    case SDLK_RIGHT: 
      if(c->right){ c->right=0; }
      break;
    case SDLK_UP:    
      if(c->up){ c->up=0; }
      break;
    case SDLK_DOWN:  
      if(c->down){ c->down=0; }
      break;
    case SDLK_LSHIFT:
      if(c->focus){
	c->focus=0;
	c->speed=5;
      }
      break;
    case SDLK_z:
      if(c->shoot){ c->shoot=0; }
      break;
    case SDLK_x:
      if(c->bomb){ c->bomb=0; }
      break;
    case SDLK_ESCAPE:
      state=3;
      menu_options=3;
      menu_index=0;
      break;
    }/* switch e.key.keysym- up */
  }/* if state=2 */
}
/***** begins the game *****/
void start(){
  /***** INIT GAME VARIABLES *****/
  c = calloc(1,sizeof(character));
  set_default_values_c(c);
  state=2;
  //mobs=summ
  //set_default_values_m(mobs);
  init_reimu_test(&c->sprite, renderer);
  time=0;
  load_dat("1e.dat");
}
void title(){
  clear(1);
  time=0;
  state=0;
  menu_options=3;
  menu_index=0;
  /*might include another options under start
    to add extra level.
    depends on time constraints
   */
  //Extra.png?
  //Entering some CS-dojo type png.
}
void levels(){
  menu_options=3;
  state=1;
  menu_index=0;
  //get images
  //Easy.png
  //Insane.png
  //Back
}
int main(){
  /***** INIT SDL AND WINDOW *****/
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  create_window();
  TTF_Init();
  //we need font only for text boxes
  font = TTF_OpenFont("whitrabt.ttf", 20);


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
  SDL_Texture* select_tex = IMG_LoadTexture(renderer,"images/select.png");
  SDL_Texture* title_tex = IMG_LoadTexture(renderer,"images/title.jpg");
  SDL_Texture* bullet_tex = IMG_LoadTexture(renderer,"images/bullet_red.png");
  /*----- test -----*/
  title();
  

  /*----- end test -----*/
  while (1){
    if (state!=3)
      time++;
    SDL_RenderClear(renderer);
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
    if (state==0){
      renderTexture(title_tex,renderer,0,0,w_width,w_height);
      renderTexture(select_tex,renderer,w_width/4-w_width/12,w_height/2+w_height/6*(menu_index-1),w_width/6,w_height/12);
      renderTexture(start_tex,renderer,w_width/4-w_width/20,w_height/3,w_width/10,w_height/15);
      renderTexture(options_tex,renderer,w_width/4-w_width/16,w_height/2,w_width/8,w_height/15);
      renderTexture(exit_tex,renderer,w_width/4-w_width/24,w_height/3*2,w_width/12,w_height/15);
    }else if (state == 1){
      renderTexture(title_tex,renderer,0,0,w_width,w_height);
      renderTexture(select_tex,renderer,w_width/4-w_width/12,w_height/2+w_height/6*(menu_index-1),w_width/6,w_height/12);
      renderTexture(mainmenu_tex,renderer,w_width/4-w_width/12,w_height/3*2,w_width/6,w_height/15);
      ////////////////////////////remember menu option images
    }else if (state==2 || state==3){
      /***** character *****/
      renderSprite(c->sprite.texture,renderer,c->x-16,c->y-21,31,42,&c->sprite.clip[c->sprite.current_frame]);
      /***** projectiles *****/
      projectile* p_buffer = projectiles;
      while(p_buffer){
	renderTexture(p_tex,renderer,p_buffer->x-16,p_buffer->y-16,32,32);
	if (state==2)
	  do_action_p(p_buffer);
	p_buffer=p_buffer->next; 
      }
      /***** mobs *****/
      mob* m_buffer = mobs;
      while (m_buffer){
	//spawn_time is still wrong
	if (m_buffer->spawn_time < time){
	  renderTexture(dw,renderer,m_buffer->x-16,m_buffer->y-16,32,32);
	  //shouldn't call the behavior due to spawn time so idk
	  if ( state == 2 )
	    do_action_m(m_buffer);
	}
	check_remove(m_buffer);
	m_buffer=m_buffer->next;
      }
      /***** bullets *****/
      bullet* b_buffer = bullets;
      while ( b_buffer ){
	renderTexture(bullet_tex,renderer,b_buffer->x-6,b_buffer->y-6,13,13);
	if ( state == 2 )
	  do_action_b(b_buffer);
	//remove bullets if off screen
	b_buffer=  b_buffer->next;
      }
      if ( state == 2 ){
	/***** sprites *****/
	if ( time%10 == 0 ){
	  c->sprite.current_frame++;
	  c->sprite.current_frame = c->sprite.current_frame % c->sprite.frames;
	}
	handle_input(c);
      }else if ( state == 3 ){
	/***** pause menu *****/
	renderTexture(shade,renderer,0,0,w_width,w_height);
	renderTexture(select_tex,renderer,w_width/3-w_width/8,w_height/2 + (w_height/6*(menu_index-1)),w_width/4,w_height/12);
	renderTexture(continue_tex,renderer,w_width/3-w_width/10,w_height/3,w_width/5,w_height/15);
	renderTexture(restart_tex,renderer,w_width/3-w_width/12,w_height/2,w_width/6,w_height/15);
	renderTexture(mainmenu_tex,renderer,w_width/3-w_width/10,w_height/3*2,w_width/5,w_height/15);
      }
      /***** background *****/
      renderTexture(bg_texture,renderer, 0, 0,w_width,w_height);
    }/* if state==2 or state==3 */
    SDL_RenderPresent(renderer);
    SDL_Delay(16);//approx 60 FPS
  }/* while 1 */
 end:
  SDL_DestroyTexture(bg_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return 0; 
}
