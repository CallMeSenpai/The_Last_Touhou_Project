#ifndef MOB_H
#define MOB_H

mob* summon();
void set_default_values_m(mob*);
void set_values_m(mob*,int,int,double,int,short);
void do_action_m(mob*);
void check_remove(mob*);
#endif
