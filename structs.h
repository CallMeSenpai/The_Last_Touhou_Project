#include <stdio.h>
#include <stdlib.h>
extern int c_height;
extern int c_width;//character dimensions
extern int w_height;
extern int w_width;//window dimensions

typedef struct character{
  char lives,bombs;
  int x,y;
  double speed;
  

  //booleans
  char left,right,up,down;
  char shoot,focus,bomb;
} character; 
typedef struct projectile{
  int x,y;
  double speed;  
  //boolean: is this bullet a homing/targetting projectile?
  char homing;
} projectile;

typedef struct mob{
  char hp;//char might be too low
  int x,y;
  double speed;
} mob;
