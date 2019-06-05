bin_PROGRAMS = minizen
minizen_SOURCES = formatter.c minizen.c minizen-search.c
minizen_CPPFLAGS = -I$(top_srcdir)/include
minizen_CFLAGS = @JSON_C_CFLAGS@
minizen_LDADD = @JSON_C_LIBS@ $(top_builddir)/lib/libminizen.la

noinst_HEADERS = formatter.h minizen.h
