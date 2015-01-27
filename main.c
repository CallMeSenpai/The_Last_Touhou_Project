#include <unistd.h>//for macs???
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
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
#include <errno.h>
//#include "fade.h"

#define WHITE (SDL_Color){255,255,255}

int level;
unsigned long score,time,last_death;
char bombs,lives,grazes;
/*---------state: 
  0=main
  1 =selections
  2 = game
  3 = paused
  4=dialogue
  5 = game-finished
  6-multiplayer ( menu screen )
  7-server waiting for connection
  8-client looking for server
  */
char state;
SDL_Event e;
int c_height=32;
int c_width=32;
int w_width , w_height;
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
int sentences;

int socket_id;
struct sockaddr_in serv_addr;
char* data;
char host;//boolean to check if you are server or not

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
/***** dialogue before boss mode? *****/
//we check for time req's before calling the function
//i.e if (time > 600?) dialogue();
void dialogue(){
  state=4;
  sentences=0;
  if (level==1){
    puts("dialogue for level 1");
    //reminder: load pics before using them
    //TTF: lib->text->surface?->texture
    //while loop: each z/enter = sentences++;
    //__if (sentences> max_sentences)
    //____state=2
    //
    /* reminder
       -bullets can move
       -projectiles can move but not kill mobs
       -mobs can move
       -character can move but not get killed
    */
  }else if (level==2){
    //do crap
  }

}
/***** INIT GAME VARIABLES *****/
void start(int num){
  num_players = num;
  //printf("the original dimensions are %d, %d\n",w_width,w_height);
  create_fade(level1_tex,150,w_width/3,w_height/3);
  c = calloc(num,sizeof(character));
  
  set_default_values_c(c);
  if (num>1)
    set_default_values_c(c+1);
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
  switch (level){
  case 1:
    puts("sup");
    mob* m = mobs;
    while (m) {
      m->x = -100;
      m = m->next;
    }
    load_dat("2e.dat");
    time=0;
    last_death=0;
    break;
    /* we don't have 3e and 4e dat files
  case 2:
    puts("sup");
    mob* m = mobs;
    while (m) {
      m->x = -100;
      m = m->next;
    }
    load_dat("3e.dat");
    time=0;
    last_death=0;
    break;
  case 3:
    puts("sup");
    mob* m = mobs;
    while (m) {
      m->x = -100;
      m = m->next;
    }
    load_dat("4e.dat");
    time=0;
    last_death=0;
    break;
    */
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
  //puts("lol multi");
}
void server(){
  state=7;
  socket_id = socket(AF_INET,SOCK_DGRAM,0);
  puts("socket created.");
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(5000);
  bind(socket_id, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  while (1){
    char mesg[256];
    int x = sizeof(serv_addr);
    int n = recvfrom(socket_id,mesg,sizeof(mesg),0,(struct sockaddr*)&serv_addr,&x);
    mesg[n]=0;
    sendto(socket_id,"hi im client",strlen("hi im client"),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

  }

  //listen(socket_id,1);
  //puts("listening...");
  //printf("error is %s\n",strerror(errno));
  //int i = accept(socket_id,NULL,NULL);
  //printf("connected: %d\n",i);
  
  //puts("accepted.");
  //data= calloc(256,sizeof(char));

  start(2);
  host=1;
}
void client(){
  state=8;
  socket_id = socket(AF_INET,SOCK_STREAM,0);
  puts("socket created.");
  puts("Please enter the host's IPv4");
  char* ip_buf=calloc(256,1);
  fgets(ip_buf,256,stdin);
  strtok(ip_buf,"\n");
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(ip_buf);
  serv_addr.sin_port = htons(80);
  char recvline[256];
  while(1){
    sendto(socket_id,"sup",strlen("sup"),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    int n= recvfrom(socket_id,recvline,256,0,0,0);
    recvline[n] = 0;

  }
  //connect(socket_id,(struct sockaddr*)&serv_addr, sizeof(serv_addr));
  //puts("eyy done connecting");
  //data= calloc(256,sizeof(char));
  start(2);
  host=0;
}
void levels(){
  menu_options=3;
  state=1;
  menu_index=0;
}
int main(){
  

  /***** INIT SDL AND WINDOW *****/
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  create_window();
  TTF_Init();
  font = TTF_OpenFont("whitrabt.ttf", 20);
  /***** INIT ALL IMAGES *****/
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
  //add images to here
  //crop iamges, bullets, sprites by barak
  //
  level1_tex = IMG_LoadTexture(renderer,"images/level1.png");
  level2_tex = IMG_LoadTexture(renderer,"images/level2.png");
  level3_tex = IMG_LoadTexture(renderer,"images/level3.png");
  level4_tex = IMG_LoadTexture(renderer,"images/level4.png");

  title();//load title screen
  host=2;
  while(host){//while server
    //puts("sup");
    if (state!=3 && state !=4)
      time++;
    else if (state == 4) {
      //display images
      //display text
      
    }
    if (state==2 && time > 1000){
      next();//temp solution, 1000 not determined yet
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
    if (state==0||state==1||state==6||state==7||state==8){
      renderTexture(title_tex,renderer,0,0,w_width,w_height);
      if (state!=7 && state!=8)
	renderTexture(select_tex,renderer,w_width/4-w_width/12,w_height/2+w_height/6*(menu_index-1),w_width/6,w_height/12);
    }
    if (state==0){
      renderTexture(start_tex,renderer,w_width/4-w_width/20,w_height/3,w_width/10,w_height/15);
      renderTexture(multi_tex,renderer,w_width/4-w_width/16,w_height/2,w_width/8,w_height/15);
      renderTexture(exit_tex,renderer,w_width/4-w_width/24,w_height/3*2,w_width/12,w_height/15);
    }else if (state == 1){
      renderTexture(mainmenu_tex,renderer,w_width/4-w_width/12,w_height/3*2,w_width/6,w_height/16);
      renderTexture(easy_tex,renderer,w_width/4-w_width/20,w_height/3,w_width/10,w_height/20);
      renderTexture(insane_tex,renderer,w_width/4-w_width/16,w_height/2,w_width/8,w_height/20);
    }else if (state == 6){
      renderTexture(server_tex,renderer,w_width/4-w_width/18,w_height/3,w_width/9,w_height/15);
      renderTexture(client_tex,renderer,w_width/4-w_width/20,w_height/2,w_width/10,w_height/15);
      renderTexture(mainmenu_tex,renderer,w_width/4-w_width/12,w_height/3*2,w_width/6,w_height/15);
    }else if (state==2 || state==3 || state ==4){
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
	if ( state == 2 || state == 4)
	  do_action_b(b_buffer);
	interact_b(b_buffer);
	//remove bullets if off screen
	b_buffer=  b_buffer->next;
      }
      
      if ( state == 2 || state == 4){
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
  while(host-1){
    /*in while(host)
      
      loop through each object: send client item
      client: receives and paints
      ** bullets must check for both chars now?

      client: moves *(c+1)
      --->make new input functions only for movement/shoot
      send server all signals...?
      0,1,2,3 for directional and 4 to shoot?

      server receives. change x,y as needed. shoot as needed.
      


      //el fin img?

    */
    
    //break;
  }

 end:
  SDL_DestroyTexture(bg_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return 0; 
}
