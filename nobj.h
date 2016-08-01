#ifndef NOBJ_H
#define NOBJ_H
#include <pthread.h>

struct nobj_meta{
  unsigned int num_of_neurs;
  unsigned int num_of_neur_properties;
  unsigned int num_of_cons;
  unsigned int num_of_con_properties;
  unsigned int num_of_var_properties;
};
struct lock_group {
   /*
      Address to a 2 dimension array, not like the other 3 dimension arrays that hold multiple objs
      address to { [nobj id][neur_d] }
   */
   pthread_mutex_t **weights_lock;
   pthread_mutex_t **cons_lock;
   pthread_mutex_t **conids_lock;
   pthread_mutex_t **vars_lock;
};
struct obj_group {
  //TBI

   /*
      Address to a 2 dimension array, not like the other 3 dimension arrays that hold multiple objs
      address to { [neur_id][property] }
   */
  unsigned int  ***nobj;
  unsigned int  ***cons;
  unsigned int  ***conids;
  double  ***weights;
  double  ***vars;
};


//got rid of weight func
/*
  pre,fire and post are of type behav, these meant to modify neur vars
  thresh determines what causes neur to fire
*/
typedef void (*behavior)(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,   unsigned int**nobj,unsigned int**cons,unsigned int**conids,double**weights,double**vars, struct nobj_meta *nobj_props/* struct lock_group *locks */) ;
typedef int (*threshhold)(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,
   unsigned int**nobj,unsigned int**cons,unsigned int**conids,double**weights,double**vars, struct nobj_meta *nobj_props/* struct lock_group *locks */);
struct behav_pool {
  behavior *behaviors;
  threshhold *threshholds;
};



//Reads file and generates prop array (read - File to read,Set - nobj meta dat:single object not 
//											array);
unsigned int ** parse_nobj_file(char * file, struct nobj_meta *nobj_props);
void init_nobj(int no, unsigned int **props,struct nobj_meta obj_prop, unsigned int ****nobj);
void free_nobj(int no, struct nobj_meta obj_prop, unsigned int ****nobj);
void display_neur_props(unsigned int**nobjs,struct nobj_meta np);

int init_cons(int no, unsigned int** con_props, struct nobj_meta obj_prop, unsigned int ****cons, unsigned int ****conids,double ****weights);
void free_nobj_con(int no, struct nobj_meta obj_prop, unsigned int ****cons, unsigned int ****conids,double ****weights);
unsigned int ** parse_con_file(char * file,struct nobj_meta *nobj_props);
void display_con_props(int obj,unsigned int***cons,unsigned int***conids,double***weights,struct nobj_meta *np);

double ** parse_vars_file(char * file, struct nobj_meta *nobj_props);
void init_vars(int no, double** props, struct nobj_meta obj_prop, double ****vars);
void free_vars(int no, struct nobj_meta obj_prop, double ****nvar);
void display_vars_props(double **vars,struct nobj_meta np);

//accepts address to a single obj , not the entire array
void init_locks(struct nobj_meta obj_prop, pthread_t **);

//nobject id, from, to, conid (index in weights array of receiving neur), stim
void stim(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim, struct behav_pool*,unsigned int**nobj,unsigned int**cons,unsigned int**conids, double**weights, double**vars, struct nobj_meta *nobj_props/* struct lock_group *locks */);

void fire_downstream(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim, struct behav_pool*,unsigned int**nobj,unsigned int**cons,unsigned int**conids, double**weights, double**vars, struct nobj_meta *nobj_props/* struct lock_group *locks */);


#endif
