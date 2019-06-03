TESTS = unit-tests
check_PROGRAMS = unit-tests
unit_tests_SOURCES = test-filter.c unit-tests.c
unit_tests_CFLAGS = -I$(top_srcdir)/src @CHECK_CFLAGS@ @JSON_C_CFLAGS@
unit_tests_LDADD = $(top_builddir)/src/libminizen.la @CHECK_LIBS@ @JSON_C_LIBS@

noinst_HEADERS = unit-tests.h
