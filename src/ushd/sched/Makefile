CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  vars  ###########################################
#########################  append flags as you want  ###########################
INCLUDE_DIR += $(CUR_DIR)
INCLUDE_DIR += $(TOP_DIR)/src/ushd/
INCLUDE_DIR += $(TOP_DIR)/src/comm/
INCLUDE_DIR += $(TOP_DIR)/src/pub/
INCLUDE_DIR += $(TOP_DIR)/src/util/


#############################  dirs  ###########################################
BUILD_SUBDIR := $(subst $(TOP_DIR), $(BUILD_DIR), $(CUR_DIR))


#############################  files  ##########################################
#################### change the src files and library name  ####################
TARGET_NAME := $(TARGET_LIB_USHD_SCHED)

SRC_FILES := ushd_sched_thread.c
SRC_FILES += ushd_sched_fifo.c
SRC_FILES += ushd_sched_proc.c
SRC_FILES += ushd_sched_proc_tch.c
SRC_FILES += ushd_sched_proc_tch_hello.c
SRC_FILES += ushd_sched_proc_tch_goodbye.c
SRC_FILES += ushd_sched_proc_realm.c
SRC_FILES += ushd_sched_proc_realm_sig.c

OBJ_FILES   := $(SRC_FILES:%.c=$(BUILD_SUBDIR)/%.o)


#############################  targets  ########################################
.PHONY: all $(TARGET_NAME)
all: $(TARGET_NAME)
	$(ECHO) $(TARGET_NAME) generated.

$(TARGET_NAME): $(OBJ_FILES)
	$(AR) crsuU $(BUILD_DIR)/$@ $^

$(OBJ_FILES): $(BUILD_SUBDIR)/%o : %c
	$(GCC) $(CFLAGS) -c $< -o $@ $(addprefix -I,$(INCLUDE_DIR))
