#ifndef LEVEL_H
#define LEVEL_H



void trim(char*);
void load_dat(char*);

/* ---------------------------SHOTS--------------------------- */

//shoot directly at the player
void target_shoot(double,double);
//shoot a circle of 8
void circle_8(double,double);
//shoot horizontally left; the angle parameter can be basically anything
void left_shoot(double,double);
//shoot horizontally right
void right_shoot(double,double);
//shoot in a cone downwards
void cone_down(double,double);



/* ----------brown---------- */
//brown regular
void brown_shot(double,double);
//brown recursion
void brown_recursion(double,double);
//brown halo
void brown_halo(double,double);
//brown ray
void brown_ray(double,double);
/* ---------zman------------ */

/* --------konstans--------- */

/* --------dw-----------*/
void dw_segfault(double,double);


#endif
