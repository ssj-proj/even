/*
  err.h -> for error handling and logging
*/
#ifndef ERR_H
#define ERR_H

struct err_settings {
  int verbosity;
  int console_mode;//0 - ignore all stdout/err as things are being handled by front end. 1 = print stdout messages
  char *file;
  int verbosity_to_screen;

};

void proc_err(const char *err_description, int err_severity, ... );
void write_log(const char *string);
void init_err(char *file, int verbosity, int verbosity_to_screen, int console_mode);

#endif

