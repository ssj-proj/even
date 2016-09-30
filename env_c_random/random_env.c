#include "../env_api.h"
#include "random_env.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


static struct env_dat *env_api_dat;
static struct env_control *env_api_control;
int num_of_queue;
/*
   a test system - reward when objs output when input is close to 100-100
*/
//data points for each obj [obj][data_point]
double **env_obj_track;
int num_of_points=2;//width of 2nd dimension array
int *sid_map;//1d arry value at [sid]=obj_id

void init_env(int num_objs, struct env_control *ec, struct env_dat *dat) {
  env_api_dat=dat;
  env_api_control=ec;
  env_api_dat->num_of_objs=num_objs;

  env_api_dat->util = malloc(sizeof(*env_api_dat->util) * num_objs);
  
  env_obj_track=malloc(sizeof(*env_obj_track) * num_objs);
  sid_map=malloc(sizeof(*sid_map)*num_objs);
  int i,j;

  for(i=0;i<num_objs;++i){
    env_obj_track[i]=malloc(sizeof(*env_obj_track[i])*num_of_points);
    sid_map[i]=i;
    for(j=0;j<num_of_points;++j) {
      env_obj_track[i][j]=0;
    }
  }
 // ec->num_of_istream=num_of_points*;

}

void *main_loop(void *state){ 
  struct job* work;
  int i,j,r;
  srand(time(NULL));
    
  while(1) {
    /* loop through work queue and update all objects */
    for(i=0;i<num_of_queue;++i) {
      work=get_next_output(i,env_api_control);
      if(work){
        /* to do - perform tasks based on data in work struct */
        printf("stream/work: %d/%lf\n",(*work).sid,(*work).dat);
      }
    } 
    /* update each object for natural time passing */
    for(j=0;j<env_api_dat->num_of_objs;++j) //loop each obj
      for(i=0;i<num_of_points;++i) { //loop each data point for each object
        env_obj_track[j][i]+=rand()%5;
        if(env_obj_track[j][i]>200)
           env_obj_track[j][i]-=200;
      }
    
    usleep(3); 
  }

}
