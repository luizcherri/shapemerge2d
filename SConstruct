import distutils.sysconfig
import os

coreenv = Environment(
    CCFLAGS=['-Wall','-Werror','-ggdb','-std=c++0x',"-I."]
    )

#swig_env = coreenv.Clone()
#swig_env.Append(SWIGFLAGS=['-python','-c++'])
#swig_env.Append(CPPPATH=[distutils.sysconfig.get_python_inc(),
#    	os.getcwd()])
#swig_env.Append(SHLIBPREFIX="")
#swig_env.Append(CCFLAGS='-Wno-uninitialized')
#swig_env.Append(CCFLAGS='-Wno-uninitialized')
##swig_env.Append(CCFLAGS='-Wno-sign-compare')
##swig_env.Append(CCFLAGS='-Wno-parentheses')
#swig_env.Append(LDFLAGS='-L. -lshapemergse2d --rpath='+os.getcwd())
#swig_env.Append(LIBS="shapemerge2d")
#swig_env.Append(LIBPATH=".")

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
progenv.Append(LIBS=["shapemerge2d"])
progenv.Append(LIBPATH=".")
progenv.Append(LDFLAGS="--rpath="+os.getcwd())


#swig_env.SharedLibrary('_pyshapemerge2d.so', [
#	swig_env.SharedObject("pyshapemerge2d.i")
#    ])

progenv.Program('example1', [
    "examples/example1.cpp"
    ],LIBS=["shapemerge2d"]
    )


