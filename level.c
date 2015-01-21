#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "level.h"
#include "main.h"
#include "character.h"
#include "projectile.h"
#include "sprite.h"
#include "mob.h"
#include "bullet.h"
#include "include/SDL.h"
void trim(char* line) { //trims the trailing newline
  int end = strlen(line) -1;
  if (line[end] == '\n')
    line[end] = '\0';
}
//typedef int (*behavior)(int,int,short,char); 
//behavior test_shoot = &shoot;
void load_dat(char* filename){
  FILE*f = fopen(filename, "r");
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  
  while ((read = getline(&line, &len, f)) != -1) {
    trim(line);
    //for mobs
    if (!strcmp(line,"mob")) {
      mob* new = summon();
      char* token;
      
      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //number for id, give function
      new->id = atoi(token);//pointer to this guy if id==x
      //printf("id: %d\n",new->id);
      
      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //int hp
      new->hp = atoi(token);
      //printf("hp %d\n", new->hp);
      
      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //double x %
      int percent = atoi(token);
      new->x = (percent * w_width /100) * 5 /8; 
      //printf("x %d\n",new->x);

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //double y %
      percent = atoi(token);
      new->y = percent * w_height /100;
      //printf("y %d\n",new->y);

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //double speed
      new->speed = (double)atoi(token);
      //printf("speed %f\n",new->speed);	
      
      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //double dv
      new->dv = (double)atoi(token) / 100;
      //printf("dv %f\n", new->dv);

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //short angle
      new->angle = (short)atoi(token);
      //printf("angle %u\n", new->angle);
	
      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //unsigned long spawn_time
      new->spawn_time = (unsigned long)atoi(token);
      //printf("spawn_time %lu\n", new->spawn_time);

      //the delay should be in the behavior
      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //char delay
      new->delay = (char)atoi(token);
      //printf("delay %d\n", new->delay);
      
      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //int set
      new->set = atoi(token);
      //printf("set %d\n", new->set);
      
      //other than next and prev, new mob is basically created here
      //load_dat(1) is tested in main() for numbers only
      //printf("address of new mob %p\n", (void*)&new);
      
      //new->behavior = &shoot;

      //if id!!
      if (new->id == 0)
	new->behavior = &circle_8_60;
      else if (new->id == 1)
	new->behavior = &target_shoot;
    }/*if mobs */
    
    
  }
  puts("-----------------");
  fclose(f);
}

/* -------------------SHOOT FUNCTIONS---------------------*/

void target_shoot(int x, int y, short angle, char speed, int delay){
  bullet* b =create();
  set_values_b(b,x,y);
  target(b);
  //set_angle(b,angle);
  set_speed(b,speed);
  
}

//one of the bullets will start at the angle
//I have no idea what 60 means anymore and I will probably change it later
void circle_8_60(int x, int y, short angle, char speed, int delay){
  int i = 0;
  for (;i<8;i++) {
    bullet* b = create();
    set_values_b(b, x, y);
    set_angle(b, (short)((short)((360/8)*i) + angle));
    set_speed(b,speed);
  }
}
