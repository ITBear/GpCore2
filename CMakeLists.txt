cmake_minimum_required(VERSION 3.10)
#include(FetchContent)

project(GpCore2
	VERSION 0.1.0
	DESCRIPTION "C++ common utilities and types library"
	LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE      ON)
set(CMAKE_BUILD_PARALLEL_LEVEL	3)

set(SOURCES
	EventBus/GpEventBus.cpp
	EventBus/GpEventSubscriber.cpp
	Exceptions/GpException.cpp
	Exceptions/GpExceptionsSink.cpp
	Multithreading/SyncPrimitives/GpConditionVar.cpp
	Multithreading/Tasks/Fibers/GpTaskFiber.cpp
	Multithreading/Tasks/Fibers/GpTaskFiberCtx.cpp
	Multithreading/Tasks/Fibers/GpTaskFiberManager.cpp
	Multithreading/Tasks/Fibers/GpTaskFiberStack.cpp
	Multithreading/Tasks/Fibers/GpTaskFiberStackPool.cpp
	Multithreading/Tasks/Fibers/GpTaskFiberStage.cpp
	Multithreading/Tasks/GpTask.cpp
	Multithreading/Tasks/GpTaskExecutor.cpp
	Multithreading/Tasks/GpTaskExecutorsPool.cpp
	Multithreading/Tasks/GpTaskScheduler.cpp
	Multithreading/Tasks/GpTaskState.cpp
	Multithreading/Threads/GpRunnable.cpp
	Multithreading/Threads/GpThread.cpp
	Pipeline/GpPipeline.cpp
	Pipeline/GpPipelineEvaluateCtx.cpp
	Pipeline/GpPipelineEvaluator.cpp
	Pipeline/GpPipelineManager.cpp
	Pipeline/GpPipelineNode.cpp
	Pipeline/GpPipelineNodeConnector.cpp
	Pipeline/GpPipelineNodeSocket.cpp
	Pipeline/GpPipelineNodeSocketDir.cpp
	Pipeline/Nodes/GpPipelineNodeSum.cpp
	Pipeline/Nodes/GpPipelineNodeSumEvCtx.cpp
	RandomGenerators/GpRandom.cpp
	RandomGenerators/GpRandomDeviceWin.cpp
	RandomGenerators/GpSRandom.cpp
	Types/DateTime/GpDateTimeOps.cpp
	Types/Enums/GpEnum.cpp
	Types/Numerics/GpNumericOps.cpp
	Types/Strings/GpStringOps.cpp
	Types/TypeSystem/GpType.cpp
	Types/TypeSystem/GpTypeContainer.cpp
	Types/TypeSystem/GpTypeInfo.cpp
	Types/TypeSystem/GpTypePropInfo.cpp
	Types/UIDs/GpUUID.cpp
	)

option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" ON)
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

set(CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME} CACHE STRING "Uno-labs-overridden CMAKE_SYSTEM_NAME")

string(JOIN " " COMPILE_FLAGS
        -std=gnu++20

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
    set(EMCMAKE_COMMAND             ${EMSCRIPTEN_ROOT_PATH}/emcmake)
    set(CMAKE_TOOLCHAIN_FILE        ${EMSCRIPTEN_ROOT_PATH}/cmake/Modules/Platform/Emscripten.cmake)
    set(CMAKE_CXX_COMPILER          ${EMSCRIPTEN_ROOT_PATH}/em++)
    set(CMAKE_C_COMPILER            ${EMSCRIPTEN_ROOT_PATH}/emcc)
    set(CMAKE_AR                    ${EMSCRIPTEN_ROOT_PATH}/emar)
    set(CMAKE_RANLIB                ${EMSCRIPTEN_ROOT_PATH}/emranlib)
    set(CMAKE_CXX_STANDARD          20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> cqs <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> cqs <TARGET> <LINK_FLAGS> <OBJECTS>")

    string(JOIN " " COMPILE_FLAGS
            -s ALLOW_MEMORY_GROWTH=1
          )
elseif(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    set(CMAKE_CXX_COMPILER          g++-10)
    set(CMAKE_C_COMPILER            gcc-10)
    set(CMAKE_CXX_STANDARD          20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(${CMAKE_SYSTEM_NAME} MATCHES Android)
elseif(${CMAKE_SYSTEM_NAME} MATCHES Macx)
elseif(${CMAKE_SYSTEM_NAME} MATCHES Win)
else()
    message(FATAL_ERROR "Unknown OS. Set -DCMAKE_SYSTEM_NAME= ... one of values: Linux, Android, Macx, Win, Browser")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${LINK_FLAGS})

#------------------------ ARC ---------------------
set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "Uno-labs-overridden CMAKE_SYSTEM_PROCESSOR")
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

#------------------------ DEBUG or RELEASE ---------------------
string(TOUPPER ${CMAKE_BUILD_TYPE} MODE_BUILD)
message(STATUS "[${PROJECT_NAME}]: ***************** Build mode ${MODE_BUILD} *****************")
target_compile_definitions(${PROJECT_NAME} PRIVATE ${MODE_BUILD}_BUILD)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -fsanitize=address
        -fsanitize=undefined
        -fno-sanitize=vptr
        )
    set(MODE_SUFFIX _d)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}$<$<CONFIG:Debug>:${MODE_SUFFIX}>")
    set(Boost_USE_DEBUG_LIBS ON)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(Boost_USE_RELEASE_LIBS ON)
else()
    message(FATAL_ERROR "Unknown build mode. Set -DCMAKE_BUILD_TYPE=Debug OR -DCMAKE_BUILD_TYPE=Release")
endif()

#------------------------ FILES LOCATIONS ---------------------
set(CMAKE_INSTALL_PREFIX	"${CMAKE_SOURCE_DIR}/../.." CACHE PATH "..." FORCE)
set(TARGET_LIB_DIR              "${CMAKE_INSTALL_PREFIX}/lib")
set(TARGET_EXT_INCLUDE_DIR	"${CMAKE_INSTALL_PREFIX}/inc")
set(TARGET_TMP_DIR              "${CMAKE_INSTALL_PREFIX}/tmp")

if (NOT EXISTS "${TARGET_EXT_INCLUDE_DIR}")
    file(MAKE_DIRECTORY "${TARGET_EXT_INCLUDE_DIR}")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES Emscripten)
    set(DESTDIR "${TARGET_LIB_DIR}")
else()
    set(SP _)
    set(DESTDIR "${TARGET_LIB_DIR}/${CMAKE_BUILD_TYPE}${SP}${CMAKE_SYSTEM_NAME}${SP}${CMAKE_SYSTEM_PROCESSOR}")
endif()

set(LIBRARY_OUTPUT_PATH	${DESTDIR})

#------------------------ LIBS ---------------------
target_include_directories(${PROJECT_NAME}
    PRIVATE "${TARGET_EXT_INCLUDE_DIR}"
    PRIVATE "${BOOST_INCLUDE}"
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE pthread
    PRIVATE $<$<CONFIG:Debug>:asan>
    PRIVATE $<$<CONFIG:Debug>:ubsan>
    )

message(STATUS "[${PROJECT_NAME}]: Target name:              " ${PROJECT_NAME}${MODE_SUFFIX})
message(STATUS "[${PROJECT_NAME}]: Target arch:              " ${CMAKE_SYSTEM_PROCESSOR})
message(STATUS "[${PROJECT_NAME}]: Target OS:                " ${CMAKE_SYSTEM_NAME})
message(STATUS "[${PROJECT_NAME}]: Build directory:          " ${DESTDIR})
message(STATUS "[${PROJECT_NAME}]: C++ standart:             " ${CMAKE_CXX_STANDARD})
message(STATUS "[${PROJECT_NAME}]: GCC C compiler version:   " ${CMAKE_C_COMPILER})
message(STATUS "[${PROJECT_NAME}]: GCC C++ compiler version: " ${CMAKE_CXX_COMPILER})

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION   ${PROJECT_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_${PROJECT_NAME}_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})

#------------------------ POST BUILD ---------------------
install(DIRECTORY		 "${CMAKE_SOURCE_DIR}"
                DESTINATION      "${TARGET_EXT_INCLUDE_DIR}"
		FILES_MATCHING
			PATTERN "*.hpp"
			PATTERN "*.h"
			PATTERN "CMake*" EXCLUDE
			PATTERN ".git"   EXCLUDE
		)

add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
                COMMAND ${EMCMAKE_COMMAND} ${CMAKE_COMMAND}
                        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                        -DEMSCRIPTEN_ROOT_PATH="${EMSCRIPTEN_ROOT_PATH}"
                        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
                        -DBOOST_INCLUDE="${BOOST_INCLUDE}"
                        -DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}
                        .
	)
