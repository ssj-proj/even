#include <stdio.h>
#include <stdlib.h>
#include "nobj.h"
#include "behaviors.h"
#include "stimpool.h"
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
  struct stim_param **param = malloc(sizeof(*param)*num_of_objs);//used for thread distribution
   
  if(weights==NULL) {
    printf("Error: failed to malloc weights\n");
    exit(-1);
  }



  char *file;
  unsigned int **props;
  unsigned int nobj_id=0;

  for(nobj_id;nobj_id<num_of_objs;++nobj_id) {
    file="./obj/obj_0.des";
    printf("   INIT OBJECT %u\n",nobj_id);
    param[nobj_id] = malloc(sizeof(struct stim_param));

    props = parse_nobj_file(file,&nobj_props[nobj_id]);
    init_nobj(nobj_id,props,nobj_props[nobj_id],&nobjs);
    //display_neur_props(nobj_id,nobjs,nobj_props);
    //free_nobj(nobj_id,nobj_props[nobj_id],&nobjs);

    file="./obj/obj_0.con";
    unsigned int **con_props = parse_con_file(file,&nobj_props[nobj_id]);
    nobj_props[nobj_id].nobj_id=nobj_id;
    if(con_props==NULL) {
      printf("ERROR parsing con file.\n");
      exit(-1);
    }
    init_cons(nobj_id, con_props, nobj_props[nobj_id], &cons, &conids, &weights); 
    display_con_props(nobj_id,cons,conids,weights,nobj_props);

   file="./obj/obj_0.var";
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

  

 
  int i =0; 
  init_workers(num_of_objs);
  for(i;i<num_of_objs;++i){
  (*param[i]).neur_from=99;
  (*param[i]).neur_to=0;
  (*param[i]).conid=0;
  (*param[i]).stim=131;
  (*param[i]).bp=&behaviors;
  (*param[i]).nobj=nobjs[i];  
  (*param[i]).cons=cons[i];
  (*param[i]).conids=conids[i];
  (*param[i]).weights=weights[i];
  (*param[i]).vars=nvar[i];
  (*param[i]).nobj_props=&(nobj_props[i]);
  manager(param[i]);
  }
 
  //stim(&param);
  
  wait_for_threads();
  exit(0);

}


























