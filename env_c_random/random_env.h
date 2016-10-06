#ifndef RANDOM_ENV_H
#define RANDOM_END_H

//should be an endless loop function of which the caller can invoke actioins with state
void *main_loop(void *state);
void init_env0(struct env_control *ec, struct env_dat *dat, int env_id);
#endif
