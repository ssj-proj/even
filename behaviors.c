#include "behaviors.h"
#include <stdio.h>

void b1(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,   unsigned int**nobj,unsigned int**cons,unsigned int**conids,double**weights,double**vars, struct nobj_meta *nobj_props/* struct lock_group *locks */) { 
  printf(" Behavior 1!\n");
  display_neur_props(nobj,*nobj_props);
  display_vars_props(vars,*nobj_props);

}
int t1(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,   unsigned int**nobj,unsigned int**cons,unsigned int**conids,double**weights,double**vars, struct nobj_meta *nobj_props/* struct lock_group *locks */) { 
  printf(" Thresh 1!\n");
  return 1;
}
