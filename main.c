/*
  int must be 32 bit
  long must be 64 bit
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include "nobj.h"
#include "behaviors.h"
#include "stimpool.h"
#include "env_api.h"
#include "env_c_random/random_env.h"
#include <unistd.h>

struct sigaction old_action;

unsigned int** create_props(int neurs, int neur_props) {
 
  unsigned int** props=malloc(sizeof(unsigned int*)*neurs);
  int i =0;
  for(i; i < neurs;++i){
    props[i]=malloc(sizeof(unsigned int)*neur_props);  
    int j =0;
    for(j;j<neur_props;++j){
      props[i][j]=(i+j);
    }
  }
  return props;
}
void end_program(int sig_no){
    printf("CTRL-C pressed\n");
    proc_err("====Exiting program====\n",0);
    FILE *f = fopen("eve.log","r");
    sigaction(SIGINT, &old_action, NULL);
    kill(0, SIGINT);

}
void main() {
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = &end_program;
  sigaction(SIGINT, &action, &old_action);

  init_err("./eve.log",3,2);

  int num_of_objs=1;
  //also dictates number of buffer arrays-be sure to pass this around where neccessary
  //(ie: stim_pool and env):  
  int num_of_threads=2;
  
  /*   
    NOBJ VARS
  */
  //[object id][neur id][neur propert id]
  unsigned int ***nobjs;//should not change during obj runtime

  struct nobj_meta *nobj_props; 
 
  //[object id][neur id][to con neur id]
  unsigned int ***cons;//will change during runtime
  pthread_t **cons_lock;//lock per neur
  /* 
    conid is associated to sending neur
    conid is used by sending neur, sent to receiving neur along with sitm upon firing
    receiving neur using conid as an index to its weights array
    these are 'dynimcally' generated at load time and not specified in config files
  */
  //[object id][neur id][to con conid]
  unsigned int ***conids;//will change during runtime
  pthread_t **conids_lock;//lock per neuer

  //[object id][neur id][receiving weights]
  double ***weights;//will change during runtime
  pthread_t **weights_lock;

  //[object id][neur id][variable] -- TBDOC / TBI
  /* A variable length list that holds variables for the neurs that are affected
     By neur behavior. To Be planned and coded */
  double ***nvar;//will change during runtime
  pthread_t **nvar_lock;
  



  /*   
    END NOBJ VARS
  */

  /* ENV VARS */
  int num_of_environments=1;
  //array of struct, element per env
  struct env_control *envs;// = malloc(sizeof(struct env_control));
  //aray of struct, element per env
  struct env_dat *env_data;//= malloc(sizeof(struct env_control));
  /*
    1D array [obj_id] = struct map { stream_id, neur_id] };
  */
  struct i_map **i_maps;
  int num_of_envs=1;
  int *num_of_streams;//1d array of ints that list number of streams for each env


  /* END ENV VARS */
  //malloc nobj vars and their mutex's
  nobj_props  =malloc( num_of_objs * sizeof(struct nobj_meta));
  weights     =malloc( num_of_objs * sizeof(double**));
  nobjs       =malloc( num_of_objs * sizeof(unsigned int**));
  cons        =malloc( num_of_objs * sizeof(unsigned int**));
  conids      =malloc( num_of_objs * sizeof(unsigned int**));
  nvar        =malloc( num_of_objs * sizeof(double**));
  i_maps      =malloc( num_of_objs * sizeof(*i_maps));
  envs = malloc(sizeof(struct env_control)*num_of_environments);
  env_data = malloc(sizeof(struct env_control)*num_of_environments);
  num_of_streams = malloc(num_of_envs * sizeof(*num_of_streams));
  
  /*
    used for neur thread distribution - stim parameters
    an array of pointers to stim_param objs
    one entry for each object
  */
  struct stim_param **param = malloc(sizeof(*param)*num_of_objs);
   
  //To do - more err checking
  if(weights==NULL) {
    printf("Error: failed to malloc weights\n");
    exit(-1);
  }



  
  unsigned int **props;
  unsigned int nobj_id=0;
  char *obj_file_base="./obj/obj_";
  char *des_extension=".des";
  char *con_extension=".con";
  char *var_extension=".var";

  char *objnum=malloc(4);
  char *file_without_ext=malloc(255);
  char *file=malloc(255);


  /*
    BEGIN INITILIZATION
  */
  /*
    Define possible behaviors, a part of stim param
    TODO - function to dynamically load this
  */
  struct behav_pool behaviors; 
  behaviors.behaviors = malloc(sizeof(behavior) * 1 );
  behaviors.behaviors[0]=&empty_behavior;
  behaviors.threshholds = malloc(sizeof(threshhold) * 2 );
  behaviors.threshholds[1]=&thresh_hold;
  behaviors.threshholds[0]=&regulated_thresh;


  /* load settings for obj from their init files */
  for(nobj_id;nobj_id<num_of_objs;++nobj_id) { 
    memset(file,0,sizeof(file));
    memset(objnum,0,sizeof(file));
    memset(file_without_ext,0,sizeof(file));

    sprintf(objnum,"%d",nobj_id);
    strcat(file_without_ext,obj_file_base);
    strcat(file_without_ext,objnum);
    
    strcpy(file,file_without_ext);
    strcat(file,des_extension);
    printf("   INIT OBJECT %u\n",nobj_id);
    param[nobj_id] = malloc(sizeof(struct stim_param));
    props = parse_nobj_file(file,&nobj_props[nobj_id]);
    nobj_props[nobj_id].nobj_id=nobj_id;//probably should be moved into parse_nobj_file()

    init_nobj(nobj_id,props,nobj_props[nobj_id],&nobjs);
    //display_neur_props(nobj_id,nobjs,nobj_props);
    //free_nobj(nobj_id,nobj_props[nobj_id],&nobjs);

    strcpy(file,file_without_ext);
    strcat(file,con_extension);
    unsigned int **con_props = parse_con_file(file,&nobj_props[nobj_id]);
    nobj_props[nobj_id].nobj_id=nobj_id;
    if(con_props==NULL) {
      printf("ERROR parsing con file.\n");
      exit(-1);
    }
    init_cons(nobj_id, con_props, nobj_props[nobj_id], &cons, &conids, &weights); 
    //display_con_props(nobj_id,cons,conids,weights,nobj_props);

    strcpy(file,file_without_ext);
    strcat(file,var_extension);
    double **var_props = parse_vars_file(file,&nobj_props[nobj_id]);
    if(con_props==NULL) {
      printf("ERROR parsing var file.\n");
      exit(-1);
    }
    init_vars(nobj_id,var_props,nobj_props[nobj_id],&nvar);
    //display_vars_props(nvar[nobj_id],nobj_props[nobj_id]);

    strcpy(file,file_without_ext);
    strcat(file,".in");
    
    parse_i_file(file,&i_maps[nobj_id],&nobj_props[nobj_id]);

     /* base values for sim param fro nobj -- rethink this design TODO */
    (*param[nobj_id]).bp=&behaviors;
    (*param[nobj_id]).nobj=nobjs[nobj_id];  
    (*param[nobj_id]).cons=cons[nobj_id];
    (*param[nobj_id]).conids=conids[nobj_id];
    (*param[nobj_id]).weights=weights[nobj_id];
    (*param[nobj_id]).vars=nvar[nobj_id];
    (*param[nobj_id]).nobj_props=&(nobj_props[nobj_id]);
    (*param[nobj_id]).neur_from=UINT_MAX;//When stim from anything but another nuer, from is max usigned int
    (*param[nobj_id]).conid=UINT_MAX;
  }


  //(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim, struct behav_pool bp,unsigned int***nobj,unsigned int***cons,unsigned int***conids, double***weights, double***vars)

   //Manually setting vars
  num_of_streams[0]=3;
  int i =0,j=0; //temp loop/index vars
  

  /* 
    Testing Nobjs
  */
  i=0;
  j=0;
  int *state = malloc(sizeof(int));//sent to env
  int init_errors = 0;//if ! 0 at end of all inits, err occured

  init_workers(num_of_threads);//init thread pool for nobjs
   /*
      to implement multiple envs, create array of void* function pointers
      create array of ints to hold the states of each env
      create a thread for each env
   */
   pthread_t env_t;
   init_errors+=init_api(1,envs,env_data);//send number of total envs so it can allocate arrays

   init_errors+=init_env(num_of_threads,100, &envs[0], &env_data[0]);//call init env for each env(1)
   env_data->num_of_objs = num_of_objs;

  /*
    init the env side, thism should call hook_env
    send the control structure, the data structure and the env_id
  */
   init_env0(&envs[0],&env_data[0],0);
   if(init_errors!=0){
     fprintf(stderr,"main: error with initilization: %d",init_errors);
   }
   pthread_create(&env_t,NULL,main_loop,state);//start env thread


  int errs = 0;
  printf("about to start input:");
  system("read");
  while(1) {
    for(i=0;i<num_of_objs;++i){//loop each object

    //printf(" Main: Obj loop %d\n  streams %d\n",i,nobj_props[i].num_of_istreams);
      for(j=0;j<nobj_props[i].num_of_istreams;++j){//loop each stream foreach object
        param[i]->neur_to=i_maps[i][j].neur_to;
        errs = get_istream(i_maps[i][j].env_id, i_maps[i][j].stream_id,&(*param[i]).stim);
        if(errs==0){
          printf("  istream value: %lf for stream_id: %d\n",(*param[i]).stim,i_maps[i][j].stream_id);
          (*param[i]).stim=101;
	  printf("about to start input:");
	  system("read");
          manager(param[i]);//drop work into thread pool
        } else {
          fprintf(stderr,"Error with gettng istreams: err#%d\n",errs);
        }
      }//end stream loop
    }//end obj loop
    
    break;
  }//main loop
  
  wait_for_threads();
  exit(0);


















  /*
    End Testing
  */
  /* Env Psuedo */
  /*
    #only working on istreams currently
      load settings from env to get total number of istream
      load settings from env to get initial istream max
      each array has accompanying array size of max istream :/
      create struct and management functions to manage input and output
         from these streams.

      load each obj in file, adjust istream array to make sure it fits. Grow istream array if needed :/
  */
   
  //loop inf
    //loop all envs [num_of_envs]
      //loop at istreams double istreams [num_of_envs][istream]
        //set var d = value of istream
        //loop clients of istream istream_clients[num_of_envs][istream][clients] 
          //set input to neur = d
  /* End Psuedo */
}


























