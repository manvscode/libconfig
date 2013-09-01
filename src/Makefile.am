
# Add new files in alphabetical order. Thanks.
libconfig_src = config.c config-lexer.c config-parser.c

# Add new files in alphabetical order. Thanks.
libconfig_headers = config.h 

library_includedir      = $(includedir)/libconfig/
library_include_HEADERS = $(libconfig_headers)

# Library
lib_LTLIBRARIES                           = $(top_builddir)/lib/libconfig.la 
__top_builddir__lib_libconfig_la_SOURCES = $(libconfig_src)
__top_builddir__lib_libconfig_la_LIBADD  = -lcollections

