all:
	gcc main.c nobj.c behaviors.c stimpool.c env_api.c ./env_c_random/random_env.c -ggdb -pthread -o even
