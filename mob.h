#ifndef MOB_H
#define MOB_H

#include "include/SDL.h"

mob* summon();
//mob* load();
void set_default_values_m(mob*);
void set_values_m(mob*,int,int,double,int,short);
void do_action_m(mob*);

#endif
