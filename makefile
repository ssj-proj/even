all:
	gcc main.c nobj.c behaviors.c stimpool.c -ggdb -pthread -o even
