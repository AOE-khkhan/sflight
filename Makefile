
include makedefs

# Library         : Requires
# ------------------------------------------------------------------------
# sf              : -
#
PROJECTS = sf

# Examples        : Requires
# ------------------------------------------------------------------------
# mainTest        : libsf
#

.PHONY: all clean $(PROJECTS)

#
# Rules
#
all: $(PROJECTS)

$(PROJECTS):
	$(MAKE) -C $@

clean:
	-for d in $(PROJECTS); do (cd $$d; $(MAKE) clean ); done

