/*
  this env is just to massage outgoing data to the openai gym
*/
#include "../../backend/env_api.h"
#include "env_oai.h"
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
static int num_of_istream=4;
static double *ostream=NULL;
static double *util=NULL;

static int num_of_ipoints=9;//width of 2nd dimension array
static char init=0;

int init_env_oai(struct env_control *ec, struct env_dat *dat, int env) {

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
    //env_api_dat->util=0;//set util to 0 for all objects
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

//returns which stream ID has been stimmed, the strengh of the stim doesnt matter
int get_oai_work(){
  struct job* work=get_next_output(env_id,env_api_control);
  if(work) { //if theres any out work
    return work->sid;

  }
  printf("No work\n")
  return -1;
}
void set_oai_input(double *ins, int size){
 /*
  TODO - send input to net

 */

}
void *main_loop_oai(void *state){
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
  int i = 0;

  /*
    empty main loop as this env does no processing
  */
  while((*main_state)==0) {
    usleep(100);//--should be same as stimpool sleep time
  }
}
