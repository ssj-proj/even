from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import os
print(os.getcwd())

setup(
	cmdclass = {'build_ext': build_ext},
	ext_modules = [
		Extension("evepy", ["src/python-frontend/evepy.pyx"],libraries=["eve"],working="build")
	]
)
