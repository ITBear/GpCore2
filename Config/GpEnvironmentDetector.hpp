#pragma once

#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#   if !defined(NOMINMAX)
#       define NOMINMAX
#   endif//if !defined(NOMINMAX)
#   define _CRT_RAND_S
#   include <Windows.h>
#endif

#if defined(__APPLE__)
#   include <TargetConditionals.h>
#endif

//************************* COMPILER *****************************************
#if defined(__INTEL_COMPILER) || defined(__ICC)
#   define  GP_COMPILER_ICC
#   define  GP_COMPILER_NAME_STR u8"Intel"
#   define  GP_COMPILER_NAME intel
#elif defined(__clang__)
#   define  GP_COMPILER_CLANG
#   define  GP_COMPILER_NAME_STR u8"Clang"
#   define  GP_COMPILER_NAME clang
#elif defined(__GNUC__) || defined(__GNUG__)
#   define  GP_COMPILER_GCC
#   define  GP_COMPILER_NAME_STR u8"GCC"
#   define  GP_COMPILER_NAME GCC
#elif defined(_MSC_VER)
#   define  GP_COMPILER_MSVS
#   define  GP_COMPILER_NAME_STR u8"Visual studio"
#   define  GP_COMPILER_NAME msvs
#else
#   error Unknown compiler
#endif

#if !defined(GP_MANUAL_ENVIRONMENT_SETTINGS)

//************************ CPU ARCHITECTURE ***********************************
#   if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(__X86__) || defined(_X86_)
#       define GP_ARCH_X86
#       define GP_ARCH_NAME u8"x86"
#       define GP_ENVIRONMENT_32
#   elif defined(__amd64__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
#       define GP_ARCH_X86_64
#       define GP_ARCH_NAME u8"x86_64"
#       define GP_ENVIRONMENT_64
#   elif defined(__mips__) || defined(mips)
#       define GP_ARCH_MIPS
#       define GP_ARCH_NAME u8"mips"
#   elif defined(__arm__) || defined(__aarch64__)

#       if defined(__SOFTFP__)
#           error Software ARM FP operations
#       endif

#       define GP_ARCH_ARM

#       if defined(__aarch64__)
#           define GP_ARCH_ARM_64
#           define GP_ARCH_NAME u8"ARM_64"
#           define GP_ENVIRONMENT_64
#       elif defined(__ARM_ARCH_2__)                                    //ARMv2: (ARM2, ARM250, ARM3)
#           define GP_ARCH_ARMv2
#           define GP_ARCH_NAME u8"ARMv2"
#           define GP_ENVIRONMENT_32
#       elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)        //ARMv3: (ARM6, ARM7)
#           define GP_ARCH_ARMv3
#           define GP_ARCH_NAME u8"ARMv3"
#           define GP_ENVIRONMENT_32
#       elif defined(__ARM_ARCH_4__)                                    //ARMv4: (ARM8)
#           define GP_ARCH_ARMv4
#           define GP_ARCH_NAME u8"ARMv4"
#           define GP_ENVIRONMENT_32
#       elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)       //ARMv4T:(ARM7TDMI, ARM9TDMI, SecurCore SC100)
#           define GP_ARCH_ARMv4T
#           define GP_ARCH_NAME u8"ARMv4T"
#           define GP_ENVIRONMENT_32
#       elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__)        //ARMv5
#           define GP_ARCH_ARMv5
#           define GP_ARCH_NAME u8"ARMv5"
#           define GP_ENVIRONMENT_32
#       elif defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) //ARMv5T:()
#           define GP_ARCH_ARMv5T
#           define GP_ARCH_NAME u8"ARMv5T"
#           define GP_ENVIRONMENT_32
#       elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)  //ARMv6:(ARM11)
#           define GP_ARCH_ARMv6
#           define GP_ARCH_NAME u8"ARMv6"
#           define GP_ENVIRONMENT_32
#       elif defined(__ARM_ARCH_6M__) && (__ARM_ARCH_6M__ == 1) //ARMv6M: (Cortex-M0, Cortex-M0+, Cortex-M1, SecurCore SC000)
#           define GP_ARCH_ARMv6_M
#           define GP_ARCH_NAME u8"ARMv6-M"
#           define GP_ENVIRONMENT_32
#           define GP_ARCH_ARM_CORTEX_M
#       elif defined(__ARM_ARCH_7M__) && (__ARM_ARCH_7M__ == 1) //ARMv7-M: (Cortex-M3, SecurCore SC300)
#           define GP_ARCH_ARMv7_M
#           define GP_ARCH_NAME u8"ARMv7-M"
#           define GP_ENVIRONMENT_32
#           define GP_ARCH_ARM_CORTEX_M
#       elif defined(__ARM_ARCH_7EM__) && (__ARM_ARCH_7EM__ == 1) //ARMv7E-M: (Cortex-M4, Cortex-M7)
#           define GP_ARCH_ARMv7E_M
#           define GP_ARCH_NAME u8"ARMv7E-M"
#           define GP_ENVIRONMENT_32
#           define GP_ARCH_ARM_CORTEX_M
#       elif defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ == 1) //ARMv8-M: (ARM Cortex-M23)
#           define GP_ARCH_ARMv8_M
#           define GP_ARCH_NAME u8"ARMv8-M"
#           define GP_ENVIRONMENT_32
#           define GP_ARCH_ARM_CORTEX_M
#       elif defined(__ARM_ARCH_8M_MAIN__) && (__ARM_ARCH_8M_MAIN__ == 1) //ARMv8-M: (ARM Cortex-M33)
#           define GP_ARCH_ARMv8_M
#           define GP_ARCH_NAME u8"ARMv8-M"
#           define GP_ENVIRONMENT_32
#           define GP_ARCH_ARM_CORTEX_M
#       elif defined(__ARM_ARCH_7A__)
#           define GP_ARCH_ARMv7A   //ARMv7-A (ARM Cortex-A5, ARM Cortex-A7, ARM Cortex-A8, ARM Cortex-A9, ARM Cortex-A12, ARM Cortex-A15, ARM Cortex-A17)
#           define GP_ARCH_NAME u8"ARMv7-A"
#           define GP_ENVIRONMENT_32
#       else
#           error Can`t detect ARM CPU architecture
#       endif
#   elif defined(__wasm)
#       define OS_BROWSER
#       if defined(__wasm32__) || defined(__wasm32)
#           define GP_ARCH_WASM_32
#           define GP_ARCH_NAME u8"WASM_32"
#           define GP_ENVIRONMENT_32
#       elif defined(__wasm64__) || defined(__wasm64)
#           define GP_ARCH_WASM_64
#           define GP_ARCH_NAME u8"WASM_64"
#           define GP_ENVIRONMENT_64
#       else
#           error Can`t detect WASM architecture
#       endif
#   else
#       error Can`t detect CPU architecture
#   endif

//**************************** OS ****************************
#   if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#       define GP_OS_WINDOWS
#       define GP_OS_NAME u8"Windows"
#   elif defined(ANDROID) || defined(__ANDROID__)
#       define GP_OS_ANDROID
#       define GP_OS_NAME u8"Android"
#       define GP_POSIX
#   elif defined(__linux__)
#       define GP_OS_LINUX
#       define GP_OS_NAME u8"Linux"
#       define GP_POSIX
#   elif defined(TARGET_OS_IPHONE) && (TARGET_OS_IPHONE == 1)
#       define GP_OS_IOS
#       define GP_OS_NAME u8"iOS"
#       define GP_POSIX
#   elif defined(TARGET_IPHONE_SIMULATOR) && (TARGET_IPHONE_SIMULATOR == 1)
#       define GP_OS_IOS_SIMULATOR
#       define GP_OS_NAME u8"iOS_sim"
#       define GP_POSIX
#   elif defined(TARGET_OS_MAC) && (TARGET_OS_MAC == 1)
#       define GP_OS_MACOSX
#       define GP_OS_NAME u8"MacOSX"
#       define GP_POSIX
#   elif defined(OS_BARE_METAL)
#       define GP_OS_BARE_METAL
#   elif defined(OS_BROWSER)
#       define GP_OS_BROWSER
#   else
#       error Can`t detect OS platform...
#   endif

#endif//#if !defined(GP_MANUAL_ENVIRONMENT_SETTINGS)
