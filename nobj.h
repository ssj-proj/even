#ifndef NOBJ_H
#define NOBJ_H

struct nobj_meta{
  unsigned int num_of_neurs;
  unsigned int num_of_neur_properties;
};
//got rid of weight func
/*
  pre,fire and post are of type behav, these meant to modify neur vars
  thresh determines what causes neur to fire
*/
void (*behav)(int no,unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,
   unsigned int***nobj,unsigned int***cons,unsigned int***conids,double***weights,double***vars);
bool (*thresh)(int no,unsigned int neur_from, unsigned int neur_to, unsigned int conid,
  double stim, unsigned int***nobj,unsigned int***cons, unsigned int***conids, double***weights,
  double***vars );


//Reads file and generates prop array (read - File to read,Set - nobj meta dat:single object not 
//											array);
unsigned int ** parse_nobj_file(char * file, struct nobj_meta *nobj_props);
void init_nobj(int no, unsigned int **props,struct nobj_meta obj_prop, unsigned int ****nobj);
void free_nobj(int no, struct nobj_meta obj_prop, unsigned int ****nobj);
void display_neur_props(int obj,unsigned int***nobjs,struct nobj_meta *np);


/*Connection  TBI*/
unsigned int ** parse_con_file(char *file, struct nobj_meta nobj_props);
//nobject id, from, to, conid (index in weights array of receiving neur), stim
void stim(int no,unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim,
   unsigned int***nobj,unsigned int***cons,unsigned int***conids,double***weights,double***vars);

void init_nobj_connections();
void free_nobj_connections();

#endif
