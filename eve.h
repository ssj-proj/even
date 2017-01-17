#ifndef
#define EVE_H
struct main_control {
  int num_of_objs;
  int loop_iteration;
}
void start_program(int argv, char *args, struct main_control *control);



#endif
