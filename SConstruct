import distutils.sysconfig
import os

coreenv = Environment(
    CCFLAGS=['-Os','-Wall','-ggdb','-std=c++0x',"-I."]
    )

swig_env = coreenv.Clone()
swig_env.Append(SWIGFLAGS=['-python','-c++'])
swig_env.Append(CPPPATH=[distutils.sysconfig.get_python_inc(),
    	os.getcwd()])
swig_env.Append(CCFLAGS='-Wno-uninitialized')
swig_env.Append(CCFLAGS='-Wno-uninitialized')
swig_env.Append(CCFLAGS='-Wno-sign-compare')
swig_env.Append(CCFLAGS='-Wno-parentheses')
swig_env.Append(LIBPATH=".")

coreenv.SharedLibrary('shapemerge2d', [
    "shapemerge2d/line.cpp",
    "shapemerge2d/vertex.cpp",
    "shapemerge2d/vector.cpp",
    "shapemerge2d/polygon.cpp",
    "shapemerge2d/shape.cpp",
    "shapemerge2d/boolean_op.cpp",
    "shapemerge2d/shapemerge2d.cpp"
    ])

progenv = coreenv.Clone()
progenv.Append(LIBPATH=".")


swig_env.SharedLibrary('_pyshapemerge2d.so', [
	swig_env.SharedObject("pyshapemerge2d.i")
    ],
    LIBS=["shapemerge2d"],
    SHLIBPREFIX="",
    LINKFLAGS=["-Wl,-rpath="+os.getcwd()]
    )

progenv.Program('example1', [
    "examples/example1.cpp"
    ],LIBS=["shapemerge2d"],
    LINKFLAGS=["-Wl,-rpath="+os.getcwd()]
    )


