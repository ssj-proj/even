#include <stdio.h>
#include <stdlib.h>
#include "nobj.h"


//initilizes object at index no with non neurs with props[] properties

void init_nobj(int no, int non, unsigned int** props, struct nobj_meta obj_prop, unsigned int ****nobj) {
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
      printf(" trying to load prop %u\n",props[i][j]);
      ((*nobj)[no][i][j])=props[i][j];
      printf("Loaded prop: %u\n",(*nobj)[no][i][j]);
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
