#include "nobj.h"
#ifndef BEHAVIORS_H
#define BEHAVIORS_H

void b1(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,   unsigned int**nobj,unsigned int**cons,unsigned int**conids,double**weights,double**vars, struct nobj_meta *nobj_props);
int t1(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,   unsigned int**nobj,unsigned int**cons,unsigned int**conids,double**weights,double**vars, struct nobj_meta *nobj_props);



#endif

