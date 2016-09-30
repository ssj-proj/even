/*
  int must be 32 bit
  long must be 64 bit
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nobj.h"
#include "behaviors.h"
#include "stimpool.h"
#include "env_api.h"
#include "env_c_random/random_env.h"
#include <unistd.h>

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
void main() {
  int num_of_objs=3;
  //also dictates number of buffer arrays-be sure to pass this around where neccessary
  //(ie: stim_pool and env):  
  int num_of_threads=2;
  struct nobj_meta *nobj_props;  
  /*   
    NOBJ VARS
  */
  //[object id][neur id][neur propert id]
  unsigned int ***nobjs;//should not change during obj runtime

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
  //malloc nobj vars and their mutex's
  nobj_props  =malloc( num_of_objs * sizeof(struct nobj_meta));
  weights     =malloc( num_of_objs * sizeof(double**));
  weights_lock=malloc( num_of_objs * sizeof(pthread_t*));
  nobjs       =malloc( num_of_objs * sizeof(unsigned int**));
  cons        =malloc( num_of_objs * sizeof(unsigned int**));
  cons_lock   =malloc( num_of_objs * sizeof(pthread_t*));
  conids      =malloc( num_of_objs * sizeof(unsigned int**));
  conids_lock =malloc( num_of_objs * sizeof(pthread_t*));
  nvar        =malloc( num_of_objs * sizeof(double**));
  nvar_lock=malloc( num_of_objs * sizeof(pthread_t*));
  struct lock_group *locks = malloc(sizeof(struct lock_group));
  
  //used for neur thread distribution - stim parameters
  struct stim_param **param = malloc(sizeof(*param)*num_of_objs);
   
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
  char *file=malloc(255);;
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
    display_con_props(nobj_id,cons,conids,weights,nobj_props);

    strcpy(file,file_without_ext);
    strcat(file,var_extension);
    double **var_props = parse_vars_file(file,&nobj_props[nobj_id]);
    if(con_props==NULL) {
      printf("ERROR parsing var file.\n");
      exit(-1);
    }
    init_vars(nobj_id,var_props,nobj_props[nobj_id],&nvar);
    //display_vars_props(nvar[nobj_id],nobj_props[nobj_id]);
    //malloc locks - on lock for each neur for each of its array properties
    (*locks).vars_lock = malloc(sizeof(pthread_mutex_t) * nobj_props[nobj_id].num_of_neurs);
    (*locks).cons_lock = malloc(sizeof(pthread_mutex_t) * nobj_props[nobj_id].num_of_neurs);
    (*locks).conids_lock = malloc(sizeof(pthread_mutex_t) * nobj_props[nobj_id].num_of_neurs);
    (*locks).weights_lock = malloc(sizeof(pthread_mutex_t) * nobj_props[nobj_id].num_of_neurs);

  }
  struct behav_pool behaviors; 
  behaviors.behaviors = malloc(sizeof(behavior) * 1 );
  behaviors.behaviors[0]=&b1;
  behaviors.threshholds = malloc(sizeof(threshhold) * 1 );
  behaviors.threshholds[0]=&t1;
  //(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim, struct behav_pool bp,unsigned int***nobj,unsigned int***cons,unsigned int***conids, double***weights, double***vars)

  
  init_workers(num_of_threads);//init thread pool
 

  /* 
    Testing
  */
  int i =0; 
  struct env_control *env = init_envapi(num_of_threads,100);
 
  for(i;i<num_of_objs;++i){
    (*param[i]).neur_from=99;
    (*param[i]).neur_to=0;
    (*param[i]).conid=0;
    (*param[i]).stim=199;
    (*param[i]).bp=&behaviors;
    (*param[i]).nobj=nobjs[i];  
    (*param[i]).cons=cons[i];
    (*param[i]).conids=conids[i];
    (*param[i]).weights=weights[i];
    (*param[i]).vars=nvar[i];
    (*param[i]).nobj_props=&(nobj_props[i]);
    manager(param[i]);//drop work into thread pool
  }
   pthread_t env_t;
   
   /*
      to implement multiple envs, create array of void* function pointers
      create array of ints to hold the states of each env
      create a thread for each env
   */
   struct env_dat env_data;
   init_env(num_of_objs,env,&env_data);//send control object to env
   int *state = malloc(sizeof(int));
   pthread_create(&env_t,NULL,main_loop,state);


  printf("env: num_of_works: %d, queue_max: %d \n", env->num_of_clients,env->queue_max);
  int c;
  for(c=0;c<100;c++) {
    for(i=0;i<num_of_objs;++i) {
	set_output(i,0,c/100.0+i,env);
        usleep(1);
    }
  }
  printf("\nend\n");
  wait_for_threads();
 
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

  exit(0);

}


























