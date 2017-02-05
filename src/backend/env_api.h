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
  int num_streams;
  double *streams;//1d array of doubles, env sets this = to a double array 
};
struct job {
  int sid;//stream id
  int nobj_id;//id of object who sent it
  double dat;//data in stream
};

struct env_dat {
  int num_of_objs;//regulates util array int num_of_istream;//number of input streams
  //util[num_of_objs]
  double *util;  
  //double[stream_id]
  double *input_stream;
};


/* client_control:
   client should keep pointer to this to set appropriate vars
   to control the corresponding env. Mainly used to control/
   add work to the environment work queue.
*/
struct env_control{
  //number of input streams (from env to objs) - ostream # is arbritrary and handled by env
  int num_of_istream;
  //number of threads the obj work pool has. Each thread has its own work queue
  int num_of_clients;
  //max number of events in each work queue
  int queue_max;
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

/* FUNCTIONS */
//returns env_control from the array stored in env_api
struct env_control * get_env_control(int env_id);
/*
  called by main, used mainly to set array sizes. Called once.
  (number of total envs)
  returns 0 on success.
*/
int init_api(int num_of_envs, struct env_control *client_work_array, struct env_dat *env_data_array);
/*
   called by main to init the api for a specific env instance. Should be called once per env.
   (num of clients, queue_max, control structure to be initialized, data structure to be init  )
   returns 0 on success
*/
int init_env(int noc, int qm,  struct env_control *client_work, struct env_dat*);
/*
  Called from env, sent an pointer to their double array of istream values
  function stores pointer in env_api to be called
  returns 0 on success
*/
int hook_env(int env_id, double *streams, int num);








//adds to work queue - called from a neur in nobj
int set_output(int nobj_id,int stream_id, double dat, int env_id);

/*
    Grabs next job from env work queue
    Function used to abstract work queue from the env itself
    Called from env. 
*/
struct job* get_next_output(int queue_id, struct env_control *client_work);


double get_input(int stream_id);//grabs from queue 

/*
   LOH 9-1-16 - implement below functions

*/

/*  x
  returns 0 on fail, 1 on success. 
  to be sent down to an obj
*/
int get_istream(int env,int stream_id, double *value);



/* x
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



#endif

