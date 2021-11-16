CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

#############################  vars  ###########################################
#########################  append flags as you want  ###########################
INCLUDE_DIR += $(CUR_DIR)


#############################  dirs  ###########################################
BUILD_SUBDIR := $(subst $(TOP_DIR), $(BUILD_DIR), $(CUR_DIR))


#############################  files  ##########################################
#################### change the src files and library name  ####################
TARGET_NAME := $(TARGET_LIB_USH_UTIL)

SRC_FILES := ush_log.c
SRC_FILES += ush_random.c
SRC_FILES += ush_string.c

OBJ_FILES   := $(SRC_FILES:%.c=$(BUILD_SUBDIR)/%.o)


#############################  targets  ########################################
.PHONY: all $(TARGET_NAME)
all: $(TARGET_NAME)
	$(ECHO) $(TARGET_NAME) generated.

$(TARGET_NAME): $(OBJ_FILES)
	$(AR) crsuU $(BUILD_DIR)/$@ $^

$(OBJ_FILES): $(BUILD_SUBDIR)/%o : %c
	$(GCC) $(CFLAGS) -c $< -o $@ $(addprefix -I,$(INCLUDE_DIR))
