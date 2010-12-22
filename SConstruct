import distutils.sysconfig
import os
coreenv = Environment(
	SWIGFLAGS=['-python','-c++'],
    CPPPATH=[distutils.sysconfig.get_python_inc(),
    	os.getcwd()],
    SHLIBPREFIX="",
    CCFLAGS=['-Wall','-Werror','-ggdb','-std=c++0x']
    )

swig_env = coreenv.Clone()
swig_env.Append(CCFLAGS='-Wno-uninitialized')
swig_env.Append(CCFLAGS='-Wno-uninitialized')
swig_env.Append(CCFLAGS='-Wno-sign-compare')
swig_env.Append(CCFLAGS='-Wno-parentheses')

coreenv.SharedLibrary('_pyshapemerge2d.so', [
	swig_env.SharedObject("pyshapemerge2d.i"),        
    "shapemerge2d/line2.cpp",
    "shapemerge2d/vertex.cpp",
    "shapemerge2d/vector.cpp",
    "shapemerge2d/polygon.cpp",
    "shapemerge2d/shape.cpp",
    "shapemerge2d/boolean_op.cpp",
    "shapemerge2d/shapemerge2d.cpp"
    ])
    
