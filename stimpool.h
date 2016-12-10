#ifndef STIMPOOL_H
#define STIMPOOL_H
#include <stdlib.h>
#include <pthread.h>
#include "nobj.h"

/*
  Used as as clock
*/
extern double progress;
extern double progress_step;

//hols data to be passed to workers
struct contract {
  int id;
  int fired; //if not 0 - exit thread
  /*
    state
    -----
    0: sleeping/ready
    1: job in place
    2: working
    3: ready - only here if last job push set jobs_in_queue > 0. Loops continiously without sleep.
  */
  char state;
  unsigned int time_to_sleep;//after each empty check of work queue, sleep this many microseconds (1 millionth of a sec)

  //should be thread safe by design. Element at current_job is being updated to worker
  //Element at pool_size is being updated by manager
  //if current_job = pool_size -> work_queue empty. Set both to 0
  /*Params to work on */
  //todo - 100 probably needs to change
  int pool_size;//index of last pool work s(total size of work)
  int current_job;//index of job being currently worked on

};

void wait_for_threads() ;
void manager(struct stim_param *p);
void * worker_thread(void *contract_v);

#endif
