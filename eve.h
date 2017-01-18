#ifndef EVE_H
#define EVE_H
struct main_control {
  int num_of_objs;//req
  int loop_iteration;
  int num_of_threads;//req
  int halt;//req to be set to 0 at first
  char log_file[255];//opt
  int log_verbosity;//opt
  int screen_verbosity;//opt
  char obj_file_base[255];//req
  char des_extension[5];//req
  char con_extension[5];//req
  char var_extension[5];//req
};
void start_program(int argv, char *args, struct main_control *control);
struct main_control *create_control();


#endif
