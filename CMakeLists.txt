cmake_minimum_required(VERSION 3.16)

project(GpCore2
	VERSION 0.1.0
        DESCRIPTION "C++ common utilities and types library"
        LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE      ON)
set(CMAKE_BUILD_PARALLEL_LEVEL	3)

option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" ON)
option(BUILD_WASM32 "Build static wasm libraries" OFF)

if (BUILD_WASM32)
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_SYSTEM_NAME Emscripten)
    set(CMAKE_SYSTEM_PROCESSOR wasm32)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
            GPCORE_LIBRARY
            HAVE_NETINET_IN_H
            _GLIBCXX_USE_NANOSLEEP
	)

string(JOIN " " COMPILE_FLAGS
        -std=gnu++20
        -O3

        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -ffunction-sections
        -fdata-sections
        -fexceptions
        -fstrict-aliasing
        -fstack-clash-protection

        -Wall
        -Wextra
        -Wdouble-promotion
        -Wswitch-default
        -Wswitch-enum
        -Wuninitialized
        -Wstrict-aliasing
        -Wfloat-equal
        -Wshadow
        -Wplacement-new
        -Wcast-align
        -Wconversion
        -Wlogical-op
        -Wduplicated-cond
        -Wduplicated-branches
        -Wrestrict
        -Wnull-dereference
        -Wno-terminate
        -Wno-unknown-warning-option
        -Wno-unused-command-line-argument
        -Wno-comment
        )

string(JOIN "," LINK_FLAGS
        -Wl
        --gc-sections
        )

#------------------------ OS ---------------------
if(${CMAKE_SYSTEM_NAME} MATCHES Emscripten)
    set(EMSCRIPTEN_ROOT             $ENV{EMSDK}/upstream/emscripten)
    set(CMAKE_TOOLCHAIN_FILE        ${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake)
    set(CMAKE_CXX_COMPILER          em++)
    set(CMAKE_C_COMPILER            emcc)
    set(CMAKE_AR                    emar)
    set(CMAKE_RANLIB                emranlib)
    set(CMAKE_CXX_STANDARD          20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> cqs <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> cqs <TARGET> <LINK_FLAGS> <OBJECTS>")

    string(JOIN " " COMPILE_FLAGS
            ${COMPILE_FLAGS}
            -s ALLOW_MEMORY_GROWTH=1
          )
elseif(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    set(CMAKE_CXX_COMPILER          g++-10)
    set(CMAKE_C_COMPILER            gcc-10)
    set(CMAKE_CXX_STANDARD          20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(${CMAKE_SYSTEM_NAME} MATCHES Android)
elseif(${CMAKE_SYSTEM_NAME} MATCHES Macx)
elseif(${CMAKE_SYSTEM_NAME} MATCHES Windows)

endif()

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            GPCORE_LIBRARY
            HAVE_NETINET_IN_H
            _GLIBCXX_USE_NANOSLEEP
        )

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${LINK_FLAGS})

#------------------------ ARC ---------------------
if (${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -mtune=generic
        -march=x86-64
        )
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -mtune=generic
        -march=i686
        )
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES wasm32)
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES arm_v6)
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES arm64_v8a)
else()
    message(FATAL_ERROR "Unknown ARC. Set -DCMAKE_SYSTEM_PROCESSOR= ... one of values: x86_64, x86, wasm32, arm_v6, arm64_v8a")
endif()

#------------------------ RELEASE ---------------------
set(CMAKE_BUILD_TYPE Release)
string(TOUPPER ${CMAKE_BUILD_TYPE} MODE_BUILD)
message(STATUS "[${PROJECT_NAME}]: ***************** Build mode ${MODE_BUILD} *****************")
target_compile_definitions(${PROJECT_NAME} PRIVATE ${MODE_BUILD}_BUILD)
set(Boost_USE_RELEASE_LIBS ON)

#------------------------ FILES LOCATIONS ---------------------
set(CMAKE_INSTALL_PREFIX	"${CMAKE_SOURCE_DIR}/../.." CACHE PATH "..." FORCE)
set(TARGET_LIB_DIR              "${CMAKE_INSTALL_PREFIX}/lib")
set(TARGET_EXT_INCLUDE_DIR	"${CMAKE_INSTALL_PREFIX}/inc")
set(TARGET_TMP_DIR              "${CMAKE_INSTALL_PREFIX}/tmp")

if (NOT EXISTS "${TARGET_EXT_INCLUDE_DIR}")
    file(MAKE_DIRECTORY "${TARGET_EXT_INCLUDE_DIR}")
endif()



#------------------------ LIBS ---------------------
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${TARGET_EXT_INCLUDE_DIR}"
        "${Boost_INCLUDE_DIR}"
        $<$<BOOL:${BUILD_WASM32}>:${TARGET_EXT_INCLUDE_DIR}/jthread>

    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        pthread
    )

message(STATUS "[${PROJECT_NAME}]: Target name:              " ${PROJECT_NAME})
message(STATUS "[${PROJECT_NAME}]: Target arch:              " ${CMAKE_SYSTEM_PROCESSOR})
message(STATUS "[${PROJECT_NAME}]: Target OS:                " ${CMAKE_SYSTEM_NAME})
message(STATUS "[${PROJECT_NAME}]: Build directory:          " ${LIBRARY_OUTPUT_PATH})
message(STATUS "[${PROJECT_NAME}]: C++ standart:             " ${CMAKE_CXX_STANDARD})
message(STATUS "[${PROJECT_NAME}]: GCC C compiler version:   " ${CMAKE_C_COMPILER})
message(STATUS "[${PROJECT_NAME}]: GCC C++ compiler version: " ${CMAKE_CXX_COMPILER})

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION   ${PROJECT_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_${PROJECT_NAME}_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})

#------------------------ INSTALL ---------------------
install(DIRECTORY        "${CMAKE_SOURCE_DIR}"
        DESTINATION      "${TARGET_EXT_INCLUDE_DIR}"
        FILES_MATCHING
                PATTERN "*.hpp"
                PATTERN "*.h"
                PATTERN "CMake*" EXCLUDE
                PATTERN ".git"   EXCLUDE
        )
