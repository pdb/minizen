AC_INIT(minizen, 1.0.0)
AM_INIT_AUTOMAKE(foreign)

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(CHECK, check)
PKG_CHECK_MODULES(JSON_C, json-c >= 0.13.0)

AC_CONFIG_FILES(
	Makefile
	include/Makefile
	include/minizen/Makefile
	lib/Makefile
	src/Makefile
	test/Makefile
)

AC_OUTPUT
