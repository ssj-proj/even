#include "eve.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *const *argv) {
  /*
    Perform initial preproccessing
  */

  int c;
  struct main_control *control = create_control();
  control->test=0;
  control->console_mode=1;
  int opterr = 0;
  char optopt;

  /*
    -O object directory path
    -t test

  */
  while ((c = getopt (argc, argv, "tO:")) != -1)
  {
    switch (c)
      {
      case 't':
        control->test=1;
        break;
      case 'O':
        printf("Setting object directory: ");
        memset(control->obj_file_base,'\0',strlen(control->obj_file_base));
	memcpy(control->obj_file_base, optarg, strlen(optarg));
        strcat(control->obj_file_base,"obj_");
        printf("%s\n",control->obj_file_base);
        break;
      case '?':
        if (optopt == 'O')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return -1;

      }
  }

  spin_off(control);
  printf("main thread doing things....");
  usleep(500000000);
}
