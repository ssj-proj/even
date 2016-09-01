#ifndef ENV_API_H
#define ENV_API_H

//double input_streams[];
//double output_streams[];


/*
Main loop
grab and push data                 - imap
process data                       - omap
manage objs (create/delete/change)
*/

//held in memory in , grabs data from steam at index, feeds to neur_index at nobj_id
struct imap {
  int nobj_id;
  int neur_index;
  int stream_index;
};
//held in neur, when fire, it adds data(double_ at given index
struct omap {
  int stream_index;
};

/*  streams:
  should be init_by the environment code
  env code can process and set stream data at any time through any means
  the nobj will only grab input at specified time interval
  the nobj may produce output at any time by setting data in the work queue
  the env will write to input stream at any time, the input stream only
  represents the state of the stream at that point in time. It is possible
  the nobj will not receive every occured state if state is manipulated multi-
  ple times between time intervals. 
  output stream array is missing here, it is to be completely handled by the
  environment code as part of work processing
*/
struct stream {
  int num_in;
  double *in;
};
struct job {
  int sid;//stream id
  double dat;
};
/* client_control:
   client should keep pointer to this to set appropriate vars
*/
struct env_control{
  //number of input streams (from env to objs) - ostream # is arbritrary and handled by env
  int num_of_istream;

  //number of threads the obj work pool has. Each thread has its own work queue
  int num_of_clients;
  int queue_max;//max number of events in each work queue
  //must of seperate array for each submitting thread to avoid segfault
  struct job **work_queue;//[num_of_client_threads][queue];
  //array of int
  int *queue_limit;//when this hits queue_max, client sets to 0
   /* current_job:
    when it hits queue_max, env sets to 0
    env performs job up to queue_liimit - upon hitting, sleep and poll again
    in the event that the it is reading at the middle of set operating from another 
    thread it will only loose 1 poll worth of time and no segfault assumming client has set
    good data
  */
  int *current_job;//[num_of_client_threads];//only env reads/writes this
};

//         (num of clients, queue_max )
struct env_control * init_envapi(int noc, int qm);
void set_output(int nobj_id,int stream_id, double dat);//adds to work queue - from nobj_side
struct job* get_next_output(int queue_id);//gets next job in queue - from env side
double get_input(int stream_id);//grabs from queue - from nobj_side

/*
   LOH 9-1-16 - implement below functions

*/
/* 
  returns the value at the particular stream_id
  to be sent down to an obj
*/
double get_istream(int env,int stream_id);
/*
  returns total number of envs
  all env ids should be sequential 
*/
int get_max_envid();
/*
  returns number of ids.
  Since all stream_ids should be sequential(no missing numbers)
  return a number should imply all valid ids ( 0 - n)
*/
int get_max_istreamids(int env);

/*
returns int number of clients or <0 on failure
 params(pointer array to be malloc inside this function);
caller responsible for freeing
*/
int get_istream_clients(int env, int stream_id,int *clients);
/*

*/
int create_env---//loh

#endif

