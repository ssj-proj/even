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
  char *file="./obj/obj_0.des";
  unsigned int **props;


  unsigned int nobj_id=1;

  for(nobj_id;nobj_id<num_of_objs;++nobj_id) {

    /* LOH - get seg fault when doing any object index > 0 */
    print("   INIt OBJECT %u",nobj_id);
    props = parse_nobj_file(file,&nobj_props[nobj_id]);
    init_nobj(nobj_id,props,nobj_props[nobj_id],&nobjs);
    display_neur_props(nobj_id,nobjs,nobj_props);
    free_nobj(nobj_id,nobj_props[nobj_id],&nobjs);
  }

  exit(0);

}


























