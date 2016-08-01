#include "behaviors.h"
#include <stdio.h>

void b1(struct stim_param *sp) { 
  printf(" Behavior 1!\n");
  display_neur_props((*sp).nobj,*(*sp).nobj_props);
  display_vars_props((*sp).vars,*(*sp).nobj_props);

}
int t1(struct stim_param *sp) { 
  printf(" Thresh 1!\n");
  return 1;
}
