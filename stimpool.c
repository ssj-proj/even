#define STIMPOOL_H
#include <stdlib.h>
#include <pthread.h>
#include "nobj.h"
#include "stimpool.h"
#include <unistd.h>

static pthread_t *workers;
static struct contract *contracts;
static int num_of_workers;



struct contract init_contract(struct *stim_param p) {
  struct contract work;
  work.param=p;
  return work;
}
void * worker_thread(void *contract){

  while((*contract).fired==0) {//dont exit thread

    if((*contract).state==1) {//fresh job 
      (*contract).state=2;//state is working
      /* TBI - Do Work */
    
    }else{}

    if((*contract).jobs_queue==0) { // if not jobs in queue last run,
      (*contract).state=0;//sleeping,ready to work
      usleep((*contract).time_to_sleep)); 
    } else {
      (*contract).state=3;//not sleeping, ready to work
    }
  }
}


void manager(void *work) {
  

}
void init_workers(int num) {
  int i = 0;
  num_of_workers=num;
  for(i;i<num;++i) {
    pthread_t_create

  }

}
