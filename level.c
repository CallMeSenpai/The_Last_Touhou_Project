#include <math.h>
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
      double percent = (double)atoi(token);
      new->x = (double)((percent * (double)w_width /100) * 5 /8); 
      //printf("x %d\n",new->x);

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //double y %
      percent = (double)atoi(token);
      new->y = (double)(percent * (double)w_height /100);
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
      new->delay = (short)atoi(token);
      //printf("delay %d\n", new->delay);

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //int load_time
      new->load_time = atoi(token);
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

      //brown
      if (new->id == 0)
	new->behavior = &circle_8;
      else if (new->id == 1)
	new->behavior = &target_shoot;
      else if (new->id == 2)
	new->behavior = &cone_down;
      else if (new->id == 3)
	new->behavior = &brown_shot;
      else if (new->id == 4)
	new->behavior = &brown_recursion;
      //konstans
      else if (new->id == 10)
	new->behavior = &tree;
      else if (new->id == 11)
	new->behavior = &get_juked;
    }/*if mobs */
    
    
  }
  puts("-----------------");
  fclose(f);
}

/* -------------------SHOOT FUNCTIONS---------------------*/

void target_shoot(double x, double y, unsigned long spawn_time){
  bullet* b =create();
  set_values_b(b,x,y);
  target(b);
  //set_angle(b,angle);
  set_speed(b,4);
}

//one of the bullets will start at the angle
//I have no idea what 60 means anymore and I will probably change it later
void circle_8(double x, double y, unsigned long spawn_time){
  int i = 0;
  for (;i<8;i++) {
    bullet* b = create();
    set_values_b(b, x, y);
    //set_angle(b, (short)((short)((360/30)*i) + angle));
    set_angle(b,(360/8)*i);
    //printf("%f\n", b->angle);
    set_speed(b,4);
  }
}

//shoot horizontally left
void left_shoot(double x, double y, unsigned long spawn_time){
  bullet* b = create();
  set_values_b(b,x,y);
  set_angle(b, 180);
  set_speed(b, 5);
}

//shoot horizontally right
void right_shoot(double x, double y, unsigned long spawn_time){
  bullet* b = create();
  set_values_b(b,x,y);
  set_angle(b, 0);
  set_speed(b, 5);
}

//seriously we need to change the sprite of bullets
//11 bullets
void cone_down(double x, double y, unsigned long spawn_time){
  int i = 0;
  for (;i<11;i++) {
    bullet* b = create();
    set_values_b(b,x,y);
    set_angle(b, 220 + (10 * i));
    set_speed(b, 5);
  }
}

/* ----------brown---------- */

void brown_shot(double x, double y, unsigned long spawn_time) {
  int i = 0;
  for (;i<15;i++) {
    bullet* b = create();
    set_values_b(b,x,y);
    set_angle(b, i * 24);
    set_speed(b, 4);
    set_da(b,0.7);
  }
  i=0;
  for (;i<15;i++) {
    bullet* b = create();
    set_values_b(b,x,y);
    set_angle(b, (i * 24)+12);
    set_speed(b, 4);
    set_da(b,0.7);
  }
}

//helper function
void cross_shot(double x, double y, unsigned long spawn_time) {
  int i = 0;
  for (;i<4;i++) {
    bullet* b = create();
    set_values_b(b, x, y);
    //set_angle(b, (short)((short)((360/30)*i) + angle));
    set_angle(b,(360/4)*i);
    //printf("%f\n", b->angle);
    set_speed(b,2);
  }
}

void brown_recursion(double x, double y, unsigned long spawn_time){
  //printf("%ld\n", time-spawn_time);
  if(time-spawn_time < 180){
    cross_shot(x,y,spawn_time);
  }
  else {
    cross_shot(x-100,y-100,spawn_time);
    cross_shot(x+100,y-100,spawn_time);
    cross_shot(x-100,y+100,spawn_time);
    cross_shot(x+100,y+100,spawn_time);

  }
}
  
/* --------- konstans ----------- */

//called every frame
void get_juked(double x, double y, unsigned long spawn_time){
  //shoot in a circle / rotate
  //angle change: determined by current time and spawn time 
  int angle_speed;
  angle_speed=7;
  //360 deg / 8 seconds = 360deg / 480 ticks.
  double heading = ((spawn_time - time)%480)*3/4;
  bullet* b = create();
  set_values_b(b, x, y);
  set_angle(b,heading);
  set_speed(b,20);
  //might want movement behavior here
  b->dv=-2;
}

//tree delay should be HIGH
void tree(double x, double y, unsigned long spawn_time){
  bullet* b = create();
  b->spawn_time = time-60;
  set_values_b(b,x,y);
  target(b);
  b->id = 10;
  b->movement = &split;
  set_speed(b,2);
  split(b,4);
}


/* --------- dw --------- */



/* -------- zman --------- */



/* -------movement -------- */

//bullets split n nodes
void split(bullet* b, int n){
  if (b->id==10 && time - b->spawn_time > 60){
    bullet* new;
    for (; n>0;n--){
      new = create();
      new->spawn_time=time;
      set_values_b(new,b->x,b->y);
      new->id = 10;
      new->movement = &split;
      set_speed(new,2);
      if (n%2==1)//if odd
	set_angle(new,b->angle - (n+1/2)*15);
      else
	set_angle(new,b->angle + (n/2)*15);
    }
    b->x=-100;//instant kill
  }
}
