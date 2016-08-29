#include "env_api.h"
#include <stdlib.h>
#include <stdio.h>


static struct env_control client_work;
static struct stream in;
static char has_init =0;
// (num of clients, queue_max )
struct env_control * init_envapi(int noc, int qm) {
  client_work.num_of_clients=noc;
  client_work.queue_max=qm;
  client_work.work_queue = malloc(noc*sizeof(*client_work.work_queue));
  int i =0;
  for(i;i<noc;++i) {
    client_work.work_queue[i]=malloc(qm*sizeof(*(client_work.work_queue[0])));
  }
  client_work.queue_limit=malloc(noc*sizeof(*client_work.queue_limit));
  client_work.current_job=malloc(noc*sizeof(*client_work.current_job));
  for(i=0;i<noc;++i) {
    client_work.current_job[i]=0;
    client_work.queue_limit[i]=0;
  }
  has_init=1;
  return &client_work;
}
struct job* get_next_output(int queue_id) {
  int cj = client_work.current_job[queue_id];
  int ql = client_work.queue_limit[queue_id];
  if(cj==ql) {//no work
    printf("no work\n");
    return NULL;
  } else if(cj+1!=ql&&cj+1!=client_work.queue_max) { //next job
    cj++;
    //printf("next job %d\n",cj);
  } else if(cj+1==client_work.queue_max) { //rotate
    cj=0;
  }
  client_work.current_job[queue_id]=cj;
  return &(client_work.work_queue[queue_id][cj]);
}
void set_output(int nobj_id,int stream_id,double data) {
  if(!has_init) {
    printf(" env_api has yet to init.\n");
    return;
  }
  int arr_id=nobj_id%client_work.num_of_clients;
  int cj = client_work.queue_limit[arr_id];
  if(cj+1 != client_work.queue_max && cj+1 != client_work.current_job[arr_id]) { 
  //next work not at max, and not at current job (not backed up) - set next job
    cj++;;
  } else if(cj+1 == client_work.queue_max && client_work.current_job[arr_id] != 0){ 
  //next job at max and cur job not at 0
    cj=0;
  } else if( (cj+1 == client_work.current_job[arr_id] ) || 
    ( cj+1 == client_work.queue_max && client_work.current_job[arr_id] == 0 ) ) {//queue backed up
    printf("Env queue backed up, dropping request at job#:%d\n",cj+1);
    return;
  }
  //set work
   
  client_work.work_queue[arr_id][cj].sid=stream_id;
  client_work.work_queue[arr_id][cj].dat=data;
  client_work.queue_limit[arr_id]=cj;
  // printf("setting env work for arr_id: %d, at QLjob#: %d  CJjob:%d\n",nobj_id,cj,client_work.current_job[arr_id]);
}

void free_envapi();//tbi
