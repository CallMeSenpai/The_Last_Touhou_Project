#ifndef LEVEL_H
#define LEVEL_H



void trim(char*);
void load_dat(char*);
//shoot directly at the player
void target_shoot(int,int,short,char,int);
//shoot a circle of 8
void circle_8(int,int,short,char,int);
//shoot horizontally; the angle parameter can be basically anything
void horizontal_shoot(int,int,short,char,int);
//shoot in a cone downwards
void cone_270(int,int,short,char,int);
/* ----------brown---------- */
//brown regular
void brown_shot(int,int,short,char,int);
//brown recursion
void brown_rec(int,int,short,char,int);
//brown halo
void brown_halo(int,int,short,char,int);
//brown ray
void brown_ray(int,int,short,char,int);
/* ---------zman------------ */

/* --------konstans--------- */

/* --------dw-----------*/
void segfault(int,int,short,char,int);


#endif
