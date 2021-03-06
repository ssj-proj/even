#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "nobj.h"
#include "stimpool.h"
#include "err.h"
#include <unistd.h>

static pthread_t *workers;
static struct contract *contracts;
static int num_of_workers=4;
static int max_queue=4;
struct stim_param **work_pool;




void * worker_thread(void *contract_v){

  
  struct contract *contract = (struct contract*)(contract_v); 
  
  while(contract->fired==0) {//dont exit thread
    
    if(contract->pool_size!=contract->current_job) {

      if(contract->current_job+1==max_queue) {
        contract->current_job=0;
      } else {
        contract->current_job++;
      }

      //work
      stim(&(work_pool[contract->id][contract->current_job]));

      contract->progress+=contract->progress_step;
      
    
    } else{}

    if(contract->pool_size==contract->current_job) {//last job in queue
      //printf("[sleeping]");
      //proc_err("INFO:stimpool.c:worker finished all jobs\n",5);
      usleep(contract->time_to_sleep);
    } else {
      //printf("[overtime(cj/pool)(%d / %d)]", (*contract).current_job ,
      //(*contract).pool_size);
  
    }
  }
}

/*
  can be called from neur stim, uses sim_param to determine which worker stim belongs
  to and then returns the progress(time) of that worker.
*/
double get_progress(struct stim_param *p) {
  int worker = (*p->nobj_props).nobj_id  % num_of_workers;
  return contracts[worker].progress;
}
double get_progress_step(struct stim_param *p) {
  int worker = (*p->nobj_props).nobj_id  % num_of_workers;
  return contracts[worker].progress_step;
}
double get_nobj_progress(int nobj_id) {
  int worker = nobj_id  % num_of_workers;
  return contracts[worker].progress;
}
double get_nobj_progress_step(int nobj_id) {
  int worker = nobj_id  % num_of_workers;
  return contracts[worker].progress_step;
}
void manager(struct stim_param *p) {
  static int num_of_call=0;
  num_of_call++;
  if( p->vars[p->neur_to][2] == 0) {
    proc_err("WARNING:stimpool.c: neur with bad fire strength: obj%u:neur%u:fire_strength = 0\n",4,p->nobj_props->nobj_id,p->neur_to);
  }
  int worker = (*p->nobj_props).nobj_id  % num_of_workers;

  if(contracts[worker].pool_size<max_queue-1) {
    if(contracts[worker].pool_size+1==contracts[worker].current_job) {
      proc_err("ERROR:stimpool.c: OBJ ID: %u has a backed up work queue(!0), dropping job!!!\nCurrent Job #[%d]\n",2,(*p->nobj_props).nobj_id,contracts[worker].current_job );
    } else {
      if(p != NULL) {  
        copy_stim_param(*p,&work_pool[worker][contracts[worker].pool_size+1]); 
        contracts[worker].pool_size++;
      } else {
        printf("  manager: received null stim_param\n");
      }
    }
  }
  else {//at max queue number
    if(contracts[worker].current_job==0) {//next available job isn't done
      proc_err("ERROR:stimpool.c: OBJ ID: %u has a backed up work queue, dropping job!!!Current Job #[%d], max queue: %d\n",2,(*(*p).nobj_props).nobj_id,contracts[worker].current_job,max_queue );
     
    } else {
      contracts[worker].pool_size=0;
      copy_stim_param(*p,&work_pool[worker][contracts[worker].pool_size]);
    }
  }
  
}
void wait_for_threads() {
  pthread_join(workers[0],NULL);
}
// (num_of_threads to make)
void init_workers(int num) {
  proc_err("\n=====Init Workers====\n",0);
  proc_err("Creating %d threads\n",0, num);
  int i = 0;
  int j =0;
  num_of_workers=num; 
  workers=malloc(sizeof(workers[0])*num);
  contracts=malloc(sizeof(contracts[0])*num);
  work_pool=malloc(sizeof(struct stim_param*)*num);//alloc pool for each all threads
  for(i;i<num;++i) {
    
    j=0;
    contracts[i].id=i;
    contracts[i].fired=0;
    contracts[i].pool_size=-1;
    contracts[i].current_job=-1;
    contracts[i].time_to_sleep=1000;//has limited effect, workers only sleep when there are no jobs left in queue
    contracts[i].progress=0;
    contracts[i].progress_step=.0001;
    work_pool[i]=malloc(sizeof(struct stim_param)*max_queue);//alloc space for job
    proc_err("Debug:Creating stimpool worker thread\n",5);
    pthread_create(&workers[i],NULL,worker_thread,&contracts[i]);
  }

}
