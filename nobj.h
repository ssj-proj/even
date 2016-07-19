#ifndef NOBJ_H
#define NOBJ_H

struct nobj_meta{
  unsigned int num_of_neurs;
  unsigned int num_of_neur_properties;
  unsigned int num_of_cons;
  unsigned int num_of_con_properties;
};
//got rid of weight func
/*
  pre,fire and post are of type behav, these meant to modify neur vars
  thresh determines what causes neur to fire
*/
typedef void (*behavior)(int no,unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,   unsigned int***nobj,unsigned int***cons,unsigned int***conids,double***weights,double***vars) ;
typedef int (*threshold)(int no,unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,
   unsigned int***nobj,unsigned int***cons,unsigned int***conids,double***weights,double***vars);
struct behav_pool {
  behavior *behaviors;
  threshold *threshholds;
};



//Reads file and generates prop array (read - File to read,Set - nobj meta dat:single object not 
//											array);
unsigned int ** parse_nobj_file(char * file, struct nobj_meta *nobj_props);
void init_nobj(int no, unsigned int **props,struct nobj_meta obj_prop, unsigned int ****nobj);
void free_nobj(int no, struct nobj_meta obj_prop, unsigned int ****nobj);
void display_neur_props(int obj,unsigned int***nobjs,struct nobj_meta *np);

int init_cons(int no, unsigned int** con_props, struct nobj_meta obj_prop, unsigned int ****cons, unsigned int ****conids,double ****weights);
unsigned int ** parse_con_file(char * file,struct nobj_meta *nobj_props);


//nobject id, from, to, conid (index in weights array of receiving neur), stim
void stim(int no,unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim, struct behav_pool,unsigned int***nobj,unsigned int***cons,unsigned int***conids, double***weights, double***vars);

void init_nobj_connections();
void free_nobj_connections();

#endif
