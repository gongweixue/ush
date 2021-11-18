CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))


#############################  vars  ###########################################
INCLUDE_DIR += $(CUR_DIR)
INCLUDE_DIR += $(TOP_DIR)/test/
INCLUDE_DIR += $(TOP_DIR)/src/pub/
INCLUDE_DIR += $(TOP_DIR)/src/util/

LIBRARY_DIR += $(BUILD_DIR)

LIBRARYS += ush ush_comm ush_util


#############################  dirs  ###########################################
BUILD_SUBDIR := $(subst $(TOP_DIR), $(BUILD_DIR), $(CUR_DIR))


#############################  files  ##########################################
TARGET_EXE := test-ush-sigtease

SRC_FILES   := case_ush_sigtease.c

OBJ_FILES   := $(SRC_FILES:%.c=$(BUILD_SUBDIR)/%.o)


#############################  command  ########################################
RUN_COMMAND :=
ifneq ($(USH_QNX), 1)
    ifeq ($(USH_TEST), 1)
        RUN_COMMAND := $(AT) $(BUILD_DIR)/$(TARGET_EXE) > $(TEST_LOG_FILE).$(TARGET_EXE)
    endif
endif


#############################  targets  ########################################
.PHONY: all RUN
all: $(BUILD_DIR)/$(TARGET_EXE) RUN

RUN:
	$(RUN_COMMAND)

$(BUILD_DIR)/$(TARGET_EXE): $(SRC_FILES)
	$(GCC) $(CFLAGS) -o $@ $< \
        $(addprefix -I, $(INCLUDE_DIR)) $(addprefix -L, $(LIBRARY_DIR)) \
        -Wl,--start-group $(addprefix -l, $(LIBRARYS)) -Wl,--end-group
