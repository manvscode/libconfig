
# Add new files in alphabetical order. Thanks.
libconfig_src = config.c config-lexer.c config-parser.c

# Add new files in alphabetical order. Thanks.
libconfig_headers = config.h

library_includedir      = $(includedir)/@PACKAGE_NAME@-@VERSION@/
library_include_HEADERS = $(libconfig_headers)

# Library
lib_LTLIBRARIES                          = $(top_builddir)/lib/libconfig.la
__top_builddir__lib_libconfig_la_SOURCES = $(libconfig_src)
__top_builddir__lib_libconfig_la_CFLAGS  = -fPIC $(COLLECTIONS_CFLAGS)
__top_builddir__lib_libconfig_la_LDFLAGS = -lm $(COLLECTIONS_LIBS)

