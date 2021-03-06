#include "err.h"
#include <stdio.h>
#include <stdarg.h>


static struct err_settings settings;
//set log location and verbosity (max level of err_severity to print)

void init_err(char *file, int verbosity, int verbosity_to_screen, int console_mode) {
  settings.file = file;
  settings.verbosity = verbosity;
  settings.verbosity_to_screen = settings.verbosity_to_screen;
  settings.console_mode=console_mode;
  write_log("Initializing logging\n");
}
void write_log(const char *string) {
  if(!settings.file){
    fprintf(stderr,"err.c:ERROR: err_settings not initialized\n");
    return;
  }
  FILE *f = fopen(settings.file,"a");
  fputs(string,f);
  fclose(f);
}

/*
  process error - write err to screen or log depending on severity (0-5)
  0 - System informational
  1 - Critical - program ceases to function
  2 - Important - severely impacts program operation
  3 - Issue - impacts program operation
  4 - Informational
  5 - Debug

*/
void proc_err(const char *err_description, int err_severity, ... ){

  if(err_severity<=settings.verbosity) {//write to log if vebosity setting permits
    write_log(err_description);
  }
  if(err_severity<=settings.verbosity_to_screen && settings.console_mode) {//write to stdout if verbosity and in console mode
    fprintf(stdout,"%",err_description);
  }
}

