#include "../env_api.h"
#include "random_env.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


static struct env_dat *env_api_dat;//all other vars
static struct env_control *env_api_control;//for work queue manipulation
static int env_id; 

int num_of_queue;
/*
   a test system - reward when objs output when input is close to 100-100
*/
//data points for each obj [obj][data_point]
static double **env_obj_track;
static double *istream;
static int num_of_istream=3;
static double *ostream;
int num_of_points=2;//width of 2nd dimension array
int *sid_map;//stream id map : 1d arry value at [sid]=obj_id
char init=0;

void init_env0(struct env_control *ec, struct env_dat *dat, int env) {
  env_api_dat=dat;
  env_api_control=ec;
  env_id  = env;
  int i,j;
  
  if(env_api_dat->num_of_objs<=0) {
    fprintf(stderr,"Can't init env with number of objects set to 0\n");
  } 
  env_api_dat->util = malloc(sizeof(*env_api_dat->util) * env_api_dat->num_of_objs);
  
  env_obj_track=malloc(sizeof(*env_obj_track) * (env_api_dat->num_of_objs) );
  sid_map=malloc(sizeof(*sid_map) * (env_api_dat->num_of_objs) );
  
  for(i=0;i<env_api_dat->num_of_objs;++i){

    env_obj_track[i]=malloc(sizeof( *(env_obj_track[0]) ) * num_of_points);
    //printf(":::::::::Size of env %d: %u\n",i,sizeof(env_obj_track[i]));
    sid_map[i]=i;

    for(j=0;j<num_of_points;++j) {
      env_obj_track[i][j]=2;
    }


  }
 // ec->num_of_istream=num_of_points*;
  istream = malloc(sizeof(double) * 3);
  istream[0]=22;
  istream[1]=33;
  istream[2]=44;
  hook_env(env_id,istream,3); 
  init=1;

}

void *main_loop(void *state){ 
  struct job* work;
  int i,j,num_loop;
  srand(time(NULL));
 
  while(1) {
    
    work=get_next_output(0,env_api_control);//get next work
    if(!work) { 
      //printf("NULL WORK\n");
    }
    //while there is work to be donec
    while(work) {//loop through work queue and update all objects 
      //printf("stream/work: %d/%lf\n",(*work).sid,(*work).dat);
      istream[work->sid]+=work->dat;
      work=get_next_output(i,env_api_control);
    } 

    //printf("random_env:end/no work\n");
    /* 
	Finished work queue
	update each object for natural time passing 
    */
    for(j=0; j < (env_api_dat->num_of_objs) ;++j) {  //loop each obj

      for(i=0; i<num_of_points; ++i) { //loop each data point for each object
        env_obj_track[j][i]--;
      }

    }
    num_loop++;
    i=0;

   /* printf("iStreams:");
    for(i=0;i<num_of_istream;++i) {
      printf("[%lf]",istream[i]);
    }
     printf("\n\n");*/
    usleep(100);//--should be same as stimpool sleep time 
  }
}


