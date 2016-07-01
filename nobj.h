#ifndef NOBJ_H
#define NOBJ_H

struct nobj_meta{
  unsigned int num_of_neurs;
  unsigned int num_of_neur_properties;
};
//Reads file and generates prop array (read - File to read,Set - nobj meta dat:single object not 
//											array);
unsigned int ** parse_nobj_file(char * file, struct nobj_meta *nobj_props);
void init_nobj(int no, unsigned int **props,struct nobj_meta obj_prop, unsigned int ****nobj);
void free_nobj(int no, struct nobj_meta obj_prop, unsigned int ****nobj);
void display_neur_props(int obj,unsigned int***nobjs,struct nobj_meta *np);


/*Connection  TBI*/
unsigned int ** parse_con_file(char *file, struct nobj_meta nobj_props);
void stim(long conid, double stim);
#endif
