CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  vars  ###########################################
TARGET_EXE_USHD := ushd
INCLUDE_DIR += $(TOP_DIR)/src/ushd/
INCLUDE_DIR += $(TOP_DIR)/src/pub/
INCLUDE_DIR += $(TOP_DIR)/src/util/

LIBRARY_DIR += $(BUILD_DIR)

LIBRARYS += ushd_sched
LIBRARYS += ushd_dist
LIBRARYS += ushd_tch
LIBRARYS += ushd_realm
LIBRARYS += ush_comm
LIBRARYS += ushd_conn
LIBRARYS += ush_util

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
        $(addprefix -I, $(INCLUDE_DIR)) \
        $(addprefix -L, $(LIBRARY_DIR)) \
		-Wl,--start-group $(addprefix -l, $(LIBRARYS)) -Wl,--end-group


$(SUBDIRS):
	$(MKDIR) $(subst $(TOP_DIR), $(BUILD_DIR), $@)
	$(MAKE) -C $@
