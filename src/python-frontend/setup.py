from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import os
print("||.......................||")
print("||.......................||")
print("||.......................||")
print("||       Setup.py        ||")
print("||   Start front end     ||")
print("||    Build Process      ||")
print("||.......................||")
print("||.......................||")
print("||.......................||")
print(os.getcwd())

setup(
	cmdclass = {'build_ext': build_ext},
	ext_modules = [
		Extension("evepy", ["src/python-frontend/evepy.pyx"],libraries=["build/eve"])
	]
)
