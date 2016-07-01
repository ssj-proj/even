#include <stdio.h>
#include <stdlib.h>
#include "nobj.h"


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
  //[object id][neur id][neur propert id]
  unsigned int ***nobjs;
  //[object id][neur id][to con neur id]
  unsigned int ***cons;
  struct nobj_meta *nobj_props;


  nobj_props=malloc(num_of_objs * sizeof(struct nobj_meta));
  nobjs=malloc( num_of_objs *  sizeof(unsigned int**));
  

  nobj_props[0].num_of_neurs=3;
  nobj_props[0].num_of_neur_properties=6;

  unsigned int **props=create_props(3,6);

  init_nobj(0,nobj_props[0].num_of_neurs,props,nobj_props[0],&nobjs);
  display_neur_props(0,nobjs,nobj_props);

  exit(0);

}


























