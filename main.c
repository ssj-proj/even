#include <stdio.h>
#include <stdlib.h>





//holds all the objs and the neur properties

struct nobj_meta{
  unsigned int num_of_neurs;
  unsigned int num_of_neur_properties;
};

//initilizes object at index no with non neurs with props[] properties
void init_nobj(int no, int non, unsigned int props[][6],struct nobj_meta obj_prop, unsigned int ****nobj) {
  //printf("init_nobj1\n");
  (*nobj[no])=malloc(sizeof(unsigned int*)*non); //initilize obj's array of neurs
  //printf("init_nobj2\n");
  int i = 0;
  for(i;i<non;++i) {
   // printf("init loop here: %d",i);
    (*nobj)[no][i]=malloc(sizeof(unsigned int)*obj_prop.num_of_neur_properties);
    //printf("init_nobj2\n");
    int j = 0;
    for(j;j<obj_prop.num_of_neur_properties;++j) {
     //printf(" trying to load prop %u\n",props[i][j]);
      ((*nobj)[no][i][j])=props[i][j];
      //printf("Loaded prop: %u\n",(*nobj)[no][i][j]);
    }
  }
}
void display_neur_props(int obj,unsigned int***nobjs,struct nobj_meta *np) {
  unsigned int i =0,j=0;
  printf("OBJ[%d]\n",obj);
  printf("  Total neurs: %u\n",np[obj].num_of_neurs);

  for(i;i<np[obj].num_of_neurs;++i) {
    j=0;
    for(j;j<np[obj].num_of_neur_properties;++j){
      printf("  Neur[%u] prop[%u] val:%u\n",i,j,nobjs[obj][i][j]);
    }
  }


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
  unsigned int props[3][6]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};


  init_nobj(0,nobj_props[0].num_of_neurs,props,nobj_props[0],&nobjs);
  display_neur_props(0,nobjs,nobj_props);

}


























