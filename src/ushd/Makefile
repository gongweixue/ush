CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  vars  ###########################################
TARGET_EXE_USHD := ushd
INCLUDE_DIR += $(TOP_DIR)/src/ushd/
INCLUDE_DIR += $(TOP_DIR)/src/pub/
INCLUDE_DIR += $(TOP_DIR)/src/util/

#############################  dirs  ###########################################
SUBDIRS := $(CUR_DIR)/conn
SUBDIRS += $(CUR_DIR)/dist
SUBDIRS += $(CUR_DIR)/realm
SUBDIRS += $(CUR_DIR)/sched
SUBDIRS += $(CUR_DIR)/tch

BUILD_SUBDIRS := $(subst $(CUR_DIR), $(BUILD_DIR), $(SUBDIRS))

#############################  targets  ########################################
.PHONY: all $(SUBDIRS)
all: $(BUILD_DIR)/$(TARGET_EXE_USHD)


$(BUILD_DIR)/$(TARGET_EXE_USHD):  $(SUBDIRS)
	$(GCC) $(CFLAGS) -o $@ ushd_main.c \
        $(addprefix -I,$(INCLUDE_DIR)) -L$(BUILD_DIR) \
        -lushd_sched -lushd_dist -lushd_tch -lushd_realm -lush_comm -lushd_conn -lush_util -lpthread -lrt


$(SUBDIRS):
	$(MKDIR) $(subst $(TOP_DIR), $(BUILD_DIR), $@)
	$(MAKE) -C $@
