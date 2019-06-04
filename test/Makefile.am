TESTS = unit-tests
check_PROGRAMS = unit-tests
unit_tests_SOURCES = test-db.c test-filter.c unit-tests.c
unit_tests_CFLAGS = -I$(top_srcdir)/src @CHECK_CFLAGS@ @JSON_C_CFLAGS@ \
	-DTOP_SRCDIR="\"$(top_srcdir)\""
unit_tests_LDADD = $(top_builddir)/src/libminizen.la @CHECK_LIBS@ @JSON_C_LIBS@

noinst_HEADERS = unit-tests.h

dist_noinst_DATA = \
	data/partial/users.json \
	data/invalid/users.json \
	data/invalid/tickets.json
