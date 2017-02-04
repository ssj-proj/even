#include "eve.h"

void main(int argv, char *args) {
  /*
    Perform initial preproccessing
  */
  struct main_control *control = create_control();
  control->halt=0;
  control->test=1;
  start_program(argv,args,control);
}


























