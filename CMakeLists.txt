cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME			GpCore2)
set(PROJECT_VERSION			2.0.1)
set(PROJECT_DESCRIPTION		"C++ common utilities and types library")
set(PROJECT_NAME_DEFINE		GPCORE_LIBRARY)

set(CMAKE_BUILD_PARALLEL_LEVEL  1)
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES  *.cpp)
file(GLOB_RECURSE INCLUDES *.hpp)

if (BUILD_RELEASE_WASM32)
	set(NAME_OS						Browser)
	set(NAME_ARC					wasm32)
	set(CMAKE_BUILD_TYPE            Release)
	set(CMAKE_CXX_COMPILER          em++)
	set(CMAKE_C_COMPILER            emcc)
	set(CMAKE_AR                    emar)
	set(CMAKE_RANLIB                emranlib)
	set(EMSCRIPTEN_ROOT             $ENV{EMSDK}/upstream/emscripten)
	set(CMAKE_TOOLCHAIN_FILE        ${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake)
	set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> cqs <TARGET> <LINK_FLAGS> <OBJECTS>")
	set(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> cqs <TARGET> <LINK_FLAGS> <OBJECTS>")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-string-literal-operator-template")
	project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} DESCRIPTION ${PROJECT_DESCRIPTION} LANGUAGES CXX)
	add_library(${PROJECT_NAME} STATIC ${SOURCES})
elseif(BUILD_RELEASE_LINUX_x86_64)
	set(NAME_OS						Linux)
	set(NAME_ARC					x86_64)
	set(CMAKE_BUILD_TYPE            Release)
	set(CMAKE_CXX_COMPILER          g++-10)
	set(CMAKE_C_COMPILER            gcc-10)
	project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} DESCRIPTION ${PROJECT_DESCRIPTION} LANGUAGES CXX)
	add_library(${PROJECT_NAME} SHARED ${SOURCES})
	target_link_options(${PROJECT_NAME} INTERFACE "-Wl;--gc-sections")
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
			pthread
			boost_context
	)
else()
	message(FATAL_ERROR "Unknown build type, please set one of these:\n-DBUILD_RELEASE_WASM32=ON\n-DBUILD_RELEASE_LINUX_x86_64=ON")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections -fexceptions -fstrict-aliasing -fstack-clash-protection")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wdouble-promotion -Wswitch-default -Wswitch-enum -Wuninitialized")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing -Wfloat-equal -Wshadow -Wplacement-new -Wcast-align -Wconversion -Wlogical-op")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond -Wduplicated-branches -Wrestrict -Wnull-dereference -Wno-terminate")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -Wno-unused-command-line-argument -Wno-comment")

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
			${BUILD_TYPE}_BUILD
			${PROJECT_NAME_DEFINE}
			HAVE_NETINET_IN_H
			_GLIBCXX_USE_NANOSLEEP
	)

#------------------------ FILES LOCATIONS ---------------------
set(CMAKE_INSTALL_PREFIX	"${CMAKE_SOURCE_DIR}/../.." CACHE PATH "..." FORCE)
set(TARGET_LIB_DIR          "${CMAKE_INSTALL_PREFIX}/bin")
set(TARGET_INCLUDE_DIR		"${CMAKE_INSTALL_PREFIX}/inc")

if (NOT EXISTS "${TARGET_INCLUDE_DIR}")
	file(MAKE_DIRECTORY "${TARGET_INCLUDE_DIR}")
endif()

set(SP _)
set(OUT_BIN_DIR "${TARGET_LIB_DIR}/${CMAKE_BUILD_TYPE}${SP}${NAME_OS}${SP}${NAME_ARC}")

#------------------------ LIBS ---------------------
target_include_directories(${PROJECT_NAME}
	PRIVATE
		"${TARGET_INCLUDE_DIR}"
	)

target_link_directories(${PROJECT_NAME}
	PRIVATE
		${OUT_BIN_DIR}
)

set_target_properties(${PROJECT_NAME}
						PROPERTIES
							ARCHIVE_OUTPUT_DIRECTORY				${OUT_BIN_DIR}
							LIBRARY_OUTPUT_DIRECTORY				${OUT_BIN_DIR}
							RUNTIME_OUTPUT_DIRECTORY				${OUT_BIN_DIR}
							VERSION									${PROJECT_VERSION}
							SOVERSION								${PROJECT_VERSION_MAJOR}
							INTERFACE_${PROJECT_NAME}_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})

message(STATUS "[${PROJECT_NAME}]: ***************** Build mode ${BUILD_TYPE} *****************")
message(STATUS "[${PROJECT_NAME}]: Target name:      " ${PROJECT_NAME}${MODE_SUFFIX} " version " ${PROJECT_VERSION})
message(STATUS "[${PROJECT_NAME}]: Target arch:      " ${NAME_ARC})
message(STATUS "[${PROJECT_NAME}]: Target OS:        " ${NAME_OS})
message(STATUS "[${PROJECT_NAME}]: Build directory:  " ${OUT_BIN_DIR})
message(STATUS "[${PROJECT_NAME}]: C++ standart:     " ${CMAKE_CXX_STANDARD})
message(STATUS "[${PROJECT_NAME}]: GCC C compiler:   " ${CMAKE_C_COMPILER})
message(STATUS "[${PROJECT_NAME}]: GCC C++ compiler: " ${CMAKE_CXX_COMPILER})
message(STATUS "[${PROJECT_NAME}]: GCC linker:       " ${CMAKE_LINKER})

#------------------------ INSTALL ---------------------
install(DIRECTORY        "${CMAKE_SOURCE_DIR}"
		DESTINATION      "${TARGET_INCLUDE_DIR}"
		FILES_MATCHING
				PATTERN "*.hpp"
				PATTERN "*.h"
				PATTERN "CMake*" EXCLUDE
				PATTERN ".git"   EXCLUDE
		)
