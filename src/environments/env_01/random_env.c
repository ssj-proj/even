#include "../env_api.h"
#include "random_env.h"
#include <unistd.h>
void *main_loop(void *state){
 while(1) {
 
	get_next_output(0);
	printf("sleep..\n");
	usleep(1000000);
 
  }


}
