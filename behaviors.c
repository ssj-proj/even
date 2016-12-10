#include "behaviors.h"
#include "stimpool.h"
#include <stdio.h>

void empty_behavior(struct stim_param *sp) { //0
  //printf(" Behavior 1 for obj [%u]!\n",(*(*sp).nobj_props).nobj_id);
  //display_neur_props((*sp).nobj,*(*sp).nobj_props);
  //display_vars_props((*sp).vars,*(*sp).nobj_props);
}
//does not care about progress
int thresh_hold(struct stim_param *sp) { //0
  //printf(" Thresh 1!\n");
 printf("[Threshold1(stim/thresh/v2)(%lf,%lf,%lf)\n",(*sp).vars[(*sp).neur_to][0], (*sp).vars[(*sp).neur_to][1],(*sp).vars[(*sp).neur_to][2]);
  if(sp->vars[sp->neur_to][0] >= sp->vars[sp->neur_to][1] ) {
    sp->vars[sp->neur_to][0]=0;//reset stim to 0
    sp->vars[sp->neur_to][2]=sp->vars[sp->neur_to][1] * sp->vars[sp->neur_to][6];//set fire strengh
    //printf("behav:fire depreciation:%lf\n",sp->vars[sp->neur_to][6]);
    return 0;
  }
  //printf("Threshold1(stim/thresh/v2)[%u] - (%lf,%lf,%lf)\n",sp->nobj_props->nobj_id,sp->vars[sp->neur_to][0],
  //sp->vars[sp->neur_to][1],sp->vars[sp->neur_to][2]);
  //printf("behav:fire depreciation:%lf\n",sp->vars[sp->neur_to][6]);
  return 1;
}
//these neurs require var7-8 - should have a cooldown after fire to limit fire frequency - TBI
/*
	LOH 12-10-16
*/
int regulated_thresh(struct stim_param *sp) { //1
  //printf(" Thresh 1!\n");
 printf("[Threshold1(stim/thresh/v2)(%lf,%lf,%lf)\n",(*sp).vars[(*sp).neur_to][0], (*sp).vars[(*sp).neur_to][1],(*sp).vars[(*sp).neur_to][2]);
  if(sp->vars[sp->neur_to][0] >= sp->vars[sp->neur_to][1] ) {
    sp->vars[sp->neur_to][0]=progress;//progress var grabbed from stimpool
    sp->vars[sp->neur_to][0]=0;//reset stim to 0
    sp->vars[sp->neur_to][2]=sp->vars[sp->neur_to][1] * sp->vars[sp->neur_to][6];//set fire strengh
    //printf("behav:fire depreciation:%lf\n",sp->vars[sp->neur_to][6]);
    return 0;
  }
  //printf("Threshold1(stim/thresh/v2)[%u] - (%lf,%lf,%lf)\n",sp->nobj_props->nobj_id,sp->vars[sp->neur_to][0],
  //sp->vars[sp->neur_to][1],sp->vars[sp->neur_to][2]);
  //printf("behav:fire depreciation:%lf\n",sp->vars[sp->neur_to][6]);
  return 1;
}
