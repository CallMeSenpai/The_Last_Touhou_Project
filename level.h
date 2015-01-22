#ifndef LEVEL_H
#define LEVEL_H



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
//shoot in a cone downwards
void cone_down(double,double,unsigned long);



/* ----------brown---------- */
//brown regular
void brown_shot(double,double,unsigned long);
//brown recursion
void brown_recursion(double,double,unsigned long);
//brown halo
void brown_halo(double,double,unsigned long);
//brown ray
void brown_ray(double,double,unsigned long);
/* ---------zman------------ */

/* --------konstans--------- */

/* --------dw-----------*/
void dw_segfault(double,double,unsigned long);


#endif
