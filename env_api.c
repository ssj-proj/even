#include "env_api.h"
#include <stdlib.h>
#include <stdio.h>

/*
	CAll in This struct holds all work for env
	work queue.
*/


static struct stream in;

 /*
    array of structs contain all neccessary istream_data
    [env_id]
  */
struct istream_list *istreams;


static char has_init =0;
// (num of clients, queue_max )
/*
	Creates an env control object used for sharing control data back and 
	forth between the env and main func thread. the returned object
	is sent to the env when initiliazing the env. 
*/
struct env_control * init_envapi(int noc, int qm) {
  struct env_control *client_work = malloc(sizeof(client_work));
  client_work->num_of_clients=noc;
  client_work->queue_max=qm;
  client_work->work_queue = malloc(noc*sizeof(*client_work->work_queue));
  int i =0;
  for(i;i<noc;++i) {
    client_work->work_queue[i]=malloc(qm*sizeof(*(client_work->work_queue[0])));
  }
  client_work->queue_limit=malloc(noc*sizeof(*client_work->queue_limit));
  client_work->current_job=malloc(noc*sizeof(*client_work->current_job));
  for(i=0;i<noc;++i) {
    client_work->current_job[i]=0;
    client_work->queue_limit[i]=0;
  }
  has_init=1;
  return client_work;
}

struct job* get_next_output(int queue_id, struct env_control *client_work) {
  int cj = client_work->current_job[queue_id];
  int ql = client_work->queue_limit[queue_id];
  if(cj==ql) {//no work
    printf("no work\n");
    return NULL;
  } else if(cj+1!=ql&&cj+1!=client_work->queue_max) { //next job
    cj++;
    //printf("next job %d\n",cj);
  } else if(cj+1==client_work->queue_max) { //rotate
    cj=0;
  }
  client_work->current_job[queue_id]=cj;
  return &(client_work->work_queue[queue_id][cj]);
}
void set_output(int nobj_id,int stream_id,double data, struct env_control *client_work) {
  if(!has_init) {
    printf(" env_api has yet to init.\n");
    return;
  }
  int arr_id=nobj_id%client_work->num_of_clients;
  int cj = client_work->queue_limit[arr_id];
  if(cj+1 != client_work->queue_max && cj+1 != client_work->current_job[arr_id]) { 
  //next work not at max, and not at current job (not backed up) - set next job
    cj++;;
  } else if(cj+1 == client_work->queue_max && client_work->current_job[arr_id] != 0){ 
  //next job at max and cur job not at 0
    cj=0;
  } else if( (cj+1 == client_work->current_job[arr_id] ) || 
    ( cj+1 == client_work->queue_max && client_work->current_job[arr_id] == 0 ) ) {//queue backed up
    printf("Env queue backed up, dropping request at job#:%d\n",cj+1);
    return;
  }
  //set work
   
  client_work->work_queue[arr_id][cj].sid=stream_id;
  client_work->work_queue[arr_id][cj].dat=data;
  client_work->queue_limit[arr_id]=cj;
  // printf("setting env work for arr_id: %d, at QLjob#: %d  CJjob:%d\n",nobj_id,cj,client_work.current_job[arr_id]);
}

void free_envapi();//tbi


