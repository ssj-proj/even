all:
	gcc  nobj.c behaviors.c stimpool.c env_api.c main.c eve.c ./env_c_random/random_env.c err.c -ggdb -pthread -o even

temp:
	gcc -c -fpic  behaviors.c stimpool.c nobj.c env_api.c main.c eve.c ./env_c_random/random_env.c err.c
	gcc -shared -o libeve.so  behaviors.o stimpool.o env_api.o main.o eve.o random_env.o err.o nobj.o
	echo >> evepy.pxd
	python setup.py build_ext -i
	rm *.o
