cimport evepy

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
