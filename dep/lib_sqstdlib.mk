
default_target: all

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------
PRJ_NAME := sqstdlib
PRJ_TYPE := lib
PRJ_INCS := lib/SqPlus/include
PRJ_LIBS :=

PRJ_LIBROOT := ..

#-------------------------------------------------------------------
# Configure build
#-------------------------------------------------------------------
include $(PRJ_LIBROOT)/config.mk

#-------------------------------------------------------------------
# File locations
#-------------------------------------------------------------------
export LOC_TAG := def
LOC_INC_def := $(CFG_LIBROOT)/SqPlus/sqstdlib
LOC_SRC_def := $(CFG_LIBROOT)/SqPlus/sqstdlib
include $(PRJ_LIBROOT)/build.mk

#-------------------------------------------------------------------
# Execute the build
#-------------------------------------------------------------------
include $(PRJ_LIBROOT)/go.mk


