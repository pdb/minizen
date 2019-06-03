noinst_LTLIBRARIES = libminizen.la
libminizen_la_SOURCES = filter.c

bin_PROGRAMS = minizen
minizen_SOURCES = minizen.c
minizen_CFLAGS = @JSON_C_CFLAGS@
minizen_LDADD = @JSON_C_LIBS@ libminizen.la

noinst_HEADERS = filter.h
