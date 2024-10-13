# OS:           os_linux|os_windows|os_browser
# Architecture: architecture_x86_64|architecture_x86|architecture_wasm32|architecture_wasm64
# Compiler:     compiler_gcc|compiler_clang|compiler_msvc|compiler_emscripten
# Build mode:   build_release|build_debug
# Link mode:    link_shared|link_static
# Sanitizers:   sanitizers_asan_ubsan|sanitizers_tsan

# Windows_x86_64_clang_debug_shared: CONFIG+=build_debug   CONFIG+=os_windows CONFIG+=architecture_x86_64 CONFIG+=compiler_clang CONFIG+=link_shared
# Windows_x86_64_clang_debug_static: CONFIG+=build_release CONFIG+=os_windows CONFIG+=architecture_x86_64 CONFIG+=compiler_clang CONFIG+=link_shared

# Linux_x86_64_clang_debug_shared:            CONFIG+=build_debug   CONFIG+=os_linux CONFIG+=architecture_x86_64 CONFIG+=compiler_clang CONFIG+=link_shared
# Linux_x86_64_clang_debug_shared_asan_ubsan: CONFIG+=build_debug   CONFIG+=os_linux CONFIG+=architecture_x86_64 CONFIG+=compiler_clang CONFIG+=link_shared CONFIG+=sanitizers_asan_ubsan
# Linux_x86_64_clang_debug_shared_tsan:       CONFIG+=build_debug   CONFIG+=os_linux CONFIG+=architecture_x86_64 CONFIG+=compiler_clang CONFIG+=link_shared CONFIG+=sanitizers_tsan
# Linux_x86_64_clang_release_shared:          CONFIG+=build_release CONFIG+=os_linux CONFIG+=architecture_x86_64 CONFIG+=compiler_clang CONFIG+=link_shared
# Linux_x86_64_clang_release_static:          CONFIG+=build_release CONFIG+=os_linux CONFIG+=architecture_x86_64 CONFIG+=compiler_clang CONFIG+=link_static

# ============================================= COMMON =============================================
# ------------ remove Qt ------------
CONFIG			-= qt
QT				-= core gui widgets

# ------------ C++ version ------------
CONFIG			+=	c++20
CONFIG			+=	c17

# ------------ Default warning serrings ------------
CONFIG			+= warn_on
QMAKE_CXXFLAGS	+= -Wall

# ------------ Boost ------------
BOOST_POSTFIX	 = _fcontext
BOOST_VERSION	 = 1_86_0

# ------------ Postfix ------------
TARGET_POSTFIX	 =

# ------------ Version defines ------------
DEFINES			+= "GP_CURRENT_LIB_VER_MAJ=\\\"$$_VER_MAJ\\\""
DEFINES			+= "GP_CURRENT_LIB_VER_MIN=\\\"$$_VER_MIN\\\""
DEFINES			+= "GP_CURRENT_LIB_VER_PAT=\\\"$$_VER_PAT\\\""
DEFINES			+= "GP_CURRENT_LIB_PACKET_NAME=\\\"$$PACKET_NAME\\\""

# ============================================= VARS =============================================
# ------------ vars (os) ------------
os_linux {
   var_os = "linux"
}

os_windows {
	var_os = "windows"
}

os_browser {
	var_os = "browser"
}

equals(var_os, "") {
    error(Unknown OS. Set CONFIG+=os_linux|os_windows|os_browser)
}

# ------------ vars (architecture) ------------
architecture_x86_64 {
	var_architecture = "x86_64"
}

architecture_x86 {
	var_architecture = "x86"
}

architecture_wasm32 {
	var_architecture = "wasm32"
}

architecture_wasm64 {
	var_architecture = "wasm64"
}

equals(var_architecture, "") {
    error(Unknown architecture. Set CONFIG+=architecture_x86_64|architecture_x86|architecture_wasm32|architecture_wasm64)
}

# ------------ vars (compiler) ------------
compiler_gcc{
	var_compiler = "gcc"
}

compiler_clang{
	var_compiler = "clang"
}

compiler_msvc{
	var_compiler = "msvc"
}

compiler_emscripten{
	var_compiler = "emscripten"
}

equals(var_compiler, "") {
    error(Unknown compiler. Set CONFIG+=compiler_gcc|compiler_clang|compiler_msvc|compiler_emscripten)
}

# ------------ vars (build) ------------
build_release{
	var_build = "release"
}

build_debug{
	var_build = "debug"
}

equals(var_build, "") {
    error(Unknown build mode. Set CONFIG+=build_release|build_debug)
}

# ------------ vars (link) ------------
link_shared{
	var_link = "shared"
}

link_static{
	var_link = "static"
}

equals(var_link, "") {
    error(Unknown link mode. Set CONFIG+=link_shared|link_static)
}

# ------------ vars (sanitizers) ------------
sanitizers_asan_ubsan {
	var_sanitizers = "asan_ubsan"
}

sanitizers_tsan {
	var_sanitizers = "tsan"
}

# ============================================= SETTINGS =============================================
# ------------ settings (os) ------------
equals(var_os, "linux") {
	OUT_PATH_OS = "Linux"

    #QMAKE_CXXFLAGS	+= -g
}

equals(var_os, "windows") {
	OUT_PATH_OS = "Windows"
}

equals(var_os, "browser") {
	OUT_PATH_OS = "Browser"
}

# ------------ settings (architecture) ------------
equals(var_architecture, "x86_64") {
	OUT_PATH_ARCHITECTURE = "x86_64"

    equals(var_compiler, "clang") {
	    QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64-v3
	}

    equals(var_compiler, "gcc") {
	    QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64-v3
	}

    equals(var_compiler, "msvc") {
	    QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64-v3
	}

    equals(var_compiler, "emscripten") {
	    error(Unsupported compiler)
	}
}

equals(var_architecture, "x86") {
	OUT_PATH_ARCHITECTURE = "x86"

    equals(var_compiler, "clang") {
	    QMAKE_CXXFLAGS	+= -mtune=generic -march=i686
	}

    equals(var_compiler, "gcc") {
	    QMAKE_CXXFLAGS	+= -mtune=generic -march=i686
	}

    equals(var_compiler, "msvc") {
	    QMAKE_CXXFLAGS	+= -mtune=generic -march=i686
	}

    equals(var_compiler, "emscripten") {
	    error(Unsupported compiler)
	}
}

equals(var_architecture, "wasm32") {
	OUT_PATH_ARCHITECTURE = "wasm32"

    equals(var_compiler, "clang") {
	    error(Unsupported compiler)
	}

    equals(var_compiler, "gcc") {
	    error(Unsupported compiler)
	}

    equals(var_compiler, "msvc") {
	    error(Unsupported compiler)
	}

    equals(var_compiler, "emscripten") {
	    # NOP
	}
}

equals(var_architecture, "wasm64") {
	OUT_PATH_ARCHITECTURE = "wasm64"

    equals(var_compiler, "clang") {
	    error(Unsupported compiler)
	}

    equals(var_compiler, "gcc") {
	    error(Unsupported compiler)
	}

    equals(var_compiler, "msvc") {
	    error(Unsupported compiler)
	}

    equals(var_compiler, "emscripten") {
	    # NOP
	}
}

# ------------ settings (compiler) ------------
equals(var_compiler, "gcc") {
    # ------------ Tools name ------------
	OUT_PATH_COMPILER	 = "gcc"
	COMPILER_VERSION	 = "-12"

    QMAKE_CC			 = gcc$$COMPILER_VERSION
	QMAKE_CXX			 = g++$$COMPILER_VERSION
	QMAKE_LINK			 = g++$$COMPILER_VERSION
	#QMAKE_LFLAGS		+=

    # ------------ Flags ------------
	QMAKE_CXXFLAGS	+= -Wplacement-new -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wrestrict -Wno-terminate
	QMAKE_CXXFLAGS	+= -fstack-clash-protection
	QMAKE_CXXFLAGS	+= -Wextra -Wno-comment -Wdouble-promotion -Wswitch-enum -Wuninitialized -Wfloat-equal -Wshadow -Wcast-align -Wconversion -Wnull-dereference -Wno-switch-default
	QMAKE_CXXFLAGS	+= -fstrict-aliasing -Wstrict-aliasing -ffunction-sections -fdata-sections -fexceptions

    QMAKE_LFLAGS	+= -Wl,--no-undefined
}

equals(var_compiler, "clang") {
    # ------------ Tools name ------------
	OUT_PATH_COMPILER	 = "clang"
	COMPILER_VERSION	 = "-18"

    QMAKE_CC			 = clang$$COMPILER_VERSION
	QMAKE_CXX			 = clang++$$COMPILER_VERSION
	QMAKE_LINK			 = clang++$$COMPILER_VERSION
	QMAKE_LFLAGS		+= -fuse-ld=lld$$COMPILER_VERSION

    # ------------ Flags ------------
	QMAKE_CXXFLAGS	+= -Werror -Wextra -Wdouble-promotion -Wswitch-enum -Wuninitialized -Wfloat-equal -Wshadow -Wcast-align -Wconversion -Wnull-dereference -Wno-switch-default -Wno-comment -Wthread-safety-analysis -Wthread-safety
	QMAKE_CXXFLAGS	+= -fstrict-aliasing -Wstrict-aliasing -ffunction-sections -fdata-sections -fexceptions

    QMAKE_LFLAGS	+= -Wl,--no-undefined

    # ------------ STD library implementation ------------
	#QMAKE_CXXFLAGS += -stdlib=libc++
	#QMAKE_LFLAGS   += -stdlib=libc++
	QMAKE_CXXFLAGS	+= -stdlib=libstdc++
	QMAKE_LFLAGS    += -stdlib=libstdc++
}

equals(var_compiler, "msvc") {
	OUT_PATH_COMPILER = "msvc"
}

equals(var_compiler, "emscripten") {
	OUT_PATH_COMPILER = "emscripten"

    QMAKE_LFLAGS			+= -s DISABLE_EXCEPTION_CATCHING=0 -s WASM=1
	QMAKE_CXXFLAGS_RELEASE	 = -O3
	QMAKE_CFLAGS_RELEASE	 = -O3
}

# ------------ settings (build) ------------
equals(var_build, "release") {
	OUT_PATH_BUILD	 = "release"
	DEFINES			+= RELEASE_BUILD
	TARGET_POSTFIX	 =

    equals(var_compiler, "clang") {
	    QMAKE_CXXFLAGS	+= -flto
		QMAKE_LFLAGS    += -flto
	}
	equals(var_compiler, "gcc") {
	    QMAKE_CXXFLAGS	+= -flto
		QMAKE_LFLAGS    += -flto
	}
	equals(var_compiler, "emscripten") {
	    QMAKE_CXXFLAGS	+= -flto
		QMAKE_LFLAGS    += -flto
	}
	equals(var_compiler, "msvc") {
	    QMAKE_CXXFLAGS	+= -flto
		QMAKE_LFLAGS    += -flto
	}
}

equals(var_build, "debug") {
	OUT_PATH_BUILD		= "debug"
	DEFINES			   += DEBUG_BUILD
	TARGET_POSTFIX		= _d
}

# ------------ settings (link) ------------
equals(var_link, "shared") {
	OUT_PATH_LINK	 = "shared"
	DEFINES			+= SHARED_LINK

	equals(var_compiler, "clang") {
		QMAKE_CXXFLAGS	+= -fvisibility=hidden -fvisibility-inlines-hidden
	}
	equals(var_compiler, "gcc") {
		QMAKE_CXXFLAGS	+= -fvisibility=hidden -fvisibility-inlines-hidden
	}
}

equals(var_link, "static") {
	OUT_PATH_LINK	 = "static"
	DEFINES			+= STATIC_LINK

	equals(var_compiler, "clang") {
		#QMAKE_CXXFLAGS	+= -ffat-lto-objects
	}
	equals(var_compiler, "gcc") {
		#QMAKE_CXXFLAGS	+= -ffat-lto-objects
	}
	equals(var_compiler, "emscripten") {
		#QMAKE_CXXFLAGS	+= -ffat-lto-objects
	}
}

# ------------ settings (sanitizers) ------------
equals(var_sanitizers, "asan_ubsan") {
	OUT_PATH_SANITIZERS = "asan_ubsan"
	BOOST_POSTFIX		= _ucontext_asan

    DEFINES            += BOOST_USE_ASAN
	DEFINES            += BOOST_USE_UCONTEXT
	QMAKE_CXXFLAGS	   += -fsanitize=address -fno-sanitize=vptr -fsanitize-recover=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
	LIBS			   += -lasan
}

equals(var_sanitizers, "tsan") {
	OUT_PATH_SANITIZERS = "tsan"
	BOOST_POSTFIX		= _ucontext_tsan

    DEFINES			   += BOOST_USE_TSAN
	DEFINES		       += BOOST_USE_UCONTEXT
	QMAKE_CXXFLAGS	   += -fsanitize=thread #-fno-omit-frame-pointer -fno-optimize-sibling-calls
	QMAKE_LFLAGS	   += -fsanitize=thread
	#LIBS			   += -ltsan
}

# ------------ Binary out path (DESTDIR) ------------
SP = _
BINARY_OUT_PATH = $$DIR_LEVEL/../../../Bin/$$OUT_PATH_OS$$SP$$OUT_PATH_ARCHITECTURE$$SP$$OUT_PATH_COMPILER$$COMPILER_VERSION$$SP$$OUT_PATH_BUILD$$SP$$OUT_PATH_LINK

!equals(var_sanitizers, "") {
    BINARY_OUT_PATH = $$BINARY_OUT_PATH$$SP$$OUT_PATH_SANITIZERS
}

DESTDIR = $$BINARY_OUT_PATH/

# ------------ Binary out name (TARGET) ------------
VER_MAJ = $$_VER_MAJ
VER_MIN = $$_VER_MIN
VER_PAT = $$_VER_PAT

TARGET = $$PACKET_NAME$$TARGET_POSTFIX

# ------------ Log message ------------
message("[$$PACKET_NAME]: -------------------------------------------------")
message("[$$PACKET_NAME]: Target name:         $$TARGET")
message("[$$PACKET_NAME]: Build dir:           $$DESTDIR")
message("[$$PACKET_NAME]: Target OS:           $$OUT_PATH_OS")
message("[$$PACKET_NAME]: Target architecture: $$OUT_PATH_ARCHITECTURE")
message("[$$PACKET_NAME]: Compiler:            $$OUT_PATH_COMPILER$$COMPILER_VERSION")
message("[$$PACKET_NAME]: Build:               $$OUT_PATH_BUILD")
message("[$$PACKET_NAME]: Link:                $$OUT_PATH_LINK")
message("[$$PACKET_NAME]: Sanitizers:          $$OUT_PATH_SANITIZERS")
message("[$$PACKET_NAME]: -------------------------------------------------")

# ------------ Libraries path ------------
LIBS += -L$$DESTDIR

# ------------ Include path ------------
INCLUDEPATH += \
    $$DIR_LEVEL/../../../../../Extras/Boost/boost_$$BOOST_VERSION$$BOOST_POSTFIX \
	$$DIR_LEVEL/../../../../../Extras/fmt/include \
	$$DIR_LEVEL/../../../../../Extras \
	$$DIR_LEVEL/../GPlatform \
	$$DIR_LEVEL/../
