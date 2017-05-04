cimport evepy
from cpython cimport array
import threading

cdef class even:
	cdef evepy.main_control *control
	steve_thread = threading.Thread()

	def __cinit__(self):
		self.control = evepy.create_control()
		self.control.halt=0
	def start(self):
		evepy.spin_off(self.control)
	def halt(self):
		self.control.halt=1
	def test(self):
		self.control.test=1
	def get_util(self, int obj):
		return evepy.get_obj_util(obj)
	def get_work(self):
		return evepy.get_oai_work()
	def set_oai_work2(self, observation, int size):
		print("stuff")
		cdef array.array input= array.array('d', observation)
		evepy.set_oai_input(input.data.as_doubles, size)
