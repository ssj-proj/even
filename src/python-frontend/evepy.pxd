cdef extern from "../backend/eve.h":
	struct main_control:
		int num_of_objs
		int loop_iteration
		int halt
		int test

	void start_program(int argv, char *args, main_control *control)
	double get_obj_util(int obj)
	main_control * create_control() 























