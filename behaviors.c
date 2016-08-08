#include "behaviors.h"
#include <stdio.h>

void b1(struct stim_param *sp) { 
  printf(" Behavior 1 for obj [%u]!\n",(*(*sp).nobj_props).nobj_id);
  //display_neur_props((*sp).nobj,*(*sp).nobj_props);
  //display_vars_props((*sp).vars,*(*sp).nobj_props);

}
int t1(struct stim_param *sp) { 
  printf(" Thresh 1!\n");
  if((*sp).vars[(*sp).neur_to][0] >= (*sp).vars[(*sp).neur_to][0] ) {
    (*sp).vars[(*sp).neur_to][0]=0;
    return 0;
  }
  return 1;
}
