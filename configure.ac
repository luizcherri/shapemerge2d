AC_INIT([shapemerge2d], [1.0], [anders.musikka@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_CC
AC_PROG_CXX
LT_INIT
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile shapemerge2d/Makefile])

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/foreach.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries (boost foreach).)])
AC_CHECK_HEADERS([boost/rational.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries (boost rational).)])
AC_LANG_POP([C++])

AC_OUTPUT

