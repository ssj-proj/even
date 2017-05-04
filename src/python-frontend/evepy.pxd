cdef extern from "../backend/eve.h":
	struct main_control:
		int num_of_objs
		int loop_iteration
		int halt
		int test

	void *start_program(void *control)
	int spin_off(main_control *control)
	double get_obj_util(int obj)
	main_control * create_control()
	int get_oai_work()
	void set_oai_input(double *ins, int size)
	int get_period_wait()
