#include "nobj.h"
#include "behaviors.h"
#include "stimpool.h"
#include "env_api.h"
#include "../environments/env_c_random/random_env.h"
#include "../environments/env_oai/env_oai.h"
#include "eve.h"
#include "err.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <errno.h>

struct sigaction old_action;
struct nobj_summary *nobj_sums;//made global so it could be easily shared with external api
static pthread_t program;
static const int period_wait=100000;




int get_period_wait() {
  return period_wait;
}

/*
  TODO - de-activate object to be re-used later
*/
int kill_nobj(int nobj) {



}
/*
  issue where nobj_id in array does not match file nobj_id
*/
int load_nobj(int nobj){


}




unsigned int** create_props(int neurs, int neur_props, struct main_control *control) {

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
  //TODO - save obj states
  sigaction(SIGINT, &old_action, NULL);
  kill(0, SIGINT);

}
struct main_control * create_control() {
  struct main_control *control = malloc(sizeof(struct main_control));
  strcpy(control->obj_file_base,"./obj/obj_");
  strcpy(control->des_extension,".des");
  strcpy(control->con_extension,".con");
  strcpy(control->var_extension,".var");
  strcpy(control->log_file,"./eve.log");
  control->log_verbosity=5;
  control->screen_verbosity=3;
  control->num_of_objs=1;
  control->num_of_threads=1;
  control->test=0;
  control->console_mode=0;
  return control;
}

//verifies integrity of control struct
int check_control(struct main_control *control){
  if(!control->obj_file_base){
    return -1;
  }
  if(!control->des_extension){
    return -2;
  }
  if(!control->con_extension){
    return -3;
  }
  if(!control->var_extension){
    return -4;
  }
  if(!control->log_file){
    return -5;
  }
  if(control->screen_verbosity<=0){
    return -6;
  }
  if(control->log_verbosity<=0){
    return -7;
  }
  if(control->num_of_objs<=0) {
    return -8;
  }
  if(control->num_of_threads<=0) {
    return -9;
  }




}
double get_obj_util(int obj_id) {
  return nobj_sums[obj_id].util;

}
int spin_off(struct main_control *control){
  printf("spinning off\n");
  pthread_create(&program,NULL,start_program,(void*)control);

}

struct init_var_struct{
  char *obj_file_base;
  char *des_extension;
  char *con_extension;
  char *var_extension;
  int num_of_objs;
  int num_of_threads;
  /*
    NOBJ VARS
  */
    //[object id][neur id][neur propert id]
  unsigned int ***nobjs;//should not change during obj runtime
  struct nobj_meta *nobj_props;
    //[object id][neur id][to con neur id]
  unsigned int ***cons;//will change during runtime
  /*
    conid is associated to sending neur
    conid is used by sending neur, sent to receiving neur along with sitm upon firing
    receiving neur using conid as an index to its weights array
    these are 'dynimcally' generated at load time and not specified in config files
  */
  //[object id][neur id][to con conid]
  unsigned int ***conids;//will change during runtime
  //[object id][neur id][receiving weights]
  double ***weights;//will change during runtime
  //[object id][neur id][variable] -- TBDOC / TBI
  /* A variable length list that holds variables for the neurs that are affected
     By neur behavior. To Be planned and coded */
  double ***nvar;//will change during runtime
  /*
    END NOBJ VARS
  */
  /*
    ENV VARS
  */
  int num_of_environments;
  //array of struct, element per env
  struct env_control *envs;// = malloc(sizeof(struct env_control));
  //aray of struct, element per env
  struct env_dat *env_data;//= malloc(sizeof(struct env_control));
  /*
    1D array [obj_id] = struct map { stream_id, neur_id] };
  */
  struct i_map **i_maps;
  struct behav_pool *behaviors;
  /*
    END ENV VARS
  */


};



/*
	LOH 8-6-17 - fixing compile -- putting vars into a struct

*/



int init_program(struct main_control *control, struct init_var_struct *vars){
  /*
    TODO - dynamic vars:
      num_of_environments
  */
  vars->obj_file_base=control->obj_file_base;
  vars->des_extension=control->des_extension;
  vars->con_extension=control->con_extension;
  vars->var_extension=control->var_extension;
  vars->num_of_objs=control->num_of_objs;
  //also dictates number of buffer arrays-be sure to pass this around where neccessary
  //(ie: stim_pool and env):
  vars->num_of_threads=control->num_of_threads;//number of nobj worker threads
  printf("eve.c:Init: Obj_file_base: %s\n",vars->obj_file_base);
  printf("eve.c:Init: Obj_file_base: %s\n",control->log_file);
  //malloc nobj vars
  vars->nobj_props  =malloc(vars->num_of_objs * sizeof(struct nobj_meta));
  //vars->nobj_sums   =malloc(vars->num_of_objs * sizeof(struct nobj_summary));
  vars->weights     =malloc(vars->num_of_objs * sizeof(double**));
  vars->nobjs       =malloc(vars->num_of_objs * sizeof(unsigned int**));
  vars->cons        =malloc(vars->num_of_objs * sizeof(unsigned int**));
  vars->conids      =malloc(vars->num_of_objs * sizeof(unsigned int**));
  vars->nvar        =malloc(vars->num_of_objs * sizeof(double**));
  vars->i_maps      =malloc(vars->num_of_objs * sizeof(*(vars->i_maps)));
  vars->envs = malloc(sizeof(struct env_control)*(vars->num_of_environments));
  vars->env_data = malloc(sizeof(struct env_control)*(vars->num_of_environments));

  vars->behaviors = malloc(sizeof(struct behav_pool));


  /*
    Load behaviors
    TODO - dynamically load this
  */
  (vars->behaviors)->behaviors = malloc(sizeof(behavior) * 1 );
  (vars->behaviors)->behaviors[0]=&empty_behavior;
  (vars->behaviors)->threshholds = malloc(sizeof(threshhold) * 2 );
  (vars->behaviors)->threshholds[1]=&thresh_hold;
  (vars->behaviors)->threshholds[0]=&regulated_thresh;
}
void *start_program(void *control_ptr) {
  /*
    init
    main loop
    object load/unload
  */
  //set up exit action
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = &end_program;
  sigaction(SIGINT, &action, &old_action);



  int nobj_id=0;
  char *objnum =malloc(4);
  char *file_without_ext=malloc(255);
  char *file=malloc(255);
  /*
    used for neur thread distribution - stim parameters
    an array of pointers to stim_param objs
    one entry for each object
  */
  struct stim_param **param;
  unsigned int **props;




  struct main_control *control = (struct main_control*)control_ptr;
  struct init_var_struct *program_vars = malloc(sizeof(struct init_var_struct));//create struct to hold all global vars

  init_program(control, program_vars);


  if(check_control(control) <0) {
    printf("Obj_file_base: %s | control err: %d\n",control->obj_file_base,check_control(control));
    return;
  }

  //sets error log location and verbosity level
  //TODO - check this have been set, if not use default values
  init_err(control->log_file,control->log_verbosity,control->screen_verbosity, control->console_mode);//file,file verbosity, screen verbosity



 // usleep(500000000);
  //To do - Move to init var err check function
  if(program_vars->weights==NULL) {
    proc_err("Error: failed to malloc weights. objs %d\n",program_vars->num_of_objs);
    exit(-1);
  }



  /*
    BEGIN INITILIZATION
  */


  /* load settings for obj from their init files */
  for(nobj_id;nobj_id<program_vars->num_of_objs;++nobj_id) {
    /*
       each obj file path:
	obj_file_base + objnum + (type extension)
    */
    memset(file,0,sizeof(file));
    memset(objnum,0,sizeof(file));
    memset(file_without_ext,0,sizeof(file));

    nobj_sums[nobj_id].util=0;
    nobj_sums[nobj_id].active=1;

    sprintf(objnum,"%d",nobj_id);
    strcat(file_without_ext,program_vars->obj_file_base);
    strcat(file_without_ext,objnum);

    strcpy(file,file_without_ext);
    strcat(file,program_vars->des_extension);
    proc_err("   INIT OBJECT %u\n",4,nobj_id);
    param[nobj_id] = malloc(sizeof(struct stim_param));
    props = parse_nobj_file(file,&(program_vars->nobj_props[nobj_id]));
    if(!props) {
      proc_err("ERROR parsing des file: ",1,strerror(errno));
      continue;//error init this obj, skip to next object
    }
    program_vars->nobj_props[nobj_id].nobj_id=nobj_id;//probably should be moved into parse_nobj_file()

    init_nobj(nobj_id,props,program_vars->nobj_props[nobj_id],&(program_vars->nobjs));
    //display_neur_props(nobj_id,nobjs,nobj_props);
    //free_nobj(nobj_id,nobj_props[nobj_id],&nobjs);

    strcpy(file,program_vars->file_without_ext);
    strcat(file,program_vars->con_extension);
    unsigned int **con_props = parse_con_file(file,&nobj_props[nobj_id]);
    nobj_props[nobj_id].nobj_id=nobj_id;
    if(con_props==NULL) {
      proc_err("ERROR parsing con file.\n",1);
      exit(-1);
    }
    init_cons(nobj_id, con_props, nobj_props[nobj_id], &(program_vars->cons), &(program_vars->conids), &(program_vars->weights));
    //display_con_props(nobj_id,cons,conids,weights,nobj_props);

    strcpy(file,file_without_ext);
    strcat(file,program_vars->var_extension);
    double **var_props = parse_vars_file(file,&(program_vars->nobj_props[nobj_id]));
    if(con_props==NULL) {
       proc_err("ERROR parsing var file.\n",1);
      exit(-1);
    }
    init_vars(nobj_id,var_props,nobj_props[nobj_id],&(program_vars->nvar));
    //display_vars_props(nvar[nobj_id],nobj_props[nobj_id]);

    strcpy(file,file_without_ext);
    strcat(file,".in");

    parse_i_file(file,&i_maps[nobj_id],&nobj_props[nobj_id]);
 
     /* base values for sim param fro nobj TODO[10] - rethink this design TODO - */
    (*param[nobj_id]).bp=&(program_vars->behaviors);
    (*param[nobj_id]).nobj=program_vars->nobjs[nobj_id];
    (*param[nobj_id]).cons=program_vars->cons[nobj_id];
    (*param[nobj_id]).conids=program_vars->conids[nobj_id];
    (*param[nobj_id]).weights=program_vars->weights[nobj_id];
    (*param[nobj_id]).vars=program_vars->nvar[nobj_id];
    (*param[nobj_id]).nobj_props=&(program_vars->nobj_props[nobj_id]);   
    (*param[nobj_id]).neur_from=UINT_MAX;//When stim from anything but another nuer, from neur_id = max usigned int
    (*param[nobj_id]).conid=UINT_MAX;
  }

  int i =0,j=0; //temp loop/index vars
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
   env_data->num_of_objs =vars->num_of_objs;

  /*
    init the env side, this should call hook_env
    send the control structure, the data structure and the env_id
  */
  envs[0].num_of_istream=3;
  int *state = malloc(sizeof(int));//sent to env

  if (init_env_oai(&envs[0],&env_data[0],0) != 0) {
     proc_err("main: error with initilization of environments.",1);
    exit(-2);
  }
  if(init_errors!=0){
     proc_err("main: error with initilization: %d",1,init_errors);
    exit(-1);
  }
  pthread_create(&env_t,NULL,main_loop_oai,state);//start env thread

  int cur_obj_id=0;
  j=0;
  i =0;
  int loop_count=0;
  int errs = 0;

  while(!control->halt) {
    for(cur_obj_id=0;cur_obj_id<num_of_objs;++cur_obj_id){//obj loop
      if(control->halt){
         proc_err("\nHalting via control\n",0);
        return;
      }
      loop_count++;
      //printf("MLC:%d\n",loop_count);
      //printf(" Main: Obj loop %d\n  streams %d\n",i,nobj_props[cur_obj_id].num_of_istreams);python
      for(j=0;j<nobj_props[cur_obj_id].num_of_istreams;++j){//loop each stream foreach object
        param[cur_obj_id]->neur_to=i_maps[cur_obj_id][j].neur_to;
        //get input to objects and process them
        errs = get_istream(i_maps[cur_obj_id][j].env_id, i_maps[cur_obj_id][j].stream_id,&(*param[cur_obj_id]).stim);
        if(errs==0){
          //printf("  istream value for [%u]: %lf for stream_id: %d\n",param[cur_obj_id]->neur_to,(*param[cur_obj_id]).stim,i_maps[cur_obj_id][j].stream_id);
          manager(param[cur_obj_id]);//drop work into thread pool
        } else {
          fprintf(stderr,"Error with gettng istreams: err#%d\n",errs);
        }



      }//end stream loop
    }//end obj loop

    /*
      future point of contentsion with nobj_ids not matching with env_obj_ids
    */
    double *sum=malloc(num_of_objs * sizeof(double));
    for(j=0;j<num_of_environments;++j){//loop each environment
      sum[i]=0;
      for(i=0;i<env_data[j].num_of_objs;++i){ //loop through each object registered in that environment
        sum[i]+=env_data[j].util[i];
        proc_err("UTIL: | %lf |",env_data[j].util[i],4);
      }
    }
     proc_err("\n",4);
    for(i=0;i<num_of_objs;++i) {//sum util
      nobj_sums[i].util=sum[i];
    }
    if(control->test && loop_count>=1000) {
      proc_err("End test loop. Exiting....\n",4);
      exit(0);//todo peform actual testing
    }
    //printf("==waiting==%d\n",loop_count);
    usleep(period_wait);

  }//main loop
printf("END");

  wait_for_threads();
  return;

}
