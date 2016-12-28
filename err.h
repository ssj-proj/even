/*
  err.h -> for error handling and logging
*/
#ifndef ERR_H
#define ERR_H

struct err_settings {
  int verbosity;
  char *file;
  int verbosity_to_screen;

};
void set_err(char file[], int verbosity);
void proc_err(char err_description[], int err_severity);
void write_log(char *string);

#endif

