#qmake options for DEBUG:   CONFIG+=debug_build   CONFIG+=arc_x86_64  CONFIG+=os_linux CONFIG+=compiler_clang CONFIG+=use_asan_ubsan
#qmake options for RELEASE: CONFIG+=release_build CONFIG+=arc_x86_64  CONFIG+=os_linux CONFIG+=compiler_clang

CONFIG		-= qt
QT			-= core gui widgets
CONFIG		+= warn_on
DEFINES		+= HAVE_NETINET_IN_H \
			   _GLIBCXX_USE_NANOSLEEP

# ------------ vars for use in equals() -----------------
use_asan_ubsan {
	var_sanitizers = use_asan_ubsan
}

use_tsan {
	var_sanitizers = use_tsan
}

os_linux {
   var_os = linux
}

os_browser {
	var_os = browser
}

os_windows {
	var_os = windows
}

compiler_gcc{
	var_compiler = gcc
}

compiler_clang{
	var_compiler = clang
}

compiler_msvc{
	var_compiler = msvc
}

compiler_emscripten{
	var_compiler = emscripten
}

# -----------------------------

compiler_gcc{
	QMAKE_CC		=	gcc-12
	QMAKE_CXX		=	g++-12
	QMAKE_LINK		=	g++-12

	equals(var_os, "linux") {
		QMAKE_CXXFLAGS	+= -Wplacement-new -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wrestrict -Wno-terminate
		QMAKE_CXXFLAGS	+= -fstack-clash-protection
	}
	QMAKE_CXXFLAGS	+= -Wextra -Wno-comment -Wdouble-promotion -Wswitch-enum -Wuninitialized -Wfloat-equal -Wshadow -Wcast-align -Wconversion -Wnull-dereference -Wno-switch-default
	QMAKE_CXXFLAGS	+= -fstrict-aliasing -Wstrict-aliasing -ffunction-sections -fdata-sections -fexceptions
	QMAKE_CXXFLAGS	+= -fvisibility=hidden -fvisibility-inlines-hidden
}else:compiler_clang{
	QMAKE_CC		=	clang-16
	QMAKE_CXX		=	clang++-16
	QMAKE_LINK		=	clang++-16

	#QMAKE_CXXFLAGS += -stdlib=libc++
	#QMAKE_LFLAGS   += -stdlib=libc++

	QMAKE_CXXFLAGS	+= -stdlib=libstdc++
	QMAKE_LFLAGS    += -stdlib=libstdc++

	QMAKE_CXXFLAGS	+= -Werror -Wextra -Wdouble-promotion -Wswitch-enum -Wuninitialized -Wfloat-equal -Wshadow -Wcast-align -Wconversion -Wnull-dereference -Wno-switch-default -Wno-comment -Wthread-safety-analysis -Wthread-safety
	QMAKE_CXXFLAGS	+= -fstrict-aliasing -Wstrict-aliasing -ffunction-sections -fdata-sections -fexceptions
	QMAKE_CXXFLAGS	+= -fvisibility=hidden -fvisibility-inlines-hidden
}else:compiler_msvc{
	#QMAKE_CXXFLAGS += -fpermissive-
	QMAKE_CXXFLAGS += -std:c++latest
}else:compiler_emscripten{
	# NOP
}else{
	error("Unknown compiler mode. Set CONFIG+=[compiler_gcc,compiler_clang,compiler_msvc,compiler_emscripten]")
}

#c++20
CONFIG			+=	c++latest
CONFIG			+=	c17
#QMAKE_CXXFLAGS	+=	-std=gnu++20

QMAKE_CXXFLAGS	+= -Wall
#QMAKE_LFLAGS    += -Wl,-E,--gc-sections -v

os_linux {
    #For symbol info
	QMAKE_CXXFLAGS += -g
	BOOST_POSTFIX	= _fcontext
}

os_browser {
	QMAKE_LFLAGS += -s DISABLE_EXCEPTION_CATCHING=0
}

os_windows {
	BOOST_POSTFIX	= _fcontext
}

# ------------------------ DEBUG or RELEASE ---------------------
debug_build {
	message([$$PACKET_NAME]: ***************** Build mode DEBUG *****************)
	DEFINES			   += DEBUG_BUILD
	TARGET_POSTFIX		= _d
	OUT_BUILD_MODE_PATH	= Debug

	equals(var_sanitizers, "use_tsan") {		
		var_sanitizers      = "tsan"
		OUT_BUILD_MODE_PATH	= DebugTsan

		DEFINES			+= BOOST_USE_TSAN
		DEFINES		    += BOOST_USE_UCONTEXT
		BOOST_POSTFIX	= _ucontext_tsan

		QMAKE_CXXFLAGS	+= -fsanitize=thread -fno-omit-frame-pointer -fno-optimize-sibling-calls
		LIBS			+= -ltsan
	}

	equals(var_sanitizers, "use_asan_ubsan") {
		var_sanitizers      = "asan, ubsan"
		OUT_BUILD_MODE_PATH	= DebugAsanUBsan

		DEFINES	+= BOOST_USE_ASAN
		DEFINES	+= BOOST_USE_UBSAN
		DEFINES += BOOST_USE_UCONTEXT
		BOOST_POSTFIX	= _ucontext_asan_ubsan

		QMAKE_CXXFLAGS	+= -fsanitize=address -fsanitize=undefined -fno-sanitize=vptr
		QMAKE_CXXFLAGS	+= -fsanitize-recover=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
		LIBS			+= -lasan
		LIBS			+= -lubsan
	}
} else:profile_build {
	message([$$PACKET_NAME]: ***************** Build mode PROFILE *****************)
	DEFINES	+= DEBUG_BUILD
	DEFINES	+= PROFILE_BUILD

	TARGET_POSTFIX		= _d
	OUT_BUILD_MODE_PATH	= Profile
} else:release_build {
	message([$$PACKET_NAME]: ***************** Build mode RELEASE *****************)
	DEFINES	+= RELEASE_BUILD

	TARGET_POSTFIX		=
	OUT_BUILD_MODE_PATH	= Release

	equals(var_compiler, "clang") {
		QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64-v3
		QMAKE_CXXFLAGS	+= -flto
		QMAKE_LFLAGS    += -flto
	}
	equals(var_compiler, "gcc") {
		QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64-v3
		QMAKE_CXXFLAGS	+= -flto
		QMAKE_LFLAGS    += -flto
	}
	equals(var_compiler, "msvc") {

	}
} else {
	error("Unknown build mode. Set CONFIG+=debug_build CONFIG+=profile_build OR CONFIG+=release_build")
}

# ------------------------ OS ---------------------
os_linux {
	OUT_BUILD_OS_PATH = Linux
} else:os_android {
	OUT_BUILD_OS_PATH = Android
} else:os_ios {
	OUT_BUILD_OS_PATH = Ios
} else:os_windows {
	OUT_BUILD_OS_PATH = Windows
} else:os_macx {
	OUT_BUILD_OS_PATH = Macx
} else:os_browser {
	OUT_BUILD_OS_PATH = Browser
} else {
	error("Unknown OS. Set CONFIG+=... one of values: os_linux, os_android, os_ios, os_windows, os_macx, os_browser, os_baremetal")
}

# ------------------------ ARC ---------------------
arc_x86_64 {
	OUT_BUILD_ARCH_PATH = x86_64

	equals(var_compiler, "clang") {
		QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64-v3
	}
	equals(var_compiler, "gcc") {
		QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64-v3
	}
} else:arc_x86 {
	OUT_BUILD_ARCH_PATH = x86

	equals(var_compiler, "clang") {
		QMAKE_CXXFLAGS	+= -mtune=generic -march=i686
	}
	equals(var_compiler, "gcc") {
		QMAKE_CXXFLAGS	+= -mtune=generic -march=i686
	}
} else:arc_arm_v6 {
	OUT_BUILD_ARCH_PATH = arm_v6
} else:arc_armeabi_v7a {
	OUT_BUILD_ARCH_PATH = armeabi_v7a
} else:arc_arm64_v8a {
	OUT_BUILD_ARCH_PATH = arm64_v8a
} else:arc_mips {
	OUT_BUILD_ARCH_PATH = mips
} else:arc_wasm32 {
	OUT_BUILD_ARCH_PATH = wasm32
} else:arc_wasm64 {
	OUT_BUILD_ARCH_PATH = wasm64
} else {
	error("Unknown ARC. Set CONFIG+=... one of values: arc_x86_64, arc_x86, arc_arm_v6, arc_armeabi_v7a, arc_arm64_v8a, arc_mips, arc_wasm32, arc_wasm64")
}

# ----------- Version -----------
os_linux {
	VER_MAJ = $$_VER_MAJ
	VER_MIN = $$_VER_MIN
	VER_PAT = $$_VER_PAT
}

DEFINES += "GP_CURRENT_LIB_VER_MAJ=\\\"$$_VER_MAJ\\\""
DEFINES += "GP_CURRENT_LIB_VER_MIN=\\\"$$_VER_MIN\\\""
DEFINES += "GP_CURRENT_LIB_VER_PAT=\\\"$$_VER_PAT\\\""
DEFINES += "GP_CURRENT_LIB_PACKET_NAME=\\\"$$PACKET_NAME\\\""

# ----------- Path -----------

TARGET = $$PACKET_NAME$$TARGET_POSTFIX
OUT_BUILD_PATH  = $$DIR_LEVEL/../../../Bin_tmp/

SP = _
DESTDIR                = $$OUT_BUILD_PATH$$OUT_BUILD_MODE_PATH$$SP$$OUT_BUILD_OS_PATH$$SP$$OUT_BUILD_ARCH_PATH/
INCLUSE_EXTRAS_DESTDIR = $$DIR_LEVEL/../../../../../Extras/

message([$$PACKET_NAME]: -------------------------------------------------)
message([$$PACKET_NAME]: Target name:     $$TARGET)
message([$$PACKET_NAME]: Target arch:     $$OUT_BUILD_ARCH_PATH)
message([$$PACKET_NAME]: Target OS:       $$OUT_BUILD_OS_PATH)
message([$$PACKET_NAME]: Compiler:        $$var_compiler)
message([$$PACKET_NAME]: Use sanitizers:  $$var_sanitizers)
message([$$PACKET_NAME]: Build dir:       $$DESTDIR)
message([$$PACKET_NAME]: Inc. extras dir: $$INCLUSE_EXTRAS_DESTDIR)
message([$$PACKET_NAME]: -------------------------------------------------)

LIBS += -L$$DESTDIR

INCLUDEPATH += \
	$$DIR_LEVEL/../../../../../Extras/Boost/boost_1_84_0$$BOOST_POSTFIX \
	$$DIR_LEVEL/../../../../../Extras/fmt/include \	
	$$DIR_LEVEL/../../../../../Extras \
	$$DIR_LEVEL/../GPlatform \
	$$DIR_LEVEL/../ \

os_windows {
#	INCLUDEPATH += \
#		c:/msys64/usr/include/w32api/
}
