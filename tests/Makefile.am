if ENABLE_TESTS
AM_CFLAGS = -std=c99 -pg -g -ggdb -O0 -I$(top_builddir)/src/ -I/usr/local/include/ -L$(top_builddir)/lib/  -L/usr/local/lib

bin_PROGRAMS = \
$(top_builddir)/bin/test-config

__top_builddir__bin_test_config_SOURCES      = test-config.c
__top_builddir__bin_test_config_LDFLAGS      = -lcollections $(top_builddir)/lib/.libs/libconfig.a
endif
