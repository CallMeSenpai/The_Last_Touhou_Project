#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
/* we are using a local edited lib */
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_ttf.h"
#include "main.h"
#include "input.h"
#include "character.h"
#include "projectile.h"
#include "sprite.h"
#include "mob.h"
#include "bullet.h"
#include "level.h"
//#include "fade.h"

#define WHITE (SDL_Color){255,255,255}

int level;
unsigned long score,time,last_death;
char bombs,lives,grazes;
//state: 0=main, 1 =selections, 2 = game, 3 = paused, 4=dialogue, 5 = game-finished, 6-multiplayer (change options->)
char state;
SDL_Event e;
int c_height=32;
int c_width=32;
int w_width;
int w_height;
projectile* projectiles;
mob* mobs;
bullet* bullets;
fade* fades;
int full;
character* c;
char menu_index,menu_options;

SDL_Window* window;
SDL_Renderer* renderer;
FILE* f;
SDL_Texture* level1_tex;
SDL_Texture* level2_tex;
SDL_Texture* level3_tex;
SDL_Texture* level4_tex;
TTF_Font* font;
int num_players;
//enum textquality {solid, shaded, blended};
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, 0, &dst );
}
//rotates images
void renderTexture_r(SDL_Texture* tex, SDL_Renderer *ren, int x, int y, int w, int h, double angle){
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopyEx(ren,tex,0,&dst,360.0-angle,0,SDL_FLIP_VERTICAL);
  //?? angles are rounded? by 15 deg...
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
    if (full){
      puts("grabbed window size");
      SDL_GetWindowSize(window,&w_width,&w_height);
    }
    window=SDL_CreateWindow("Touhou- stuy version",0,0,
			    w_width,w_height,
			    full*SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  SDL_GetWindowSize(window,&w_width,&w_height);
  if (! window){
    printf("Unable to create window.\n%s\n",SDL_GetError());
    exit(-1);
  }
}
/***** clears screen *****/
void clear(char bool){
  //clear bullets
  mob* m_buf = mobs;
  while (m_buf){
    if (m_buf==mobs)
      mobs=mobs->next;
    mob* to_free = m_buf;
    m_buf=m_buf->next;
    free(to_free);
  }
  //clear projectiles
  projectile* p_buf = projectiles;
  while (p_buf){
    if (p_buf==projectiles)
      projectiles=projectiles->next;
    projectile* to_free = p_buf;
    p_buf=p_buf->next;
    free(to_free);
  }
  //clear bullets
  bullet* b_buf = bullets;
  while (b_buf){
    if (b_buf==bullets)
      bullets = bullets->next;
    bullet* to_free = b_buf;
    b_buf=b_buf->next;
    free(to_free);
  }
  //clear character
  if (bool && c)
    free(c);
}

/***** INIT GAME VARIABLES *****/
void start(int num){
  num_players = num;
  //printf("the original dimensions are %d, %d\n",w_width,w_height);
  create_fade(level1_tex,150,w_width/3,w_height/3);
  c = calloc(num,sizeof(character));
  
  set_default_values_c(c);
  state=2;
  //init_reimu_test(&c->sprite, renderer);
  time=0;
  load_dat("1e.dat");
  lives=3;//might set in options
  bombs=3;//options
  last_death=0;
  level=1;
  //create_fade(level1_tex,150,0.33f,0.33f);
}
void next(){
  if (level==2){
    puts("sup");
    load_dat("2e.dat");
    time=0;
    last_death=0;
  }
  level++;
}
void title(){
  clear(1);
  time=0;
  state=0;
  menu_options=3;
  menu_index=0;
  //Extra.png?
}
void multi(){
  state=6;
  menu_options=3;
  menu_index=0;
  puts("lol multi");
}
void levels(){
  menu_options=3;
  state=1;
  menu_index=0;
  //Easy.png
  //Insane.png
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
  SDL_Texture* temp = IMG_LoadTexture(renderer,"images/dw.bmp");
  SDL_Texture* p_tex = IMG_LoadTexture(renderer,"images/star.gif");
  SDL_Texture* shade = IMG_LoadTexture(renderer,"images/transparency.png");
  SDL_Texture* score_tex = IMG_LoadTexture(renderer,"images/score.png");
  SDL_Texture* lives_tex = IMG_LoadTexture(renderer,"images/lives.png");
  SDL_Texture* grazes_tex = IMG_LoadTexture(renderer,"images/grazes.png");
  SDL_Texture* bombs_tex = IMG_LoadTexture(renderer,"images/bombs.png");
  SDL_Texture* mainmenu_tex = IMG_LoadTexture(renderer,"images/mainmenu.png");
  SDL_Texture* easy_tex = IMG_LoadTexture(renderer,"images/easy.png");
  SDL_Texture* insane_tex = IMG_LoadTexture(renderer,"images/insane.png");
  SDL_Texture* continue_tex = IMG_LoadTexture(renderer,"images/continue.png");
  SDL_Texture* restart_tex = IMG_LoadTexture(renderer,"images/restart.png");
  SDL_Texture* start_tex = IMG_LoadTexture(renderer,"images/start.png");

  SDL_Texture* options_tex = IMG_LoadTexture(renderer,"images/options.png");

  SDL_Texture* exit_tex = IMG_LoadTexture(renderer,"images/exit.png");
  SDL_Texture* select_tex = IMG_LoadTexture(renderer,"images/select.png");
  SDL_Texture* title_tex = IMG_LoadTexture(renderer,"images/title.jpg");
  SDL_Texture* bullet_tex = IMG_LoadTexture(renderer,"images/bullet_red.png");
  SDL_Texture* server_tex = IMG_LoadTexture(renderer,"images/server.png");
  SDL_Texture* client_tex = IMG_LoadTexture(renderer,"images/client.png");
  SDL_Texture* multi_tex = IMG_LoadTexture(renderer,"images/multi.png");
  level1_tex = IMG_LoadTexture(renderer,"images/level1.png");
  level2_tex = IMG_LoadTexture(renderer,"images/level2.png");
  level3_tex = IMG_LoadTexture(renderer,"images/level3.png");
  level4_tex = IMG_LoadTexture(renderer,"images/level4.png");
  title();
  while (1){
    break;
  }//while host

  while (1){//while server
    if (state!=3)
      time++;
    if (time > 60*10){
      next();
    }
    SDL_RenderClear(renderer);
    if (SDL_PollEvent(&e)){
      switch (e.type){
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
      renderTexture(multi_tex,renderer,w_width/4-w_width/16,w_height/2,w_width/8,w_height/15);
      renderTexture(exit_tex,renderer,w_width/4-w_width/24,w_height/3*2,w_width/12,w_height/15);
    }else if (state == 1){
      renderTexture(title_tex,renderer,0,0,w_width,w_height);
      renderTexture(select_tex,renderer,w_width/4-w_width/12,w_height/2+w_height/6*(menu_index-1),w_width/6,w_height/12);
      renderTexture(mainmenu_tex,renderer,w_width/4-w_width/12,w_height/3*2,w_width/6,w_height/16);
      renderTexture(easy_tex,renderer,w_width/4-w_width/20,w_height/3,w_width/10,w_height/20);
      renderTexture(insane_tex,renderer,w_width/4-w_width/16,w_height/2,w_width/8,w_height/20);
    }else if (state == 6){
      ////////////////////////////multi
      renderTexture(title_tex,renderer,0,0,w_width,w_height);
      renderTexture(select_tex,renderer,w_width/4-w_width/12,w_height/2+w_height/6*(menu_index-1),w_width/6,w_height/12);
      renderTexture(server_tex,renderer,w_width/4-w_width/18,w_height/3,w_width/9,w_height/15);
      renderTexture(client_tex,renderer,w_width/4-w_width/20,w_height/2,w_width/10,w_height/15);
      renderTexture(mainmenu_tex,renderer,w_width/4-w_width/12,w_height/3*2,w_width/6,w_height/15);
      ///////////////////multi
    }else if (state==2 || state==3){
      /***** character *****/
      renderTexture(temp,renderer,c->x-16,c->y-16,32,32);
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
	if (m_buffer->spawn_time && m_buffer->spawn_time < time){
	  renderTexture(dw,renderer,m_buffer->x-16,m_buffer->y-16,32,32);
	  if ( state == 2 ){
	    do_action_m(m_buffer);
	  }
	}
	check_remove(m_buffer);
	m_buffer=m_buffer->next;
      }
      /***** bullets *****/
      bullet* b_buffer = bullets;
      while ( b_buffer ){
	renderTexture_r(bullet_tex,renderer,b_buffer->x-w_height/80,b_buffer->y-w_height/60,w_height/40,w_height/30,b_buffer->angle);
	//renderTexture(bullet_tex,renderer,b_buffer->x-w_height/80,b_buffer->y-w_height/60,w_height/40,w_height/30);
	if ( state == 2 )
	  do_action_b(b_buffer);
	interact_b(b_buffer);
	//remove bullets if off screen
	b_buffer=  b_buffer->next;
      }
      
      if ( state == 2 ){
	/***** sprites *****/
	//cycleSprite(c->sprite,time);
	handle_input(c);
      }else if ( state == 3 ){
	/***** pause menu *****/
	renderTexture(shade,renderer,0,0,w_width,w_height);
	renderTexture(select_tex,renderer,w_width/3-w_width/8,w_height/2 + (w_height/6*(menu_index-1)),w_width/4,w_height/12);
	renderTexture(continue_tex,renderer,w_width/3-w_width/10,w_height/3,w_width/5,w_height/15);
	renderTexture(restart_tex,renderer,w_width/3-w_width/12,w_height/2,w_width/6,w_height/15);
	renderTexture(mainmenu_tex,renderer,w_width/3-w_width/10,w_height/3*2,w_width/5,w_height/15);
      }
      /***** fading images *****/
      fade* f_buf = fades;
      while ( f_buf ){
	render_fade(f_buf);
	f_buf = f_buf->next;
      }
      /***** background *****/
      renderTexture(bg_texture,renderer, 0, 0,w_width,w_height);
      /***** game panel on right *****/
      renderTexture(score_tex,renderer,w_width*13/20,w_height/6,w_width/9,w_height/16);//not sure on dimensions, need testing
      renderTexture(lives_tex,renderer,w_width*13/20,w_height/4,w_width/10,w_height/20);
      renderTexture(bombs_tex,renderer,w_width*13/20,w_height/3,w_width/9,w_height/16);
      renderTexture(grazes_tex,renderer,w_width*13/20,w_height*5/12,w_width/9,w_height/16);
      //----!! start using text for displaying score and grazes

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
