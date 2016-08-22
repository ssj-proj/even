all:
	gcc main.c nobj.c behaviors.c stimpool.c env_api.c ./env_01/random_env.c -ggdb -pthread -o even
