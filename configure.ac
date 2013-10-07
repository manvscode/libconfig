AC_INIT([libconfig], [0.4], [manvscode@gmail.com], [libconfig], [http://www.manvscode.com/])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_SILENT_RULES([yes])
AC_CONFIG_HEADERS([src/libconfig-config.h])

AC_PROG_CC
AC_PROG_CC_C99
AC_C_INLINE
AC_HEADER_STDC


AH_TOP([
#ifndef _LIBCONFIG_H_
#define _LIBCONFIG_H_
])
AH_BOTTOM([
#endif /* _LIBCONFIG_H_ */
])


DFLAGS="-D DEBUG "

#CFLAGS="-Wall -g -ggdb -O0 $DFLAGS -I/usr/local/include"
CFLAGS="-Wall -O3 -I/usr/local/include"
LDFLAGS="-lcollections"

AM_PROG_AR
LT_INIT([shared static])
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL



AC_PROG_INSTALL

echo " ------------------------------ "
echo ""
echo "  CFLAGS = $CFLAGS"
echo " LDFLAGS = $LDFLAGS"
echo ""
echo " ------------------------------ "

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	tests/Makefile
])

AC_OUTPUT
