/*
  Left/Right environment - 2 inputs and 2 outputs, when input is set, the corresponding output
    should be stimed

*/

#include "../../backend/env_api.h"
#include "lr_env.h"
#include "../../backend/stimpool.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


static struct env_dat *env_api_dat=NULL;//all other vars
static struct env_control *env_api_control=NULL;//for work queue manipulation
static int env_id; 

static double *istream=NULL;
static int num_of_istream=2;
static double *ostream=NULL;
static double *util=NULL;

static int num_of_ipoints=9;//width of 2nd dimension array
static char init=0;

int init_env_lr(struct env_control *ec, struct env_dat *dat, int env) {

  if(!ec || !dat){
    fprintf(stderr,"ERR: init_env_lr : null env_control or env_dat\n");
    return 0;
  }
  env_api_dat=dat;
  env_api_control=ec;
  env_id  = env;
  int i,j;

  if(dat->num_of_objs<=0) {
    fprintf(stderr,"Can't init env with number of objects set to 0\n");
    return 1;
  }
  if(ec->num_of_istream<=0) {
    fprintf(stderr,"Can't init env with number of istreams set to 0\n");
    return 2;
  }
  //create array for util measurements for each object
  env_api_dat->util = malloc(sizeof(*env_api_dat->util) * env_api_dat->num_of_objs);
  util=env_api_dat->util;

  //create array for sid mappings
 //?? sid_map = malloc(sizeof(*sid_map)*ec->num_of_istream);

  for(i=0; i < env_api_dat->num_of_objs; ++i) {
    env_api_dat->util=0;//set util to 0 for all objects
    util[i]=0;
  }

  istream = malloc(sizeof(double) * num_of_ipoints);
  //set initial values for input
  istream[0]=100;
  istream[1]=0;
  if(hook_env(env_id,istream,num_of_ipoints)!=0){
    fprintf(stderr,"Error hooking environment init_env_lr\n");
    exit(-1);
  } 
  init=1;
  return 0;
}

void *main_loop_lr(void *state){ 
   if(!init){
    fprintf(stderr,"ERR : init_env_lr : called main environment loop before init.\n");
    exit(-1);
  } 
  //Variables for env api
  struct job* work;
  int *main_state = (int *)state;
  double progress=0;
  /*
    Variables for internal environment workings
  */
  srand(time(NULL));//seed random 
  int cur_dir=4;//just set
  int dir_1=-9999;//last direction
  int dir_2=-9999;//direction 2 times ago
  int dir_3=-9999;//direction 3 times ago
  int i = 0;
  while((*main_state)==0) {
    work=get_next_output(env_id,env_api_control);//get next work
    //Perform internal actions
    //determine objs util
    int sid_activated = work->sid;
    double util_granted=0;
    if(work->sid==cur_dir) { util_granted = 100; }
    else if(work->sid==dir_1) { util_granted = 50; }
    else if(work->sid==dir_2) { util_granted = 25; }  
    else if(work->sid==dir_3) { util_granted = 10; }
    else { util_granted = -10; }
    //set util
    util[work->nobj_id]+=util_granted;
    
    //set next input at time step
    if( ((  get_nobj_progress(work->nobj_id)-progress )/get_nobj_progress_step(work->nobj_id)) > 10 ) {
     
      progress=get_nobj_progress(work->nobj_id);
      dir_3=dir_2;
      dir_2=dir_1;
      dir_1=cur_dir;
      cur_dir=rand()%9;//todo change 9 to equal number of ostream
      for(i = 0; i < 9; ++i){
        istream[i]=0;
      }
      istream[cur_dir]=100;
      //sleep
    }
    usleep(100);//--should be same as stimpool sleep time 
  }
}


