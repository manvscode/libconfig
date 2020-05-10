AC_PREREQ(2.67)
AC_INIT([config], [0.4], [manvscode@gmail.com], [libconfig], [http://www.manvscode.com/])

AM_INIT_AUTOMAKE([1.13 foreign silent-rules dist-zip -Wall -Werror])
#AC_CONFIG_MACRO_DIRS([m4])

# Enable silent build when possible.
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_HEADERS([src/libconfig-config.h])

AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_CC_STDC
AC_HEADER_STDC
AC_C_INLINE


AH_TOP([
#ifndef _LIBCONFIG_H_
#define _LIBCONFIG_H_
])
AH_BOTTOM([
#endif /* _LIBCONFIG_H_ */
])


#DFLAGS="-D DEBUG "
DFLAGS=""

#CFLAGS="-Wall -g -ggdb -O0 $DFLAGS -I/usr/local/include"
CFLAGS="$CFLAGS "
LDFLAGS="$LDFLAGS"

AM_PROG_AR
LT_INIT([static])
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL


PKG_CHECK_MODULES([COLLECTIONS], [collections-static],
	[AC_MSG_NOTICE([Found libcollections])],
	[AC_MSG_ERROR([Missing libcollections])],
)


# -------------------------------------------------
AC_ARG_ENABLE([tests],
	[AS_HELP_STRING([--enable-tests], [Enable test programs.])],
	[:],
	[enable_tests=no])

AM_CONDITIONAL([ENABLE_TESTS], [test "x$enable_tests" = "xyes"])
# -------------------------------------------------


AC_PROG_INSTALL

echo "------------------------------------------------------------------------------"
if [test -z "$CFLAGS"]; then
	echo "  CFLAGS: Not set"
else
	echo "  CFLAGS: $CFLAGS"
fi
if [test -z "$LDFLAGS"]; then
	echo " LDFLAGS: Not set"
else
	echo " LDFLAGS: $LDFLAGS"
fi
echo "------------------------------------------------------------------------------"

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	tests/Makefile
	config.pc
])

AC_OUTPUT
