CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  vars  ###########################################
#########################  append flags as you want  ###########################
INCLUDE_DIR += $(CUR_DIR)
INCLUDE_DIR += $(TOP_DIR)/src/comm/
INCLUDE_DIR += $(TOP_DIR)/src/pub/
INCLUDE_DIR += $(TOP_DIR)/src/util/


#############################  dirs  ###########################################
BUILD_SUBDIR := $(subst $(TOP_DIR), $(BUILD_DIR), $(CUR_DIR))


#############################  files  ##########################################
#################### change the src files and library name  ####################
TARGET_NAME := $(TARGET_LIB_USH)

SRC_FILES := ush_connect.c
SRC_FILES += ush_sync.c
SRC_FILES += ush_realm.c
SRC_FILES += ush_tch.c
SRC_FILES += ush_version.c
SRC_FILES += ush_lstnr.c
SRC_FILES += ush_lstnr_proc.c
SRC_FILES += ush_lstnr_proc_cmd.c
SRC_FILES += ush_lstnr_proc_hay.c
SRC_FILES += ush_lstnr_proc_sig.c
SRC_FILES += ush_pipe_create.c
SRC_FILES += ush_pipe_delete.c
SRC_FILES += ush_sigreg.c
SRC_FILES += ush_sigset.c
SRC_FILES += ush_sigtease.c

OBJ_FILES   := $(SRC_FILES:%.c=$(BUILD_SUBDIR)/%.o)


#############################  targets  ########################################
.PHONY: all $(TARGET_NAME)
all: $(TARGET_NAME)
	$(ECHO) $(TARGET_NAME) generated.

$(TARGET_NAME): $(OBJ_FILES)
	$(AR) crsuU $(BUILD_DIR)/$@ $^

$(OBJ_FILES): $(BUILD_SUBDIR)/%o : %c
	$(GCC) $(CFLAGS) -c $< -o $@ $(addprefix -I,$(INCLUDE_DIR))
