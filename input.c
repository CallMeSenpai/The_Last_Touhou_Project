#include <stdio.h>
#include <stdlib.h>
#include "include/SDL.h"
#include "main.h"
#include "input.h"


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
	c->speed=4;
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
  if (state==0 || state==1 || state==3 || state==6){
    switch (e.key.keysym.sym){
    case SDLK_DOWN:
      menu_index=(menu_index+1)%menu_options;
      break;
    case SDLK_UP:
      menu_index=(menu_index-1+menu_options)%menu_options;
      break;
    }
  }
  if (state==6){
    switch (e.key.keysym.sym){
    case SDLK_RETURN:
      if ( menu_index == 2){
        state=0;
	menu_options=3;
	menu_index=0;
      }else if ( menu_index == 0 ){
	puts("server");
	server();
      }else if (menu_index == 1){
	puts("client");
	client();
      }
      break;
    case SDLK_ESCAPE:
      menu_options=3;
      menu_index=0;
      state=0;
      break;
    }
  }else if (state==0){
    switch (e.key.keysym.sym){
    case SDLK_RETURN:
      if ( menu_index == 0 )
	levels();
      else if ( menu_index == 2 )
	exit(0);

      else if ( menu_index == 1 ){//multi
	multi();
      }
      break;
    }
  }else if ( state == 1 ){
    switch (e.key.keysym.sym){
    case SDLK_RETURN:
      if ( menu_index == 0 ){ start(1); } // start on ez
      if ( menu_index == 1 ){ start(1); }//start on insane
      if ( menu_index == 2 ){ 
	state=0;
	menu_options=3;
	menu_index=0;
      }
      break;
    case SDLK_ESCAPE:
      state=0;
      menu_options=3;
      menu_index=0;
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
	clear(1);
        start(num_players);
	break;
      }else if (menu_index==2){ 
	title(); 
	//bugs, double free @ game->title
      }
    }
  }else if (state==2||state==4){
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
	c->speed=8;
      }
      break;
    case SDLK_z:
      if(c->shoot || state ==2){
	c->shoot=0;
      }else if (c->shoot || state == 4){
	//continue with dialogue?
	sentences++;
      }
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
