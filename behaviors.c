#include "behaviors.h"
#include <stdio.h>

void b1(struct stim_param *sp) { 
  //printf(" Behavior 1 for obj [%u]!\n",(*(*sp).nobj_props).nobj_id);
  //display_neur_props((*sp).nobj,*(*sp).nobj_props);
  //display_vars_props((*sp).vars,*(*sp).nobj_props);
}
int t1(struct stim_param *sp) { 
  //printf(" Thresh 1!\n");
 //printf("[Threshold1(stim/thresh/v2)(%lf,%lf,%lf)\n",(*sp).vars[(*sp).neur_to][0], (*sp).vars[(*sp).neur_to][1],(*sp).vars[(*sp).neur_to][2]);
  if(sp->vars[sp->neur_to][0] >= sp->vars[sp->neur_to][1] ) {

    sp->vars[sp->neur_to][0]=0;//reset stim to 0
    sp->vars[sp->neur_to][2]=sp->vars[sp->neur_to][1] * sp->vars[sp->neur_to][6];//set fire strengh
    //printf("behav:fire depreciation:%lf\n",sp->vars[sp->neur_to][6]);
    return 0;
  }
  //printf("Threshold1(stim/thresh/v2)[%u] - (%lf,%lf,%lf)\n",sp->nobj_props->nobj_id,sp->vars[sp->neur_to][0],
  //sp->vars[sp->neur_to][1],sp->vars[sp->neur_to][2]);
  //printf("behav:fire depreciation:%lf\n",sp->vars[sp->neur_to][6]);
  return 2;
}
