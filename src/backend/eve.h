#ifndef EVE_H
#define EVE_H
struct main_control {
  int test;//opt
  int num_of_objs;//req
  int loop_iteration;
  int num_of_threads;//req must at least be one
  int halt;//req to be set to 0 at first
  char log_file[255];//opt
  int log_verbosity;//opt
  int screen_verbosity;//opt
  int console_mode;

  char obj_file_base[255];//req
  char des_extension[5];//req
  char con_extension[5];//req
  char var_extension[5];//req
};
//used to send info/back/forth to front end
struct nobj_summary {
  double util;//sum of util from all envs
  //if set 1 -> being actively used if set to 0 -> skipped. Index in array could be re-used for future objects
  int active;
};
void *start_program(void *control_ptr);
struct main_control *create_control(void);
double get_obj_util(int obj_id);//called by front end to get util of an obj

#endif
