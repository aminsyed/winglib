
PRJ_RELROOT := ../..
include $(PRJ_RELROOT)/buildpath.mk

default_target: all

#-------------------------------------------------------------------
# Project
#-------------------------------------------------------------------
PRJ_NAME := irr_test
PRJ_DEPS := irrlicht
PRJ_TYPE := exe
PRJ_INCS := winglib/lib/oexlib
PRJ_LIBS := oexlib
PRJ_WINL := winmm opengl32 glu32
PRJ_DEFS := _STDCALL_SUPPORTED

#-------------------------------------------------------------------
# Configure build
#-------------------------------------------------------------------
include $(PRJ_LIBROOT)/config.mk

ifeq ($(OS),iphone)
UNSUPPORTED := TOOLS=$(TOOLS) not supported
include $(PRJ_LIBROOT)/unsupported.mk
else

ifeq ($(OS),android)
UNSUPPORTED := TOOLS=$(TOOLS) not supported
include $(PRJ_LIBROOT)/unsupported.mk
else

ifeq ($(PROC),arm)
	ifeq ($(OS),android)
		# -Wl,-rpath=/system/lib/egl GLESv1_CM
		PRJ_DEFS := ANDROID_NDK DISABLE_IMPORTGL
		PRJ_INCS := $(PRJ_INCS) irrlicht-android/project/include irrlicht-android/project/jni
		PRJ_LIBS := $(PRJ_LIBS) irrlicht-android cximage jpeg png tiff zlib bzip2
		ifneq ($(LIBLINK),static)
			PRJ_OSLB := $(PRJ_OSLB) GLESv2
		endif
		CFG_LFLAGS := $(CFG_LFLAGS) -Wl,-allow-shlib-undefined
	else
		PRJ_INCS := $(PRJ_INCS) irrlicht-es/include
		PRJ_LIBS := $(PRJ_LIBS) irrlicht-es jpeg png tiff zlib bzip2
	endif
else
	PRJ_INCS := $(PRJ_INCS) irrlicht/include
	PRJ_LIBS := $(PRJ_LIBS) irrlicht jpeg png tiff zlib bzip2
endif

#_IRR_USE_WINDOWS_CE_DEVICE_
ifeq ($(PLATFORM),posix)
	ifneq ($(PROC),arm)
		PRJ_OSLB := X11 GLU Xxf86vm
		PRJ_DEFS := $(PRJ_DEFS) _IRR_USE_LINUX_DEVICE_
	endif
else
	PRJ_DEFS := $(PRJ_DEFS) _IRR_STATIC_LIB_ _IRR_MULTI_ZLIB_
	ifeq ($(OS),wince)
		#PRJ_OSLB := libGLES_CM
		#PRJ_DEFS := $(PRJ_DEFS) _IRR_COMPILE_WITH_OGLES1_
		#PRJ_INCS := $(PRJ_INCS) ogles/include
	else
		ifeq ($(BUILD),gcc)
			PRJ_OSLB := opengl32
		endif
#		PRJ_DEFS := $(PRJ_DEFS) _IRR_COMPILE_WITH_OPENGL_
	endif	
endif

#-------------------------------------------------------------------
# File locations
#-------------------------------------------------------------------
export LOC_TAG := def
include $(PRJ_LIBROOT)/build.mk

#-------------------------------------------------------------------
# Execute the build
#-------------------------------------------------------------------
include $(PRJ_LIBROOT)/go.mk

endif

endif

