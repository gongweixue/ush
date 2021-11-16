CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  vars  ###########################################
export TEST_OUTPUT_DIR := $(BUILD_DIR)
export TEST_LOG_FILE   := $(TEST_OUTPUT_DIR)/test.log

#############################  dirs  ###########################################
SUBDIRS := $(CUR_DIR)/comm
SUBDIRS += $(CUR_DIR)/pub
SUBDIRS += $(CUR_DIR)/ush


#############################  targets  ########################################
.PHONY: all $(SUBDIRS)
all: $(SUBDIRS)

$(SUBDIRS):
	$(MKDIR) $(subst $(TOP_DIR), $(BUILD_DIR), $@)
	$(MAKE) -C $@
