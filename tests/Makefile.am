if ENABLE_TESTS
AM_CFLAGS  = -std=c99 -pg -g -ggdb -O0 $(COLLECTIONS_CFLAGS)
AM_LDFLAGS = -static
LDADD      = $(COLLECTIONS_LIBS) $(top_builddir)/lib/libconfig.la

bin_PROGRAMS = $(top_builddir)/bin/test-config

__top_builddir__bin_test_config_SOURCES = test-config.c
endif
