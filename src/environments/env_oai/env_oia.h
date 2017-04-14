#ifndef ENV_OAI_H
#define ENV_OAI_H

//should be an endless loop function of which the caller can invoke actioins with state
void *main_loop_oai(void *state);
void init_env_oai(struct env_control *ec, struct env_dat *dat, int env_id);
#endif
