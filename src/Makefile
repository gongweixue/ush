CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  dirs  ###########################################
SUBDIRS := $(CUR_DIR)/comm
SUBDIRS += $(CUR_DIR)/util
SUBDIRS += $(CUR_DIR)/ush
SUBDIRS += $(CUR_DIR)/ushd

BUILD_SUBDIRS := $(subst $(CUR_DIR), $(BUILD_DIR), $(SUBDIRS))

#############################  targets  ########################################
.PHONY: all $(SUBDIRS)
all: $(SUBDIRS)
	$(ECHO) $(CUR_DIR)
	$(ECHO) $(BUILD_SUBDIRS)


$(SUBDIRS):
	$(MKDIR) $(subst $(TOP_DIR), $(BUILD_DIR), $@)
	$(MAKE) -C $@
