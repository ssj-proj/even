cimport evepy
from cpython cimport array


cdef class even:
	cdef evepy.main_control *control
	def __cinit__(self):
		self.control = evepy.create_control()
		self.control.halt=0
	def start(self):
		evepy.start_program(0, NULL,self.control)
	def halt(self):
		self.control.halt=1
	def test(self):
		self.control.test=1
	def get_util(self, int obj):
		return evepy.get_obj_util(obj)
	def get_oai_work():
		return evepy.get_oai_work()
	def set_oai_work(double *observation):
		evepy.set_oai_input(&observation)
