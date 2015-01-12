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

int shoot(int x, int y, short angle, char speed) {
//to be implemented
  bullet* b =create();
  set_values_b(b,x,y,angle,speed);
  return 0;
}

typedef int (*behavior)(int,int,short,char); //the return of an int is now the function pointer called "behavior" with 0 parameters

behavior test_shoot = &shoot; //test_shoot is now shoot

void load_dat(char* filename){
  FILE*f = fopen(filename, "r");
  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, f)) != -1) {
    trim(line);
    //for mobs
    if (!strcmp(line,"mob")) {
      mob* new = load();
      //printf("created new mob\n");
      char* token;

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //number for id
      //skip this for now

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //int hp
      new->hp = atoi(token);
      printf("hp %d\n", new->hp);

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //double x %
      double percent = (double)((double)atoi(token) / (double)100.0);
      new->x = (double)(percent * (double)w_width); //isn't this really bad? This is the actual window width but not the game width
      //also what's bad: atoi makes a number an int. So 3.5 -> 3. (double) 3 is still 3, not 3.5.
      printf("x %f\n",new->x);

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //double y %
      percent = (double)((double)atoi(token) / (double)100.0);
      new->y = (double)(percent * (double)w_height);
      printf("y %f\n",new->y);

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //double speed
      new->speed = (double)atoi(token);
      printf("speed %f\n",new->speed);	

      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //short angle
      new->angle = (short)atoi(token);
      printf("angle %u\n", new->angle);
	
      read = getline(&line, &len, f);
      token = strsep(&line,"=");
      token = strsep(&line,"="); //unsigned long spawn_time
      new->spawn_time = (unsigned long)atoi(token);
      printf("spawn_time %lu\n", new->spawn_time);
      //other than next and prev, new mob is basically created here
      //load_dat(1) is tested in main() for numbers only
      printf("address of new mob %p\n", (void*)&new);
    }
    
  }
  
  fclose(f);
}
