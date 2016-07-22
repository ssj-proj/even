#ifndef NOBJ_H
#define NOBJ_H

struct nobj_meta{
  unsigned int num_of_neurs;
  unsigned int num_of_neur_properties;
  unsigned int num_of_cons;
  unsigned int num_of_con_properties;
  unsigned int num_of_var_properties;
};
//got rid of weight func
/*
  pre,fire and post are of type behav, these meant to modify neur vars
  thresh determines what causes neur to fire
*/
typedef void (*behavior)(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,   unsigned int**nobj,unsigned int**cons,unsigned int**conids,double**weights,double**vars, struct nobj_meta nobj_props) ;
typedef int (*threshhold)(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,
   unsigned int**nobj,unsigned int**cons,unsigned int**conids,double**weights,double**vars, struct nobj_meta nobj_props);
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



//nobject id, from, to, conid (index in weights array of receiving neur), stim
void stim(unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim, struct behav_pool,unsigned int**nobj,unsigned int**cons,unsigned int**conids, double**weights, double**vars, struct nobj_meta nobj_props);


#endif
