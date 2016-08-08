#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "nobj.h"
#include "stimpool.h"
#include <unistd.h>

static pthread_t *workers;
static struct contract *contracts;
static int num_of_workers=4;
static int max_queue=100;
struct stim_param ****work_pool;

void * worker_thread(void *contract_v){

  
  struct contract *contract = (struct contract*)(contract_v);
  int last_job=0; 
  
  while((*contract).fired==0) {//dont exit thread
    
    if((*contract).pool_size!=(*contract).current_job) {

      last_job=(*contract).current_job;
      if((*contract).current_job+1==max_queue) {
        (*contract).current_job=0;
      } else {
        (*contract).current_job++;
      }



      if((contract)==NULL) {
        printf("null contract\n");
        //exit(-1);
      }
      printf("THREAD!!:%d\n",(*contract).current_job);
      //work
    
     if((*work_pool[(*contract).id][(*contract).current_job])==NULL) {
      // if(1==0){
        printf("  Bad job!!\n");//exit(-1);
        continue;
      }
      stim((*work_pool[(*contract).id][(*contract).current_job]));
      //free queue spot
      free(  *(work_pool[(*contract).id][(*contract).current_job])  );
 
    
    } else{}

    if((*contract).pool_size==(*contract).current_job) {//last job in queue
      printf("empty queue... %\n");
      //usleep((*contract).time_to_sleep);
      usleep(500000);
    }
  }
}


void manager(struct stim_param **par) {
  struct stim_param *p = (*par);
  int worker = (*(*p).nobj_props).nobj_id  % num_of_workers;
  printf(" worker id: %u\n",worker);
  printf(" pool: %d curjob: %d\n",contracts[worker].pool_size,contracts[worker].current_job);
  if(contracts[worker].pool_size==max_queue) {
    if(contracts[worker].pool_size+1==contracts[worker].current_job) {
      printf("  OBJ ID: %u has a backed up work queue, dropping job!!!Sleeping current thread to slow down\n",(*(*p).nobj_props).nobj_id );
      usleep(1000000);
    } else {
      if(p != NULL) { 
        printf("    Setting work...\n");
        work_pool[worker][contracts[worker].pool_size+1]=par;
        usleep(50000);
        contracts[worker].pool_size++;
      } else {
        printf("  Manager: null work\n");
      }
    }
  }
  else {
    if(contracts[worker].current_job==0) {
      printf("  OBJ ID: %u has a backed up work queue, dropping job!!!Sleeping current thread to slow down\n",(*(*p).nobj_props).nobj_id );
      usleep(1000000);
    } else {
      contracts[worker].pool_size=0;

        work_pool[worker][contracts[worker].pool_size]=par;
    }
  }
  
}
void wait_for_threads() {
  pthread_join(workers[0],NULL);
}
void init_workers(int num) {
  int i = 0;
  int j =0;
  num_of_workers=num; 
  workers=malloc(sizeof(workers[0])*num);
  contracts=malloc(sizeof(contracts[0])*num);
  work_pool=malloc(sizeof(struct stim_param***)*num);//alloc pool for each all threads
  for(i;i<num;++i) {
    j=0;
    contracts[i].id=i;
    contracts[i].fired=0;
    contracts[i].pool_size=-1;
    contracts[i].current_job=-1;
    contracts[i].time_to_sleep=50000;
    work_pool[i]=malloc(sizeof(struct stim_param**)*max_queue);//alloc space for job
    for(j;j<max_queue;++j) {
      work_pool[i][j]=malloc(sizeof(struct stim_param**));
    }
    pthread_create(&workers[i],NULL,worker_thread,&contracts[i]);
  }

}
