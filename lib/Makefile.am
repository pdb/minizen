lib_LTLIBRARIES = libminizen.la
libminizen_la_CPPFLAGS = -I$(top_srcdir)/include
libminizen_la_SOURCES = db.c filter.c
libminizen_la_CFLAGS = @JSON_C_CFLAGS@
libminizen_la_LIBADD = @JSON_C_LIBS@
libminizen_la_LDFLAGS = -version-info 1:0:0

noinst_HEADERS = filter.h
