all:
	gcc  nobj.c behaviors.c stimpool.c env_api.c main.c ./env_c_random/random_env.c err.c -ggdb -pthread -o even
