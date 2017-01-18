cimport evepy

cdef class even:
	cdef evepy.main_control *control
	def __cinit__(self):
		self.control = evepy.create_control()
		evepy.start_program(0, NULL,self.control)
