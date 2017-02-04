#admittedly poor build process - fix later
root = ../
env_src = ${root}src/environments/env_c_random/
backend_dir = ${root}src/backend/
fronted_src =src/python-frontend/
backend_files = behaviors.o stimpool.o nobj.o env_api.o main.o eve.o err.o
backend_src = ${backend_dir}behaviors.c ${backend_dir}eve.c ${backend_dir}stimpool.c ${backend_dir}nobj.c ${backend_dir}env_api.c ${backend_dir}main.c  ${backend_dir}err.c
environment_files = random_env.o

all:
	gcc -c -fpic -ggdb -pthread ${backend_src}
	gcc -c -fpic -ggdb -pthread ${env_src}*.c
	#stand alone cmd executable
	gcc -o ../bin/even -pthread  ${backend_files} ${environment_files}
	#library for front end
	gcc -shared -o libeve.so ${backend_files} ${environment_files}
	#build python front end
	cd ${root} && touch ${fronted_src}evepy.pxd && python ${fronted_src}setup.py build_ext -i  
	mv ${root}*.so ./
backend:
	gcc -c -fpic -ggdb -pthread ${backend_src}
	gcc -c -fpic -ggdb -pthread ${env_src}*.c
	#stand alone cmd executable
	gcc -o ../bin/even -pthread  ${backend_files} ${environment_files}

install:
	cd ${root} && python ${fronted_src}setup.py install
	cp libeve.so /usr/lib/
	cp libeve.so /usr/lib64/
test: 
	${root}bin/even
