#qmake options for DEBUG:   CONFIG+=debug_build   CONFIG+=os_linux CONFIG+=compiler_gcc CONFIG+=arc_x86_64
#qmake options for RELEASE: CONFIG+=release_build CONFIG+=os_linux CONFIG+=compiler_gcc CONFIG+=arc_x86_64

CONFIG		-= qt
QT			-= core gui widgets
CONFIG		+= warn_on
DEFINES		+= HAVE_NETINET_IN_H \
			   _GLIBCXX_USE_NANOSLEEP

compiler_gcc{
	QMAKE_CC		=	gcc-11
	QMAKE_CXX		=	g++-11
	QMAKE_LINK		=	g++-11
}else:compiler_clang{
	QMAKE_CXXFLAGS	+= -stdlib=libc++
	QMAKE_LFLAGS    += -stdlib=libc++

	#QMAKE_CXXFLAGS	+= -stdlib=libstdc++
	#QMAKE_LFLAGS   += -stdlib=libstdc++
}else:compiler_emscripten{
}else{
	error("Unknown compiler mode. Set CONFIG+=compiler_gcc OR CONFIG+=compiler_clang OR CONFIG+=compiler_emscripten")
}

#c++20
CONFIG			+=	c++20
CONFIG			+=	c17
QMAKE_CXXFLAGS	+=	-std=gnu++20

QMAKE_CXXFLAGS	+= -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_CXXFLAGS	+= -ffunction-sections -fdata-sections -fexceptions -fstrict-aliasing -fstack-clash-protection
QMAKE_CXXFLAGS	+= -Wall -Wextra -Wno-comment -Wdouble-promotion -Wswitch-default -Wswitch-enum -Wuninitialized
QMAKE_CXXFLAGS	+= -Wstrict-aliasing -Wfloat-equal -Wshadow -Wplacement-new -Wcast-align -Wconversion -Wlogical-op
QMAKE_CXXFLAGS	+= -Wduplicated-cond -Wduplicated-branches -Wrestrict -Wnull-dereference -Wno-terminate
#QMAKE_CXXFLAGS	+= -fno-rtti
QMAKE_LFLAGS    += -Wl,--gc-sections

#sanitizers = use_tsan
sanitizers = use_asan_usan

#------------------------ DEBUG or RELEASE ---------------------
debug_build {
	message([$$PACKET_NAME]: ***************** Build mode DEBUG *****************)
	DEFINES			   += DEBUG_BUILD
	TARGET_POSTFIX		= _d
	OUT_BUILD_MODE_PATH	= Debug

	BOOST_POSTFIX		= _fcontext

	equals(sanitizers, "use_tsan") {
		message([$$PACKET_NAME]: Use sanitizers: tsan)
		OUT_BUILD_MODE_PATH	= DebugTsan

		DEFINES			+= BOOST_USE_TSAN
		QMAKE_CXXFLAGS	+= -fsanitize=thread -fno-omit-frame-pointer -fno-optimize-sibling-calls
		LIBS			+= -ltsan

		DEFINES		   += BOOST_USE_UCONTEXT
		BOOST_POSTFIX	= _ucontext_tsan
	}

	equals(sanitizers, "use_asan_usan") {
		message([$$PACKET_NAME]: Use sanitizers: asan usan)
		OUT_BUILD_MODE_PATH	= DebugAsanUsan

		DEFINES	+= BOOST_USE_ASAN
		QMAKE_CXXFLAGS	+= -fsanitize=address -fsanitize=undefined -fno-sanitize=vptr
		QMAKE_CXXFLAGS	+= -fsanitize-recover=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
		LIBS			+= -lasan
		LIBS			+= -lubsan

		DEFINES		   += BOOST_USE_UCONTEXT
		BOOST_POSTFIX	= _ucontext_asan_usan
	}

	os_browser {
		QMAKE_LFLAGS += -s DISABLE_EXCEPTION_CATCHING=0
	}
} else:profile_build {
	message([$$PACKET_NAME]: ***************** Build mode PROFILE *****************)
	DEFINES			   += DEBUG_BUILD
	DEFINES			   += PROFILE_BUILD
	TARGET_POSTFIX		= _d
	OUT_BUILD_MODE_PATH	= Profile

	BOOST_POSTFIX		= _fcontext
} else:release_build {
	message([$$PACKET_NAME]: ***************** Build mode RELEASE *****************)
	DEFINES			    += RELEASE_BUILD
	TARGET_POSTFIX		=
	OUT_BUILD_MODE_PATH	= Release

	BOOST_POSTFIX		= _fcontext

	os_browser {
		QMAKE_LFLAGS += -s DISABLE_EXCEPTION_CATCHING=0
	}
} else {
	error("Unknown build mode. Set CONFIG+=debug_build CONFIG+=profile_build OR CONFIG+=release_build")
}

#------------------------ OS ---------------------
os_linux {
	OUT_BUILD_OS_PATH = Linux
} else:os_android {
	OUT_BUILD_OS_PATH = Android
} else:os_ios {
	OUT_BUILD_OS_PATH = Ios
} else:os_windows {
	OUT_BUILD_OS_PATH = Win
} else:os_macx {
	OUT_BUILD_OS_PATH = Macx
} else:os_browser {
	OUT_BUILD_OS_PATH = Browser
} else {
	error("Unknown OS. Set CONFIG+=... one of values: os_linux, os_android, os_ios, os_windows, os_macx, os_browser, os_baremetal")
}

#------------------------ ARC ---------------------
arc_x86_64 {
	OUT_BUILD_ARCH_PATH = x86_64
	QMAKE_CXXFLAGS	+= -mtune=generic -march=x86-64
} else:arc_x86 {
	OUT_BUILD_ARCH_PATH = x86
	QMAKE_CXXFLAGS	+= -mtune=generic -march=i686
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

TARGET = $$PACKET_NAME$$TARGET_POSTFIX
OUT_BUILD_PATH  = $$DIR_LEVEL/../../../Bin_tmp/

SP = _
DESTDIR = $$OUT_BUILD_PATH$$OUT_BUILD_MODE_PATH$$SP$$OUT_BUILD_OS_PATH$$SP$$OUT_BUILD_ARCH_PATH/

message([$$PACKET_NAME]: Target name: $$TARGET)
message([$$PACKET_NAME]: Target arch: $$OUT_BUILD_ARCH_PATH)
message([$$PACKET_NAME]: Target OS:   $$OUT_BUILD_OS_PATH)
message([$$PACKET_NAME]: Build directory $$DESTDIR)
message([$$PACKET_NAME]: -------------------------------------------------)

LIBS += -L$$DESTDIR

INCLUDEPATH += \
	$$DIR_LEVEL/../Extras \
	$$DIR_LEVEL/../Extras/Boost/boost_1_77_0$$BOOST_POSTFIX
