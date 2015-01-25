#ifndef LEVEL_H
#define LEVEL_H
#include "main.h"


void trim(char*);
void load_dat(char*);

/* ---------------------------SHOTS--------------------------- */

//shoot directly at the player
void target_shoot(double,double, unsigned long);
//shoot a circle of 8
void circle_8(double,double,unsigned long);
//shoot horizontally left; the angle parameter can be basically anything
void left_shoot(double,double,unsigned long);
//shoot horizontally right
void right_shoot(double,double,unsigned long);
void down_shoot(double,double,unsigned long);
//shoot in a cone downwards
void cone_down(double,double,unsigned long);



/* ----------brown---------- *///brown regular
void brown_shot(double,double,unsigned long);
//brown recursion
void brown_recursion(double,double,unsigned long);
void x_shot(double,double,unsigned long);
void cross_shot(double,double,unsigned long);
void b_recursion(bullet*);
void b_rec_target(bullet*);
//brown ray
void brown_ray(double,double,unsigned long);
void angel_rain(double,double,unsigned long);
void rain(bullet*);
void explode(bullet*);
/* ---------zman------------ */
//z flask
//z sharknado
/* --------konstans--------- */
void get_juked(double,double,unsigned long);
void k_tree(double,double,unsigned long);
void circle(double,double, unsigned long);
/* --------dw-----------*/
void extend_shoot(double,double,unsigned long);
void dw_segfault(double,double,unsigned long);

/* ------zman------- */

/* -----movement ----- */
#endif
