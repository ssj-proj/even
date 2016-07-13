#include <stdio.h>
#include <stdlib.h>
#include "nobj.h"
typedef int bool;
#define true 1
#define false 0


unsigned int ** parse_nobj_file(char * file, struct nobj_meta *nobj_props) {
  FILE *fp = fopen(file,"r");
  char buff[255];
  int t;
  bool has_neur_settings=false;
  bool has_neur_count=false;
  int max=10;
  int i =0;
  /* Loops until all meta data is read setting:\s val */
  while(!(has_neur_settings&&has_neur_count)) {
    i++;
    //printf("bools %d   and   %d",has_neur_settings,has_neur_count);
    fscanf(fp,"%[^:]:%u ",buff,&t);
    printf(" READ (%s) %s %d\n",file,buff,t);
    if(strcmp(buff,"non")==0) {
      (*nobj_props).num_of_neurs=t;
      has_neur_count=true;
    } else if(strcmp(buff,"np")==0){
      (*nobj_props).num_of_neur_properties=t;
      has_neur_settings=true;
    } else {
      printf("Unkown setting %s in %s\n",buff,file);
    }

    if(i>=max) { 
       printf("ERROR READING %s\n",file);
       fclose(fp);
       return NULL;
    }
  }
  
  /* Read all neurs  */
  unsigned int**props=malloc(sizeof(unsigned int*) * (*nobj_props).num_of_neurs);
  int n =0;
  for(n;n<(*nobj_props).num_of_neurs;++n) {
    int p = 0;
    props[n]=malloc(sizeof(unsigned int) * (*nobj_props).num_of_neur_properties);
    for(p;p<(*nobj_props).num_of_neur_properties;++p) {
      if(fscanf(fp,"%u%*[, \t\n]",&props[n][p])<1) {
        printf("ERROR READING %s : MISSING AT LEAST 1 NEUR PROPERTY\n",file);
        fclose(fp);
        return NULL;
      }
      //printf("  props:%u\n",props[n][p]);
    }
  }
  fclose(fp);
  return props;
}
//initilizes(inserts) object at index no with non neurs with props[] properties
void init_nobj(int no, unsigned int** props, struct nobj_meta obj_prop, unsigned int ****nobj) {
  (*nobj)[no]=malloc(sizeof(unsigned int*)*obj_prop.num_of_neurs); //initilize obj's array of neurs

  if( !(*nobj)[no]) {
    printf("ERROR - failed to malloc nobj\n");
    return;
  }

  int i = 0;
  for(i;i<obj_prop.num_of_neurs;++i) {
    (*nobj)[no][i]=malloc(sizeof(unsigned int)*obj_prop.num_of_neur_properties);
    
    int j = 0;
    for(j;j<obj_prop.num_of_neur_properties;++j) {
      
      ((*nobj)[no][i][j])=props[i][j];
     
    }
  }
}
void free_nobj(int no, struct nobj_meta obj_prop, unsigned int ****nobj){
 
  int i = 0;
  for(i;i<obj_prop.num_of_neurs;++i) {
    free((*nobj)[no][i]);
  }
  free((*nobj)[no]);

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
void stim(int no,unsigned int neur_from, unsigned int neur_to, unsigned int conid, double stim) {
//pre
//weight
//thresh
//fire
//post

}

