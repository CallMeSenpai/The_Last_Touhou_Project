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
#define center_x (double)((double)w_width * 5 /16)
#define center_y (double)((double)w_height/2)
#define PI 3.14159265

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

      //IMPORTANT: Remember to take out the print in main
      //high
      if (new->id == 0)
	new->behavior = &circle_8;
      //mid
      else if (new->id == 1)
	new->behavior = &target_shoot;
      //high
      else if (new->id == 2)
	new->behavior = &cone_down;
      //brown
      //low
      else if (new->id == 3)
	new->behavior = &brown_shot;
      //high
      else if (new->id == 4)
	new->behavior = &brown_recursion;
      //mid
      else if (new->id == 5)
	new->behavior = &brown_ray;
      //konstans
      //very low
      else if (new->id == 11)
	new->behavior = &k_get_juked;
      //low
      else if (new->id == 10)
	new->behavior = &k_tree;
      //low
      else if (new->id == 12)
	new->behavior = &circle;
      //low
      else if (new->id == 20)
	new->behavior = &dw_extend_shoot;
      //very low
      else if (new->id == 21)
	new->behavior = &dw_segfault;
      //low
      else if (new->id == 31)
	new->behavior = &z_shot;
      //medium
      else if (new->id == 32) 
	new->behavior = &z_flask;
      //med-low
      else if (new->id == 33) 
	new->behavior = &z_sharknado;
      
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
  set_speed(b,6);
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
void down_shoot(double x, double y, unsigned long spawn_time){
  bullet* b = create();
  set_values_b(b,x,y);
  set_angle(b,270);
  set_speed(b,4);
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

//helper function for brown_recursion


//bullet movement for brown_recursion
void b_recursion(bullet* b) {
  //b->da = 1;
  unsigned long t = time - b->spawn_time; //current time
  if (t % 30 == 0 && t > 0) {
    if ((int)b->angle % 90 == 0) {
      //copy of x_shot without recursion
      int i = 0;
      for (;i<4;i++) {
	bullet* bb = create();
	set_values_b(bb, b->x, b->y);
	set_angle(bb,(360/4)*i + 45);
	set_speed(bb,4);
	set_dv(b,-0.2);
	bb->movement = &b_rec_target;
      }
      //remove self
      b->x = -1;
    }
    else {
      //copy of cross_shot without recursion
      int i = 0;
      for (;i<4;i++) {
	bullet* bb = create();
	set_values_b(bb, b->x, b->y);
	set_angle(bb,(360/4)*i);
	set_speed(bb,4);
	set_dv(b,-0.2);
	bb->movement = &b_rec_target;
      }
      b->x = -1;
    }
  } 
}

void b_rec_target(bullet* b) {
  if (time - b->spawn_time == 60) {
    target(b);
  }
}

//should be slightly higher delays
void x_shot(double x, double y, unsigned long spawn_time){
  int i = 0;
  for (;i<4;i++) {
    bullet* b = create();
    set_values_b(b, x, y);
    //set_angle(b, (short)((short)((360/30)*i) + angle));
    set_angle(b,(360/4)*i + 45);
    //printf("%f\n", b->angle);
    set_speed(b,4);
    set_dv(b,-0.2);

    b->movement = &b_recursion;
  }
}
void cross_shot(double x, double y, unsigned long spawn_time) {
  int i = 0;
  for (;i<4;i++) {
    bullet* b = create();
    set_values_b(b, x, y);
    //set_angle(b, (short)((short)((360/30)*i) + angle));
    set_angle(b,(360/4)*i);
    //printf("%f\n", b->angle);
    //set_speed(b,2);
    set_speed(b,4);
    set_dv(b,-0.2);

    b->movement = &b_recursion;
  }
}


void brown_recursion(double x, double y, unsigned long spawn_time){
  unsigned long t = time - spawn_time;
  if (t % 60 == 0) {
    x_shot(x,y,spawn_time);
  }
  else {
    cross_shot(x,y,spawn_time);
  }
}


//helper functions for brown_halo
void angel_rain(double x, double y, unsigned long spawn_time){
  bullet* b = create();
  set_values_b(b, x, y);
  //set_angle(b, (short)((short)((360/30)*i) + angle));
  set_angle(b,90);
  //printf("%f\n", b->angle);
  set_speed(b,10);
  set_dv(b,-1);
  b->movement = &rain;
}

void rain(bullet* b) {
  b->speed += b->dv;
  if (b->speed == 0) {
    int i = 0;
    for (;i<2;i++) {
      bullet* bb = create();
      set_values_b(bb,b->x,b->y);
      set_angle(bb, 260 + (20 * i));
      set_speed(bb, 12);
      set_dv(bb, -0.5);
      bb->movement = &explode;
    }
    b->x = -100;
  }
}
void explode(bullet* b) {
  b->speed += b->dv;
  if (b->speed == 0){
    int i = 0;
    for (;i<3;i++) {
      bullet* bb = create();
      set_values_b(bb,b->x,b->y);
      set_angle(bb, 250 + (20 * i));
      set_speed(bb, 2.5);
    }
    b->x = -100;
  }
}


//I NEED A RAY
void brown_ray(double x, double y, unsigned long spawn_time){
  unsigned long t = time - spawn_time;
  if (t == 30) {
    mob* new = summon();
    //I need two angels to get in here    
    new->id =0;
    new->hp = 200;
    new->x = x - 100; 
    new->y = y;
    new->speed = 0;
    new->dv = 0;
    new->angle = 270;
    new->spawn_time = spawn_time + 29;
    new->delay =34;
    new->load_time = 59;
    new->set = 5;
    //angel
    new->behavior = &angel_rain;
    
    new = summon();
    new->id =0;
    new->hp = 200;
    new->x = x + 100; 
    new->y = y;
    new->speed = 0;
    new->dv = 0;
    new->angle = 270;
    new->spawn_time = spawn_time + 29;
    new->delay = 34;
    new->load_time = 59;
    new->set = 5;
    //angel
    new->behavior = &angel_rain;

  }
}
/* --------- konstans ----------- */

//called every frame - id11
//very low delay
void k_get_juked(double x, double y, unsigned long spawn_time){
  //shoot in a circle / rotate
  //angle change: determined by current time and spawn time 
  int angle_speed;
  angle_speed=7;
  //360 deg / 8 seconds = 360deg / 480 ticks.
  double heading = ((spawn_time - time)%480)*3/4;
  bullet* b;
  int index=0;
  for(;index<4;index++){
    b = create();
    set_values_b(b, x, y);
    target(b);
    b->angle = b->angle + heading + index*90;
    //set_angle(b,heading + index*90);
    set_speed(b,20);
    b->movement = &movement11;

    b = create();
    set_values_b(b, x, y);
    target(b);
    b->angle = b->angle + heading + index*90;
    //set_angle(b,360 - (heading + index*90));
    set_speed(b,20);
    b->movement = &movement11;
  }
}

//tree delay should be HIGH - id 10
void k_tree(double x, double y, unsigned long spawn_time){
  bullet* b = create();
  b->spawn_time = time-60;
  set_values_b(b,x,y);
  target(b);
  b->id = 10;
  b->movement = &split;
  set_speed(b,2);
  split(b);
}
// id 12
void circle(double x, double y, unsigned long spawn_time){
  int i=-1;
  bullet* b;
  double heading = ((spawn_time - time)%480)*3/4;
  while(i++<8){
    b = create();
    set_values_b(b,x,y);
    set_angle(b,heading + 45*i);
    set_speed(b,4);
  }
  
}

/* --------- dw --------- */

//mob id 20, delay of 120 
void dw_extend_shoot(double x, double y, unsigned long spawn_time){
  bullet* b;
  int i = -1;
  for (;i<2;i++){
    int j = 0;
    for (; j<8;j++){
      b=create();
      set_values_b(b,x,y);
      target(b);
      set_angle(b,b->angle + i*15);
      set_speed(b,2);
      b->dv=0.005*j;
      b->id=20;
      b->movement=&movement20;
    }
  }
}

//very low delay
void dw_segfault(double x, double y, unsigned long spawn_time) {
  int r = rand() % 15;
  double x_cor = (double)w_width*5/8 * r / 15;
  r = rand() % 12;
  double y_cor = ((double)center_y-300) * r / 12;
  bullet* b = create();
  set_values_b(b, x_cor, y_cor);
  set_angle(b,270);
  set_speed(b, 8);
}

/* -------- zman --------- */
//low delay
void z_shot(double x, double y, unsigned long spawn_time) {
  int i = 0;
  int phase = rand() % 30;
  for(;i<12;i++) {
    bullet* b = create();
    set_values_b(b,x,y);
    set_angle(b,(30*i + phase) % 360); 
    set_speed(b, 3.5);
    printf("%f\n",b->angle);
  }
}

void span(bullet* b) {
  unsigned long t = time - b->spawn_time;
  if (t % 60 == 0 && t > 0) {
    b->speed = -1 * b->speed;
  }
  
}

//high delay
void z_flask(double x, double y, unsigned long spawn_time) {
  target_shoot(x,y,spawn_time);
  if (time - spawn_time == 60) {
    int i = 1;
    for (;i<16;i++) {
      bullet* b = create();
      double x_cor = ((double)w_width*5/8)/15 * i;
      set_values_b(b,x_cor,0);
      //converge at center
      double dx = fabs(b->x - center_x);
      double dy = fabs(b->y - center_y);
      double ref_angle = atan((double)dy/(double)dx)/PI*180.0;
      if (center_x < b->x)
	if (center_y > b->y)
	  ref_angle+=180;
	else 
	  ref_angle=180-ref_angle;
      else
	if (center_y > b->y)
	  ref_angle = 360-ref_angle;
      set_angle(b,ref_angle);

      //printf("%f\n",b->angle);
      set_speed(b, 3);
      b->movement = &span;
      //second
      b = create();
      set_values_b(b,x_cor,w_height);
      //converge at center
      dx = fabs(b->x - center_x);
      dy = fabs(b->y - center_y);
      ref_angle = atan((double)dy/(double)dx)/PI*180.0;
      if (center_x < b->x)
	if (center_y > b->y)
	  ref_angle+=180;
	else 
	  ref_angle=180-ref_angle;
      else
	if (center_y > b->y)
	  ref_angle = 360-ref_angle;
      set_angle(b,ref_angle);

      //printf("%f\n",b->angle);
      set_speed(b, 3);
      b->movement = &span;
      //third
      b = create();
      double y_cor = ((double)w_height)/15 * i;
      set_values_b(b,0,y_cor);
      //converge at center
      dx = fabs(b->x - center_x);
      dy = fabs(b->y - center_y);
      ref_angle = atan((double)dy/(double)dx)/PI*180.0;
      if (center_x < b->x)
	if (center_y > b->y)
	  ref_angle+=180;
	else 
	  ref_angle=180-ref_angle;
      else
	if (center_y > b->y)
	  ref_angle = 360-ref_angle;
      set_angle(b,ref_angle);

      //printf("%f\n",b->angle);
      set_speed(b, 3);
      b->movement = &span;
      //fourth
      b = create();
      set_values_b(b,(double)w_width*5/8,y_cor);
      //converge at center
      dx = fabs(b->x - center_x);
      dy = fabs(b->y - center_y);
      ref_angle = atan((double)dy/(double)dx)/PI*180.0;
      if (center_x < b->x)
	if (center_y > b->y)
	  ref_angle+=180;
	else 
	  ref_angle=180-ref_angle;
      else
	if (center_y > b->y)
	  ref_angle = 360-ref_angle;
      set_angle(b,ref_angle);

      //printf("%f\n",b->angle);
      set_speed(b, 3);
      b->movement = &span;
    }
  }
}

void shark(bullet* b) {
  //dv_until(b, -7.2);
  //b->speed += b->dv;
  unsigned long t = time-b->spawn_time;
  if (t == 60) {
    set_speed(b,6);
    set_angle(b, 0);
    set_da(b,5);
  }
  if (t > 240 && t < 250) {
    set_speed(b,4);
    set_da(b, 0);
    target(b);
  }
}

void z_sharknado(double x, double y, unsigned long spawn_time) {
  bullet* b = create();
  set_values_b(b, x, y);
  //set_angle(b, (short)((short)((360/30)*i) + angle));
  //set_angle(b,270);
  target(b);
  //printf("%f\n", b->angle);
  set_speed(b,3);
  //set_dv(b,-0.3);
  //set_da(b,2.5);
  b->movement = &shark;
}
/* -------movement -------- */

//bullets split n nodes
//very low delay
void movement11(bullet* b){
  if (time - b->spawn_time > 3)
    if (time - b->spawn_time < 5){
      b->dv=0;
      b->speed = 0;
    }else if (time - b->spawn_time > 30)
      b->speed = 2;
}
void movement20(bullet* b){
  if (time - b->spawn_time < 70)
    b->speed += b->dv;
  else
    b->speed=2;
}
void split(bullet* b){
  if (b->id==10 && time - b->spawn_time > 60){
    bullet* new;
    int n=2;//went from general to static lol
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
