CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  dirs  ###########################################
SUBDIRS := $(CUR_DIR)/pipe
SUBDIRS += $(CUR_DIR)/sig
SUBDIRS += $(CUR_DIR)/simple

#############################  targets  ########################################
.PHONY: all $(SUBDIRS)
all: $(SUBDIRS)

$(SUBDIRS):
	$(MKDIR) $(subst $(TOP_DIR), $(BUILD_DIR), $@)
	$(MAKE) -C $@
