/*
  Contains functions related to workings of individual neurs
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "nobj.h"
#include <pthread.h>
#include "env_api.h"
#include <errno.h>

typedef int bool;
#define true 1
#define false 0

unsigned int ** parse_nobj_file(char * file, struct nobj_meta *nobj_props) {

  printf("INFO: trying to open %s\n", file);
  FILE *fp = fopen(file,"r");
  if(!fp) {
  printf("ERROR: unable to open %s : %s\n", file, strerror(errno));
  exit(-1);
  return NULL;

  }
  char buff[255];
  int t;
  bool has_neur_settings=false;
  bool has_neur_count=false;
  int max=10;
  int i =0;
  /* Loops until all meta data is read setting:\s val */
  while(!(has_neur_settings&&has_neur_count)) {
    i++;
    printf("bools %d   and   %d",has_neur_settings,has_neur_count);
    fscanf(fp,"%[^:]:%u ",buff,&t);//reads string:value into buff:t
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
void free_nobj_con(int no, struct nobj_meta obj_prop, unsigned int ****cons, unsigned int ****conids,double ****weights){

  int i = 0;
  for(i;i<obj_prop.num_of_neurs;++i) {
    free((*cons)[no][i]);
    free((*conids)[no][i]);
    free((*weights)[no][i]);
  }
  free((*cons)[no]);
  free((*conids)[no]);
  free((*weights)[no]);

}

unsigned int ** parse_con_file(char * file,struct nobj_meta *nobj_props) {
  FILE *fp = fopen(file,"r");
  if(!fp) {
    printf("ERROR: unable to open %s : %s", file, strerror(errno));

  }

  char buff[255];
  unsigned int t;//temp var for reading header settings

  bool has_con_count=false;
  bool has_con_properties=false;
  int max_header_lines=10;
  int i =0;
  /* Loops until all meta data is read setting:\s val */
  while(!(has_con_count&&has_con_properties)) {
    i++;
    //printf("bools %d   and   %d",has_neur_settings,has_neur_count);
    fscanf(fp,"%[^:]:%u ",buff,&t);
    printf(" READ (%s) %s %d\n",file,buff,t);

    if(strcmp(buff,"con")==0) {
      (*nobj_props).num_of_cons=t;
       has_con_count=true;

    } else if(strcmp(buff,"cp")==0) {
      (*nobj_props).num_of_con_properties=t;
      has_con_properties=true;

    }else {
      printf("Unkown setting %s in %s\n",buff,file);
    }

    if(i==max_header_lines) {
       printf("ERROR READING %s\n",file);
       fclose(fp);
       return NULL;
    }
  }

  /* Read all neurs  */
  unsigned int**cons=malloc(sizeof(unsigned int*) * (*nobj_props).num_of_cons);
  unsigned int n =0;
  for(n;n<(*nobj_props).num_of_cons;++n) {//loop every con (line) for object
    int p = 0;
    cons[n]=malloc(sizeof(unsigned int) * (*nobj_props).num_of_con_properties);
    for(p;p<(*nobj_props).num_of_con_properties;++p) {//loop every property (column) for each con
      //put an unsigned int into the corresping index
      //should be from,to,weight
      if(fscanf(fp,"%u%*[, \t\n]",&cons[n][p])<1) {
        printf("ERROR READING %s : MISSING AT LEAST 1 con\n",file);
        fclose(fp);
        return NULL;
      }else{}
      //printf("  cons[%u] prop val:%u\n",n,cons[n][p]);
    }
     if(cons[n][0]>(*nobj_props).num_of_neurs-1 || cons[n][1]>(*nobj_props).num_of_neurs-1 ) {
        printf("ERROR Reading con file, reference to invalid neur: from[%u] or to[%u]\n",cons[n][0],cons[n][1]);
        return NULL;
      }

  }
  fclose(fp);


  //returns array representing parse file
  //should be cons[neur_id][property]
  //with property 0,1,2 equaling from,to,weight respectively
  return cons;
}
int init_cons(int no, unsigned int** con_props, struct nobj_meta obj_prop, unsigned int ****cons, unsigned int ****conids,double ****weights) {

  if(weights[no]==NULL) {
    printf("Error: WEIGHTS ARRAY IS NULL\n");
    exit(-1);
  }
  (*cons)[no]   =malloc(sizeof(  unsigned int*)*obj_prop.num_of_cons); //initilize obj's array of neurs
  (*conids)[no] =malloc(sizeof(  unsigned int*)*obj_prop.num_of_cons);
  (*weights)[no]=malloc(sizeof(        double*)*obj_prop.num_of_cons);

  if( !((*cons)[no]||(*conids)[no]||(*weights)[no]) ) {
    printf("ERROR - failed to malloc at least 1 of 3 con arrays\n");
    return -1;
  }
  unsigned int num_of_cons[obj_prop.num_of_neurs];//holds total neur count
  unsigned int num_of_cons_rec[obj_prop.num_of_neurs];//holds total receiving cons
  unsigned int num_of_cons_ass[obj_prop.num_of_neurs];//tmp used for array init, holds amount assigned
  unsigned int nocw[obj_prop.num_of_neurs];//tmp used for indexing - numc of cons weight
  unsigned int i = 0;
  //init array to 0
  for(i;i<obj_prop.num_of_neurs;++i) {
    //these arrays used to to track the size of other arrays
    //init to 1 since first element on cons,conids,weights array store legnth of array
    //total array length is (array[0]+1)
    num_of_cons[i]=0;
    num_of_cons_ass[i]=1;
    num_of_cons_rec[i]=0;
    nocw[i]=1;
  }


  i = 0;
  //tally array members
  for(i;i<obj_prop.num_of_cons;++i) {
    if(con_props[i][0]<obj_prop.num_of_neurs) {//make sure its not an invalid indexx
      num_of_cons[con_props[i][0]]++;
      num_of_cons_rec[con_props[i][1]]++;
    } else {
      printf("ERROR LOADING CON FILE, REFERENCE TO NONEXISTENT NEUR\n");
      return -1;
    }
  }
  i=0;;
  //allocate space for each array for each neur for the num of cons each neur has
  for(i;i<obj_prop.num_of_neurs;++i) {
    (*cons)[no][i]=malloc(sizeof(unsigned int)* (num_of_cons[i]+1));
    (*cons)[no][i][0]=num_of_cons[i];//first element in array specifies number of cons
    (*conids)[no][i]=malloc(sizeof(unsigned int)* (num_of_cons[i]+1));
    (*conids)[no][i][0]=num_of_cons[i];//first element in array specifies number of cons
    (*weights)[no][i]=malloc(sizeof(double)* (num_of_cons_rec[i]+1));
    (*weights)[no][i][0]=num_of_cons_rec[i];//first element in array specifies number of weights
    //printf("malloc for nobj %u for receiving nuer %u , rec count %u\n",no,i,num_of_cons_rec[i]);
  }


  i=0;
  for(i;i<obj_prop.num_of_cons;++i) {
    //source neur holds con array and conid array
    //con array holds neur_id of targets
    //conids holds the index of the targets weight array that corresponds to this connection
    ((*cons   )[no][con_props[i][0]][ num_of_cons_ass[con_props[i][0]] ]) = con_props[i][1];
    ((*conids )[no][con_props[i][0]][ num_of_cons_ass[con_props[i][0]] ]) = nocw[con_props[i][1]];

    ((*weights)[no][con_props[i][1]][ nocw           [con_props[i][1]] ]) = (double)(con_props[i][2]/100.0);


    //printf("  NEUR %u , CON[%u] , TO %u\n",con_props[i][0],(num_of_cons_ass[con_props[i][0]]),con_props[i][1]);
    //printf("-----CONID: %u\n",nocw[con_props[i][1]] );
    //printf(" ----WEIGHT: %lf\n",((*weights)[no][con_props[i][1]][ nocw[con_props[i][1]] ]));
    num_of_cons_ass[con_props[i][0]]++;//incrementr num of cons sender has in con array
    nocw[con_props[i][1]]++;//increment index for target neur weight array
  }

}

//uses the 'To con' in the stim_param as the source
void add_ocon(struct stim_param *sp, int num_of) {
  num_of = 1;
  unsigned int i =0;
	//to neur
	//find targets
  unsigned int connect_to[num_of];//neur id of new targets
	unsigned int current_size = sp->cons[sp->neur_to][0];//get total number of connections

  for(i;i<num_of;++i) {
    connect_to[i]=2;//TODO some algo to randomly chose neur weight for closeness with neurs represented in cube
  }
  unsigned int new_size = sp->cons[sp->neur_to][0]+num_of;
  //allocate array for single neur with size = to the number of current size + additional
  unsigned int * new_con_array = malloc(sizeof(unsigned int)* (current_size+num_of))
  //copy old array to nuew array
  memcpy(new_con_array,sp->cons[sp->neur_to],sizeof(unsigned int)*current_size );
  //change first value to reflect size of array
  new_con_array[0]=new_size;

  //assign new values to new source con array
  for(i=0;i<new_size;++i) {
    new_con_array[i]=connect_to[i-current_size];
  }

	//malloc new conids array for TARGET of new size
	//copy array
	//add new value
	//change first value to reflect size of array

	//malloc new weights array for TARGET of new size
	//copy array
	//add new value
	//change first value to reflect size of array

  //assign conids array to target new array
  //assign weights array to target new array
  //assign cons array to source new array

}
void display_con_props(int obj,unsigned int***cons,unsigned int***conids,double***weights,struct nobj_meta *np) {
  unsigned int i =0,j=1;
  printf("OBJ[%d]\n",obj);
  printf("  Total cons: %u\n",np[obj].num_of_cons);

  for(i;i<np[obj].num_of_neurs;++i) {
    j=1;
    printf("  Neur[%u] num of cons:%u\n",i,cons[obj][i][0]);

    for(j;j<cons[obj][i][0]+1;++j){
      printf("  Neur[%u] conto:%u\n",i,cons[obj][i][j]);
      printf("      conid: %u\n"    ,conids[obj][i][j]);
      printf("      weight:%lf\n"   ,weights[obj][cons[obj][i][j]][ conids[obj][i][j] ]  );
    }
  }
}
void display_neur_props(unsigned int**nobjs,struct nobj_meta np) {
  unsigned int i =0,j=0;
  printf("  Total neurs: %u\n",np.num_of_neurs);

  for(i;i<np.num_of_neurs;++i) {
    j=0;
    for(j;j<np.num_of_neur_properties;++j){
      printf("  Neur[%u] prop[%u] val:%u\n",i,j,nobjs[i][j]);

    }
  }
}

double ** parse_vars_file(char * file, struct nobj_meta *nobj_props) {
  FILE *fp = fopen(file,"r");
  if(!fp) {
    printf("ERROR: unable to open %s : %s", file, strerror(errno));

  }
  char buff[255];
  int t;
  bool has_var_settings=false;
  bool has_neur_count=false;
  int max=10;
  int i =0;
  /* Loops until all meta data is read setting:\s val */
  while(!(has_var_settings&&has_neur_count)) {
    i++;
    //printf("bools %d   and   %d",has_neur_settings,has_neur_count);
    fscanf(fp,"%[^:]:%u ",buff,&t);
    printf(" READ (%s) %s %d\n",file,buff,t);
    if(strcmp(buff,"non")==0) {
      has_neur_count=true;
    } else if(strcmp(buff,"vp")==0){
      (*nobj_props).num_of_var_properties=t;
      has_var_settings=true;
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
  double**props=malloc(sizeof(props) * (*nobj_props).num_of_neurs);
  int n =0;
  for(n;n<(*nobj_props).num_of_neurs;++n) {
    int p = 0;
    props[n]=malloc(sizeof(*props) * (*nobj_props).num_of_var_properties);
    for(p;p<(*nobj_props).num_of_var_properties;++p) {
      if(fscanf(fp,"%lf%*[, \t\n]",&props[n][p])<1) {//convert string to double
        printf("ERROR READING %s : MISSING AT LEAST 1 NEUR PROPERTY\n",file);
        fclose(fp);
        return NULL;
      }
      //printf(" VAR:%lf\n",props[n][p]);
    }
  }
  fclose(fp);
  return props;
}

void init_vars(int no, double** props, struct nobj_meta obj_prop, double ****vars) {
  (*vars)[no]=malloc(sizeof(double**)*obj_prop.num_of_neurs); //initilize obj's array of neurs

  if((*vars)[no]==NULL) {
    printf("ERROR - failed to malloc nobj\n");
    return;
  }

  unsigned int i = 0;
  printf(" I vs non : %u v %u\n",i,obj_prop.num_of_neurs);
  for(i;i<obj_prop.num_of_neurs;++i) {
    (*vars)[no][i]=malloc(sizeof(double*)*obj_prop.num_of_var_properties);
    //printf(":::--------:%u\n",obj_prop.num_of_var_properties);
    int j = 0;
    for(j;j<obj_prop.num_of_var_properties;++j) {
      ((*vars)[no][i][j])=props[i][j];
      //printf(" varp:%lf\n",((*vars)[no][i][j]));
    }
    if( (*vars)[no][i][2] == 0) {
      fprintf(stderr,"nobj:init_vars:fire_strength = 0; exiting...\n");
      fprintf(stderr,"    nobj:init_vars: obj id[%u] neur[%u]",no,i);
      exit(0);
    }
    printf("nobj:init_vars:obj %d , neur %u , firestrength %lf, depreciation %lf\n",no,i,(*vars)[no][i][2],(*vars)[no][i][6]);
  }


}
void free_vars(int no, struct nobj_meta obj_prop, double ****vars){

  int i = 0;
  for(i;i<obj_prop.num_of_neurs;++i) {
    free((*vars)[no][i]);
  }
  free((*vars)[no]);
}
void display_vars_props(double**vars,struct nobj_meta np) {
  unsigned int i =0,j=0;

  printf("  Total neurs: %u\n",np.num_of_neurs);

  for(i;i<np.num_of_neurs;++i) {
    j=0;
    for(j;j<np.num_of_var_properties;++j){
      printf("  Neur[%u] var[%u] val:%lf\n",i,j,vars[i][j]);

    }
  }
}
int verify_obj_vars(double **vars,struct nobj_meta np) {
  unsigned int i =0,j=0;
  int err=0;

  for(i;i<np.num_of_neurs;++i) {
    j=0;
    if(vars[i][0]>vars[i][1]) {
      printf("Stim or thresh may be invalid\n");
      err++;
    }
    if(vars[i][2]==0){
      printf("fire_strength may be invalid or dead neur\n");
      err++;
    }
    if(vars[i][4]<0){
      printf("invalid env_id\n");
      err++;
    }
    if(vars[i][5]<0){
      printf("invalid stream_id\n");
      err++;
    }
  }

  return err;

}
/*
  must init istream_client array before running this
  must know number of environments
  must know number of streams in each environmnet
  this can init clients portion of that array


*/
int parse_i_file(char * file, struct i_map **im, struct nobj_meta *nobj_props) {
  printf("PARSE I FILE %s\n",file);

  FILE *fp;
  fp=fopen(file,"rt");
  if(!fp) {
    perror("Error opening file");
  }
  int buff_size=256;
  char *buff=malloc(buff_size);

  int num_of_inputs=0;
  int max_head=1,i=0;//only check for this num of header lines
  char found=0;
  while(found<max_head&&i<100) {

    printf("HERE\n");
    if(fgets(buff,buff_size,fp)!= NULL){
      printf("HERE2\n");
      if(sscanf(buff,"num_of_inputs: %d",&num_of_inputs) > 0) {
       found++;
      }
      if(i==max_head) { //have max number of headers, exit loop
        break;
      }
      i++;
    }
  }
  printf("Number of inputs %d\n", num_of_inputs);//Complete this func
  nobj_props->num_of_istreams = num_of_inputs;
  (*im) =malloc( num_of_inputs * sizeof(**im));
  /*
     parse body file, format of:
       neur_id, env_id,stream_id
  */
  i=0;
  char *delim=",";
  char *val=NULL;// = malloc(256);
  unsigned int neur_to;
  int env_id, stream_id;
  for(i;i<num_of_inputs;++i) {
    if(fgets(buff,buff_size,fp) != NULL) {
      val = strtok(buff,delim);
      sscanf(val,"%u",&neur_to);

      val = strtok(NULL,delim);
      sscanf(val,"%u",&env_id);

      val = strtok(NULL, delim);
      sscanf(val,"%u",&stream_id);
      (*im)->env_id=env_id;
      (*im)->stream_id=stream_id;
      (*im)->neur_to=neur_to;
      printf("to neur %u, from env %d from stream %d\n", neur_to,env_id,stream_id);
    }

  }
}
void init_io(int no, double** props, struct nobj_meta obj_prop, double ****vars);//tbi
void free_io(int no, struct nobj_meta obj_prop, double ****nvar);//tbi
void display_io_props(double **vars,struct nobj_meta np);


void copy_stim_param(struct stim_param from, struct stim_param *to) {
  (*to).neur_from=from.neur_from;
  (*to).neur_to=from.neur_to;
  (*to).conid=from.conid;
  (*to).stim=from.stim;
  (*to).bp=from.bp;
  (*to).nobj=from.nobj;
  (*to).cons=from.cons;
  (*to).conids=from.conids;
  (*to).weights=from.weights;
  (*to).vars=from.vars;
  (*to).nobj_props=from.nobj_props;

}

/* Stim func - the neur_to is the one this is executing this func */
void stim(struct stim_param *sp) {
      if( sp->vars[sp->neur_to][2] == 0) {
      fprintf(stderr,"nobj%u:stim:neur%u:fire_strength = 0\n",sp->nobj_props->nobj_id,sp->neur_to);

    }


  //printf("behviors index: %u\n",(*sp).nobj[(*sp).neur_to][0]);
  (*sp->bp).behaviors[ sp->nobj[sp->neur_to][0]  ](sp);//PRE
  //printf("weight: %lf\n",(*sp).weights[(*sp).neur_to][(*sp).conid]);

  /*
    if conid is max unint - don't multiply by weight. Max unint is phantom input (input not from another neur). Will be re-engineerd so all Input has registered weight, requires the growth/shrinking of a couple arrays to add/remove cons
  */
  if(sp->conid != UINT_MAX) {
    (*sp).vars[(*sp).neur_to][0]+= ((*sp).stim * (*sp).weights[(*sp).neur_to][(*sp).conid] );

  }
  else {
    sp->vars[sp->neur_to][0]+= sp->stim;
   // printf("  OBJ[%u] STIM:%lf!!!!!!!\n",sp->nobj_props->nobj_id,sp->vars[sp->neur_to][0]);
   }



  //Thresh
  if(  (*sp->bp).threshholds[ sp->nobj[sp->neur_to][1]  ](sp) == 0 ) {
    //printf("  OBJ[%u] FIRE!!!!!!!!!!\n",sp->nobj_props->nobj_id);
    fire_downstream(sp);
  } else { //POST
    (*(*sp).bp).behaviors[ (*sp).nobj[(*sp).neur_to][3]  ](sp);
  }
}
void fire_downstream(struct stim_param *sp) {

//set_output(int nobj_id,int stream_id,double data, struct env_control *client_work)
//need env_id, stream_id, nobj-id, pointer to work struct

  int i;
  sp->neur_from = sp->neur_to;//last stimmed neur is now firing to
  sp->stim= sp->vars[sp->neur_from][2];//set outgoing stim to neur strength
  if(sp->stim==0){
    printf("nobj:Neur strength 0, non-functioning nuer.obj:%u,neur:%u\n",sp->nobj_props->nobj_id,sp->neur_from);

  }
  int num_to_send=sp->cons[sp->neur_from][0];
  //printf("  OBJ[%u] setting output\n",sp->nobj_props->nobj_id);
  int t = (int)(sp->vars[sp->neur_from][4]);
  if(set_output(sp->nobj_props->nobj_id,sp->vars[sp->neur_from][5],
    sp->stim,(int)(sp->vars[sp->neur_from][4])  ) != 0) {

    char err_buff[100];
    sprintf(err_buff,"  nobj[%u]:fire_downstream:Failure to set_out to env_id[%lf] stream[%lf]\n",sp->nobj_props->nobj_id,sp->vars[sp->neur_from][4],(sp->vars[sp->neur_from][5]));
    proc_err(err_buff,2);
    if(t!=(int)(sp->vars[sp->neur_from][4])){
      proc_err("ERROR:nobj.c:ENV got corrupt!",1);
    }
  }
/*

*/

  //iterate from index 1 to index 1+num_to_send - first index holds length of array
  for(i=1; i < num_to_send+1;++i) {
     sp->conid=sp->conids[sp->neur_from][i];
     sp->neur_to=sp->cons[sp->neur_from][i];
     char err_buff[100];
    sprintf(err_buff,"DEBUG:nobj.c:fire from/to %u/%u data:%lf\n",sp->neur_from,sp->neur_to,sp->stim);
    proc_err(err_buff,5);
    manager(sp);//put work on top of work queue
  }
}
//int check_nobj_props(struct nobj_meta props);
//void stim_from_env(int obj_id, unsigned int neur_id, double dat);
