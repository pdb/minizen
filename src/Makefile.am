noinst_LTLIBRARIES = libminizen.la
libminizen_la_CPPFLAGS = -I$(top_srcdir)/include
libminizen_la_SOURCES = db.c filter.c

bin_PROGRAMS = minizen
minizen_SOURCES = formatter.c minizen.c minizen-search.c
minizen_CPPFLAGS = -I$(top_srcdir)/include
minizen_CFLAGS = @JSON_C_CFLAGS@
minizen_LDADD = @JSON_C_LIBS@ libminizen.la

noinst_HEADERS = filter.h formatter.h minizen.h
