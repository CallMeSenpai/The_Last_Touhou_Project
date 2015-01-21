#ifndef LEVEL_H
#define LEVEL_H



void trim(char*);
void load_dat(char*);

/* ---------------------------SHOTS--------------------------- */

//shoot directly at the player
void target_shoot(int,int,short,char,int);
//shoot a circle of 8
void circle_8(int,int,short,char,int);
//shoot horizontally left; the angle parameter can be basically anything
void left_shoot(int,int,short,char,int);
//shoot horizontally right
void right_shoot(int,int,short,char,int);
//shoot in a cone downwards
void cone_down(int,int,short,char,int);



/* ----------brown---------- */
//brown regular
void brown_shot(int,int,short,char,int);
//brown recursion
void brown_recursion(int,int,short,char,int);
//brown halo
void brown_halo(int,int,short,char,int);
//brown ray
void brown_ray(int,int,short,char,int);
/* ---------zman------------ */

/* --------konstans--------- */

/* --------dw-----------*/
void dw_segfault(int,int,short,char,int);


#endif
