CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  dirs  ###########################################
SUBDIRS := $(CUR_DIR)/comm
SUBDIRS += $(CUR_DIR)/util
SUBDIRS += $(CUR_DIR)/ush
SUBDIRS += $(CUR_DIR)/ushd


#############################  targets  ########################################
.PHONY: all $(SUBDIRS)
all: $(SUBDIRS)


$(SUBDIRS):
	$(MKDIR) $(subst $(TOP_DIR), $(BUILD_DIR), $@)
	$(MAKE) -C $@
