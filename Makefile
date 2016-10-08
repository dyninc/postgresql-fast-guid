#
# Makefile
#

MODULE_big = fast_guid
DATA=fast_guid--0.1.sql
OBJS=fast_guid.o
EXTENSION=fast_guid

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

