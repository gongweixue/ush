CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  dirs  ###########################################
SUBDIRS := $(CUR_DIR)/reg
SUBDIRS += $(CUR_DIR)/set
SUBDIRS += $(CUR_DIR)/tease

#############################  targets  ########################################
.PHONY: all $(SUBDIRS)
all: $(SUBDIRS)

$(SUBDIRS):
	$(MKDIR) $(subst $(TOP_DIR), $(BUILD_DIR), $@)
	$(MAKE) -C $@
