#include "env_api.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>

//1d array of stream struct. [env_id]
static struct stream *in;
static int num_of_envs;

  //array of struct, element per env
static struct env_control *env_control_array;// = malloc(sizeof(struct env_control));
  //aray of struct, element per env
static struct env_dat *env_data_array;//= malloc(sizeof(struct env_control));

static char verbose = 0;//1 = print verbose

static char has_init =0;
static char has_init_env=0;
static char has_hooked=0;//set to 1 when env has hooked here



void vp(FILE *f,char * s){
  if(verbose)
    fprintf(f,s);

}
/*
	Creates an env control object used for sharing control data back and 
	forth between the env and main func thread. the returned object
	is sent to the env when initiliazing the env. 
*/
int init_env(int noc, int qm,  struct env_control *client_work, struct env_dat *env_data) {
  client_work->num_of_clients=noc;
  client_work->queue_max=qm;
  client_work->work_queue = malloc(noc*sizeof(struct job*));
  if(client_work->work_queue==NULL)//need to expand this err checking
    return 1;
  if(noc==0) {
    fprintf(stderr,"!!!env_api:init_env:Can't init env with no clients\n");
    usleep(99999999999);
    return 2;
  }

  int i =0,j;
  for(i; i<noc; ++i) {
    client_work->work_queue[i]=malloc(qm * sizeof(struct job));
  }
  client_work->queue_limit=malloc(noc*sizeof(*(client_work->queue_limit)));
  client_work->current_job=malloc(noc*sizeof(*(client_work->current_job)));

  for(i=0;i<noc;++i) {
    client_work->current_job[i]=0;
    client_work->queue_limit[i]=0;
  }

  has_init_env=1;
  return 0;
}


struct env_control * get_env_control(int env_id) {
  if(env_id<num_of_envs&&env_id>=0)
    return &env_control_array[env_id];
  else
    fprintf(stderr,"!!!env_api:get_env_control:Invalid Env_id[%d]. Max num of envs[%d]\n",env_id,num_of_envs);
    return NULL;
}


int init_api(int envs, struct env_control *client_work_array, struct env_dat *env_dat_array) {
  env_control_array=client_work_array;
  env_data_array=env_dat_array;

  num_of_envs=envs;
  in = malloc(sizeof(*in)*num_of_envs);
  if(!in) {
    fprintf(stderr,"init_api: Error, failed to allocate memory for istreams\n");
    return 1;
  }
  has_init=1;
  return 0;
}
int hook_env(int env_id, double *stream, int num_stream){
  if(env_id<0|| env_id >=  num_of_envs ) {
    fprintf(stderr,"hook_env: invalid env_id. Total envs %d\n", num_of_envs);
    return 1;
  }
  
  if(has_init) {
    in[env_id].streams=stream;
    in[env_id].num_streams=num_stream;
    return 0;
  }
  else {
    fprintf(stderr,"hook_env: can't hook env until init_api has been called\n");
    return 2;
  }
  
}

struct job* get_next_output(int queue_id, struct env_control *client_work) {
  int cj = client_work->current_job[queue_id];
  int ql = client_work->queue_limit[queue_id];
  if(cj==ql) {//no work
    vp(stdout,"no work\n");
    return NULL;
  } else if(cj+1!=ql&&cj+1!=client_work->queue_max) { //next job
    cj++;
    printf("next job %d\n",cj);
  } else if(cj+1==client_work->queue_max) { //rotate
    cj=0;
  }
  client_work->current_job[queue_id]=cj;
  vp(stdout,"RETURNING WORK::::::::::::::::::::::;\n");
  return &(client_work->work_queue[queue_id][cj]);
}


int set_output(int nobj_id,int stream_id,double data, int env_id) {
  vp(stdout,"Setting output!!!!!!!!!!!!!!!!!!\n");
  struct env_control *client_work=get_env_control(env_id);
  if(!client_work){
    fprintf(stderr,"Unable to get env_control for env_id:%d\n",env_id);
    return 1;
  }
  //printf("SET OUT!!!!!!!!!!!!!!!!!!!!\n");
    //pid_t x = syscall(__NR_gettid);
    //printf("Set output---THREAD: %d\n", x);
  if(!has_init) {
    fprintf(stderr," env_api has yet to init.\n");
    return 2;
  }
  if(client_work->num_of_clients==0){
    fprintf(stderr,"!!!:env_api:set_output:num_of_clients is 0");
    return 3;
  }

  int arr_id=nobj_id% client_work->num_of_clients;
  int cj = client_work->queue_limit[arr_id];

  if(cj+1 != client_work->queue_max && cj+1 != (client_work->current_job)[arr_id]) { 
  //next work not at max, and not at current job (not backed up) - set next job
    cj++;
  } else if(cj+1 == client_work->queue_max && (client_work->current_job)[arr_id] != 0){ 
  //next job at max and cur job not at 0
    cj=0;
  } else if( (cj+1 == (client_work->current_job)[arr_id] ) || 
    ( cj+1 == client_work->queue_max && (client_work->current_job)[arr_id] == 0 ) ) {//queue backed up
    fprintf(stderr,"Env queue backed up, dropping request at job#:%d\n",cj+1);
    return 4;
  }
  //set work
   

  ((client_work->work_queue)[arr_id][cj]).sid=stream_id;
  (client_work->queue_limit)[arr_id]=cj;
  ((client_work->work_queue)[arr_id][cj]).dat=data;
}
int get_istream(int env,int stream_id, double *value) {
  if(!has_init || env<0 || stream_id<0 || env >=  num_of_envs 
       || !in || !has_init_env) {
    fprintf(stderr,"Error: attempt at getting istream involved invalid parameters\n");
    return 1;
  } else {//check must be seperate in case in[env] is null
      if(in[env].num_streams!=0 && in[env].streams){
        (*value)= in[env].streams[stream_id];
        return 0;
      } else {
        fprintf(stderr,"no streams for given env\n");
        return 2;
      }
 
  }

}

int get_max_envid() {
  return num_of_envs;
}
void free_envapi();//tbi

