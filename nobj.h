#ifndef NOBJ_H
#define NOBJ_H

struct nobj_meta{
  unsigned int num_of_neurs;
  unsigned int num_of_neur_properties;
};
void parse_nobj_file(int *no, int *non, unsigned int prop);
void init_nobj(int no, int non, unsigned int **props,struct nobj_meta obj_prop, unsigned int ****nobj);
void display_neur_props(int obj,unsigned int***nobjs,struct nobj_meta *np);

#endif
