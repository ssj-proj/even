#ifndef STIMPOOL_H
#define STIMPOOL_H
#include <stdlib.h>
#include <pthread.h>
#include "nobj.h"

//hols data to be passed to workers
struct contract {
  char fired; //if not 0 - exit thread
  /*
    state
    -----
    0: sleeping/ready
    1: job in place
    2: working
    3: ready - only here if last job push set jobs_in_queue > 0. Loops continiously without sleep.
  */
  char state;
  char jobs_in_queue;//when manager thread see worker is sleeping, it will place next job in worker,
    //if manager has more than 1 job to place, worker will not sleep after job is done. 0 = no jobs in queue (last job)
  unsigned int time_to_sleep;//after each empty check of work queue, sleep this many microseconds (1 millionth of a sec)

  /*Params to work on */
  struct *stim_param param;//be sure to free when job is done
  
}


#endif
