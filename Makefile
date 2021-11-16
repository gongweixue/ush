CUR_DIR:=$(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

##########################  global var  ########################################
export AT    := @
export RM    := $(AT) rm -rf
export ECHO  := $(AT) echo
export MAKE  := $(AT) make
export MKDIR := $(AT) mkdir -p

export TOP_DIR   := $(CUR_DIR)
export BUILD_DIR := $(CUR_DIR)/build

ifeq ($(USH_QNX), 1)
    QNX_HOST_TOOL_DIR := usr/bin
    export AR     := $(QNX_HOST)/$(QNX_HOST_TOOL_DIR)/ntoaarch64-ar
    export CC     := $(QNX_HOST)/$(QNX_HOST_TOOL_DIR)/ntoaarch64-gcc
    export LD     := $(QNX_HOST)/$(QNX_HOST_TOOL_DIR)/ntoaarch64-ld
    export RANLIB := $(QNX_HOST)/$(QNX_HOST_TOOL_DIR)/ntoaarch64-ranlib
    export STRP   := $(QNX_HOST)/$(QNX_HOST_TOOL_DIR)/ntoaarch64-strip
    export RUNTIME_LIBS := c
else
    export AR     := @ar
    export GCC    := gcc
    export LD     := ld
    export RANLIB := ranlib
    export STRP   := strip
    export RUNTIME_LIBS := rt pthread
endif # USH_QNX

export CFLAGS := -Wall -Wbad-function-cast -Wbool-compare -Wcast-align \
            -Wcast-qual -Wcomments -Wconversion -Wdisabled-optimization \
            -Wdouble-promotion -Wempty-body -Wendif-labels -Werror -Wextra \
            -Wfatal-errors -Wfloat-conversion -Wfloat-equal \
            -Wignored-qualifiers -Winline -Wlogical-not-parentheses \
            -Wlogical-op -Wmaybe-uninitialized -Wmemset-transposed-args \
            -Wmissing-braces -Wmissing-declarations \
            -Wmissing-field-initializers -Wmissing-include-dirs \
            -Wmissing-parameter-type -Wmissing-prototypes -Wnested-externs \
            -Woverflow -Wparentheses -Wpointer-arith -Wsign-compare \
            -Wsizeof-pointer-memaccess -Wsequence-point -Wshadow \
            -Wshift-count-negative -Wshift-count-overflow -Wstrict-prototypes \
            -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code \
            -Wunsafe-loop-optimizations -Wunused -Wunused-label -Wvla \
            -Wwrite-strings -finline-functions -fmax-errors=10 \
            -fno-strict-aliasing

export INCLUDE_DIR = $(CUR_DIR)/gen
export LIBRARY_DIR = $(patsubst %, -l%, $(RUNTIME_LIBS))

ifneq ($(USH_DEBUG), 0)
    CFLAGS += -g -O0
else
    CFLAGS += -O2 -s
endif


#############################  targets  ########################################
export TARGET_LIB_USH        := libush.a
export TARGET_LIB_USH_COMM   := libush_comm.a
export TARGET_LIB_USH_UTIL   := libush_util.a
export TARGET_LIB_USHD_CONN  := libushd_conn.a
export TARGET_LIB_USHD_DIST  := libushd_dist.a
export TARGET_LIB_USHD_REALM := libushd_realm.a
export TARGET_LIB_USHD_SCHED := libushd_sched.a
export TARGET_LIB_USHD_TCH   := libushd_tch.a


#############################  dirs  ###########################################
SUBDIRS := $(CUR_DIR)/src
# SUBDIRS += $(TOP_DIR)/test

BUILD_SUBDIRS := $(subst $(CUR_DIR), $(BUILD_DIR), $(SUBDIRS))

#############################  targets  ########################################
.PHONY: all clean autogen $(SUBDIRS)
all: autogen $(SUBDIRS)
	$(ECHO) @@@ Everything is done. @@@

autogen:
	$(AT) ls $(CUR_DIR)/src/plugin/* | xargs cat  > $(CUR_DIR)/gen/ush_sig_conf

$(SUBDIRS):
	$(MKDIR) $(subst $(CUR_DIR), $(BUILD_DIR), $@)
	$(MAKE) -C $@

clean:
	-$(RM) $(BUILD_DIR)
