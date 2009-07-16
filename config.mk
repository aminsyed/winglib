
SHELL=/bin/sh
#SHELL=cmd.exe

# config.mk
# Cross compiler config

BUILD	 := gcc
#BUILD	 := vs

#OS		 := linux
#OS		 := win32
#OS		 := wince

PROC	 := i386
#PROC	 := arm

TOOLS	 := local
#TOOLS	 := debian
#TOOLS	 := codesourcery
#TOOLS	 := snapgear
#TOOLS	 := buildroot
#TOOLS	 := crosstool
#TOOLS	 := nihilism
#TOOLS	 := openmoko
#TOOLS	 := uclinux
#TOOLS	 := armel
#TOOLS	 := cegcc
#TOOLS	 := mingw32
#TOOLS	 := ming232ce
#TOOLS	 := iphone
#TOOLS	 := openmoko
#TOOLS	 := mac

#OS := $(shell uname -o)
#ifeq $(OS) GNU/Linux

#CFG_CURDIR := $(shell pwd)

ifeq ($(BUILD),vs)
	LIBLINK	 := static
else
	LIBLINK := shared
endif

ifdef PRJ_ROOT
	CFG_ROOT := $(PRJ_ROOT)
	CFG_TOOLROOT := $(PRJ_ROOT)/tools
	CFG_LIBROOT  := $(PRJ_OSROOT)
else
	CFG_ROOT := $(PRJ_LIBROOT)/../..
	CFG_TOOLROOT := $(PRJ_LIBROOT)/../../tools
	CFG_LIBROOT  := $(PRJ_LIBROOT)/..
endif

ifdef UNICODE
CFG_CEXTRA := $(CFG_CEXTRA) -DUNICODE -D_UNICODE
endif

ifdef PRJ_DEFS
	ifeq ($(BUILD),vs)
		CFG_DEFS := $(foreach def,$(PRJ_DEFS),/D$(def) )
	else
		CFG_DEFS := $(foreach def,$(PRJ_DEFS),-D$(def) )
	endif	
	PRJ_DEFS := 
endif

ifeq ($(BUILD),vs)

	OS := win32
	PLATFORM := windows

	ifdef DBG
		ifeq ($(LIBLINK),static)	
			CFG_CEXTRA	 := /DDEBUG /D_DEBUG /D_MT /MTd /Z7 $(CFG_CEXTRA)
		else
			ifeq ($(PRJ_TYPE),dll)
				CFG_CEXTRA	 := /DDEBUG /D_DEBUG /D_MT /MDd /Z7 $(CFG_CEXTRA)
			else
				CFG_CEXTRA	 := /DDEBUG /D_DEBUG /D_MT /MDd /Z7 $(CFG_CEXTRA)
			endif
		endif
		ifeq ($(LIBLINK),static)	
			CFG_LEXTRA	 := /DEBUG
		else
			CFG_LEXTRA	 := /DEBUG /NODEFAULTLIB:libcmtd
		endif
		CFG_DPOSTFIX := _d
		ifeq ($(CFG_STDLIBS),)
			CFG_STDLIBS	 := ole32.lib Oleaut32.lib user32.lib gdi32.lib comdlg32.lib comctl32.lib rpcrt4.lib shell32.lib Advapi32.lib
		endif
	else
		ifeq ($(LIBLINK),static)	
			CFG_CEXTRA	 := /MT /O2 /DNDEBUG=1 $(CFG_CEXTRA)
		else
			CFG_CEXTRA	 := /MD /O2 /DNDEBUG=1 $(CFG_CEXTRA)
		endif
		CFG_LEXTRA	 := 
		ifeq ($(CFG_STDLIBS),)
			CFG_STDLIBS	 := ole32.lib Oleaut32.lib user32.lib gdi32.lib comdlg32.lib comctl32.lib rpcrt4.lib shell32.lib Advapi32.lib
		endif
	endif


	# Tools
	CFG_PP := cl /nologo /DWIN32 /wd4996
	CFG_LD := link /NOLOGO
	CFG_CC := cl /nologo /DWIN32 /wd4996
	CFG_AR := lib /nologo
	
	CFG_DP := makedepend
	CFG_RM := rmdir /s /q
	CFG_DEL:= del /f /q
#	CFG_MD := md
	CFG_MD := $(PRJ_LIBROOT)/make_directory.bat

# +++ As to the line above, I have no clue why, but *sometimes*
#     make complains that the 'md' command cannot be found on 
#     Windows.  Moving it to a batch file seems to fix the problem.
#     BTW, it's *not* the embedded relative ellipsis, I suspected 
#     that too.

	CFG_CC_OUT := /Fo
	CFG_LD_OUT := /OUT:
	CFG_AR_OUT := /OUT:
	CFG_CC_INC := /I
	
	CFG_FLAG_EXPORT := /EXPORT:

	CFG_CFLAGS := /EHsc /c $(CFG_CEXTRA)
	CFG_LFLAGS := $(CFG_LEXTRA)
	
	ifeq ($(PRJ_TYPE),dll)
		CFG_LFLAGS := $(CFG_LFLAGS) /DLL
	endif

	#CFG_CUR_ROOT := $(shell cd)
	CFG_CUR_ROOT := $(shell pwd)

else

	# --with-sysroot
	# --with-headers

	ifdef DBG
		CFG_CEXTRA	 := -g -DDEBUG -D_DEBUG $(CFG_CEXTRA) 
		CFG_LEXTRA	 := -g
		CFG_DPOSTFIX := _d
	else
		CFG_CEXTRA	 := -O3 -s -DNDEBUG=1 $(CFG_CEXTRA) 
		ifneq ($(PRJ_TYPE),dll)
			CFG_LEXTRA	 := -s
		endif
	endif
	
	# Arm compiler
	ifeq ($(PROC),arm)

		ifeq ($(TOOLS),snapgear)
		
			OS := linux
			PLATFORM := posix
		
			# Snapgear
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/usr/local/bin/arm-linux-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/arm-linux

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSTRUCTINIT -DOEX_PACKBROKEN -DOEX_NOSHM -DOEX_NODL
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),codesourcery)

			OS := android
			PLATFORM := posix
			
			# Google Android
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/arm/bin/arm-none-eabi-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/arm/arm-none-eabi

			CFG_STDLIB := -lc -lstdc++ -lg
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_PACKBROKEN -DOEX_NODIRENT
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),nihilism)

			OS := linux
			PLATFORM := posix
			
			# nihilism
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/bin/arm-unknown-linux-gnu-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/arm-unknown-linux-gnu

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT -DOEX_PACKBROKEN -DOEX_NOVIDEO -DOEX_NODL
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),uclinux)

			OS := linux
			PLATFORM := posix
			
			# uclinux
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/usr/local/bin/arm-linux-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/usr/local/arm-linux

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT -DOEX_PACKBROKEN -DOEX_NOVIDEO -D__GCC_FLOAT_NOT_NEEDED
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq
			
		endif
		ifeq ($(TOOLS),openmoko)

			OS := linux
			PLATFORM := posix
			
			# openmoko
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/arm/bin/arm-angstrom-linux-gnueabi-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/arm/arm-angstrom-linux-gnueabi

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT -DOEX_NOVIDEO
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),armel)

			OS := linux
			PLATFORM := posix
			
			# armel
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/usr/bin/arm-linux-gnu-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/usr/

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT -DOEX_NOVIDEO
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),buildroot)

			OS := linux
			PLATFORM := posix
			
			# buildroot
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/build_arm/staging_dir/usr/bin/arm-linux-uclibcgnueabi-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/build_arm/staging_dir/

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT -DOEX_NOSTAT64 \
											    -DOEX_NOWCHAR -DOEX_NOEXECINFO
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),crosstool)

			OS := linux
			PLATFORM := posix
			
			# crosstool
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/usr/bin/arm-crosstool-linux-gnueabi-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/usr/arm-crosstool-linux-gnueabi

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)  
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT -DOEX_NOSTAT64 
										   -DOEX_NOWCHAR -DOEX_NOEXECINFO
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),scratchbox)

			OS := linux
			PLATFORM := posix
			
			# scratchbox/compilers/arm-linux-gcc3.4.cs-glibc2.3/bin/arm-linux-g++
			# scratchbox/compilers/arm-linux-gcc3.4.cs-glibc2.3/bin
			
			# martin's crosstool build
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/arm-gcc3.4-uclibc0.9.28/bin/arm-linux-
			#CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/arm-gcc3.4-uclibc0.9.28

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA) 
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM -DOEX_NOSTRUCTINIT \
												-DOEX_NOSTAT64 -DOEX_NOWCHAR -DOEX_NOEXECINFO -DOEX_PACKBROKEN
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD 
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),iphone)

			OS := darwin9
			PLATFORM := posix
			
			# iphone
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/tc/toolchain/pre/bin/arm-apple-darwin9-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/tc/toolchain/sys/

			CFG_STDLIB := 
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -DOEX_IPHONE -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSHM \
											    -DOEX_NOSTRUCTINIT -DOEX_NOSTAT64 -DOEX_NOVIDEO -DOEX_NOEPOLL
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),cegcc)

			OS := wince
			PLATFORM := windows
			
			# cegcc
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/bin/arm-wince-cegcc-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/arm-wince-cegcc/

			CFG_STDLIB := -lole32 
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -D_WIN32_WCE=0x0400 -DOEX_ARM -D__int64="long long" \
											    -DOEX_LOWRAM -DOEX_NOVIDEO -DOEX_NOCRTDEBUG -DOEX_NOXIMAGE 
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq
			
			CFG_EXE_POST := .exe			

		endif
		ifeq ($(TOOLS),mingw32ce)

			OS := wince
			PLATFORM := windows
			
			# mingw32ce
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/bin/arm-wince-mingw32ce-
			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/arm-wince-mingw32ce/

			CFG_STDLIB := -lole32 -laygshell -lwinsock -lws2
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -D_WIN32_WCE=0x0400 -DOEX_ARM -D__int64="long long" \
											    -DOEX_LOWRAM -DOEX_NOCRTDEBUG -DOEX_NODSHOW -DOEX_NOVFW
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

			CFG_EXE_POST := .exe			
			
		endif
		ifeq ($(TOOLS),)

	
			OS := linux
			PLATFORM := posix
			
			# Custom tools
			CFG_TOOLPREFIX := $(CFG_TOOLROOT)/$(TOOLS)/bin/$(TOOLS)-
#			CFG_SYSROOT := $(CFG_TOOLROOT)/$(TOOLS)/sysroot
			override TOOLS := custom

			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA)
			CFG_CFLAGS := $(CFG_CEXTRA) -c -fexceptions -MMD -DOEX_ARM -DOEX_LOWRAM -DOEX_NOSTRUCTINIT \
									                         -DOEX_PACKBROKEN -DOEX_NOSHM
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq
			
		endif

	else
	
		ifeq ($(TOOLS),mingw32)

			OS := win32
			PLATFORM := windows
			
			# Cross compile for windows
			CFG_TOOLPREFIX := i586-mingw32msvc-

			CFG_STDLIB := -lole32 -lgdi32 -lwsock32 -lws2_32
			CFG_LFLAGS := $(CFG_LEXTRA) -export-all-symbols 
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -Wall -fno-strict-aliasing \
										-DOEX_NODSHOW -DOEX_NOVFW -DOEX_NOCRTDEBUG -D__int64="long long"
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),mac)

			OS := apple
			PLATFORM := posix
			
			# Cross compile for mac
			CFG_TOOLPREFIX := 

			CFG_STDLIB := 
			CFG_LFLAGS := $(CFG_LEXTRA) 
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -Wall -fno-strict-aliasing \
									    -DOEX_NOSTRUCTINIT
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
		ifeq ($(TOOLS),local)

			OS := linux
			PLATFORM := posix
			
			# Local platform
			CFG_TOOLPREFIX := 

			# -lregex -lpng -ljpeg -lzlib -ltiff -lstdc++ -lgcc -lodbc32 -lwsock32 -lwinspool -lwinmm -lshell32 -lcomctl32 -lctl3d32 -lodbc32 -ladvapi32 -lodbc32 -lwsock32 -lopengl32 -lglu32 -lole32 -loleaut32
			
			CFG_STDLIB := -lrt -pthread
			CFG_LFLAGS := $(CFG_LEXTRA) -rdynamic -Wl,-E -Wl,--export-dynamic
			CFG_CFLAGS := $(CFG_CEXTRA) -c -MMD -Wall -fno-strict-aliasing
			CFG_SFLAGS := $(CFG_CFLAGS) -S -MMD
			CFG_AFLAGS := cq

		endif
	
	endif

	ifeq ($(PRJ_TYPE),dll)
		CFG_LFLAGS := $(CFG_LFLAGS) -shared
	else	
		ifeq ($(LIBLINK),static)
			CFG_LFLAGS := $(CFG_LFLAGS) -static -static-libgcc
		endif
	endif


	# you can't use dlopen() [-ldl] with static linking!
	# http://www.qnx.com/developers/docs/6.3.2/neutrino/lib_ref/d/dlopen.html
	ifeq ($(PLATFORM),posix)
#		ifeq ($(LIBLINK),shared)
			CFG_STDLIB := $(CFG_STDLIB) -ldl
#		endif		
	endif
	
#	ifeq ($(PRJ_TYPE),dll)
#		CFG_LD := $(CFG_TOOLPREFIX)ld -E --export-dynamic
#	else
#		ifeq ($(PRJ_TYPE),lib)
#			CFG_LD := $(CFG_TOOLPREFIX)ld -E --export-dynamic
#		else
#			CFG_LD := $(CFG_TOOLPREFIX)g++ -rdynamic -Wl,-E -Wl,--export-dynamic
#		endif
#	endif
	
	ifneq ($(CFG_SYSROOT),)
		CFG_SYSROOT_OPTIONS := --sysroot=$(CFG_SYSROOT)
	endif

	ifneq ($(CFG_SYSROOT_HEADERS),)
		CFG_SYSROOT_OPTIONS := $(CFG_SYSROOT_OPTIONS) --headers=$(CFG_SYSROOT_HEADERS)
	endif

	# Tools
	CFG_PP := $(CFG_TOOLPREFIX)g++ $(CFG_SYSROOT_OPTIONS)
	CFG_LD := $(CFG_TOOLPREFIX)g++
	CFG_CC := $(CFG_TOOLPREFIX)gcc $(CFG_SYSROOT_OPTIONS)
	CFG_AR := $(CFG_TOOLPREFIX)ar
	CFG_DT := $(CFG_TOOLPREFIX)dlltool
	CFG_DP := $(CFG_TOOLPREFIX)makedepend
	
	CFG_MD := mkdir -p
	CFG_RM := rm -rf
	CFG_DEL:= rm -f
	
	CFG_CC_OUT := -o $(nullstring)
	CFG_LD_OUT := -o $(nullstring)

	CFG_CUR_ROOT := $(shell pwd)
	
	CFG_CC_INC := -I
	
endif

ifeq ($(PLATFORM),windows)

	CFG_OBJ_EXT  := obj
	CFG_DEP_EXT  := d
	CFG_LIB_PRE	 :=
	CFG_LIB_POST := .lib
	CFG_EXE_POST := .exe
	CFG_DLL_POST := .dll
	
else

	CFG_OBJ_EXT := o
	CFG_DEP_EXT := d
	CFG_LIB_PRE	 := lib
	CFG_LIB_POST := .a
	CFG_DLL_PRE	 := lib
	CFG_DLL_POST := .so
	
endif

CFG_BUILD_TYPE := $(PLATFORM)-$(BUILD)-$(OS)-$(PROC)-$(TOOLS)

ifdef UNICODE
	CFG_BUILD_TYPE := $(CFG_BUILD_TYPE)-unicode
endif

ifdef DBG
	CFG_BUILD_TYPE := $(CFG_BUILD_TYPE)-debug
endif

ifeq ($(LIBLINK),static)
	CFG_BUILD_TYPE := $(CFG_BUILD_TYPE)-static
else
	CFG_BUILD_TYPE := $(CFG_BUILD_TYPE)-shared
endif

CFG_BINROOT  := $(CFG_ROOT)/bin/$(CFG_BUILD_TYPE)

ifdef PRJ_BINROOT
	CFG_OUTROOT := $(PRJ_BINROOT)/bin/$(CFG_BUILD_TYPE)
else
	CFG_OUTROOT  := $(CFG_BINROOT)
endif

ifdef PRJ_RESCMP
	CFG_RESCMP := $(CFG_OUTROOT)/$(CFG_EXE_PRE)$(PRJ_RESCMP)$(CFG_DPOSTFIX)$(CFG_EXE_POST)
else
	CFG_RESCMP  := $(CFG_OUTROOT)/$(CFG_EXE_PRE)resbld$(CFG_DPOSTFIX)$(CFG_EXE_POST)
endif

