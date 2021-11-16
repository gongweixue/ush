CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  vars  ###########################################
#########################  append flags as you want  ###########################
CFLAGS += -Wlarger-than=128

INCLUDE_DIR += $(CUR_DIR)
INCLUDE_DIR += $(TOP_DIR)/src/pub/
INCLUDE_DIR += $(TOP_DIR)/src/util/


#############################  dirs  ###########################################
BUILD_SUBDIR := $(subst $(TOP_DIR), $(BUILD_DIR), $(CUR_DIR))


#############################  files  ##########################################
#################### change the src files and library name  ####################
TARGET_NAME := $(TARGET_LIB_USH_COMM)

SRC_FILES   := ush_comm_lstnr_cmd_stop.c
SRC_FILES   += ush_comm_lstnr_hay.c
SRC_FILES   += ush_comm_lstnr_sig_upd.c
SRC_FILES   += ush_comm_lstnr_sigreg_ack.c
SRC_FILES   += ush_comm_realm_cmd.c
SRC_FILES   += ush_comm_realm_sigreg.c
SRC_FILES   += ush_comm_realm_sigset.c
SRC_FILES   += ush_comm_realm_sigtease.c
SRC_FILES   += ush_comm_tch_hello.c
SRC_FILES   += ush_comm_tch_goodbye.c

OBJ_FILES   := $(SRC_FILES:%.c=$(BUILD_SUBDIR)/%.o)


#############################  targets  ########################################
.PHONY: all $(TARGET_NAME)
all: $(TARGET_NAME)
	$(ECHO) $(TARGET_NAME) generated.

$(TARGET_NAME): $(OBJ_FILES)
	$(AR) crsuU $(BUILD_DIR)/$@ $^

$(OBJ_FILES): $(BUILD_SUBDIR)/%o : %c
	$(GCC) $(CFLAGS) -c $< -o $@ $(addprefix -I,$(INCLUDE_DIR))
