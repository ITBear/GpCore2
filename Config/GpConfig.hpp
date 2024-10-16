#pragma once

#include <GpCore2/Config/GpEnvironmentDetector.hpp>

//******************* ARCH *****************
#if defined(GP_ARCH_X86)
#   include <GpCore2/Config/GpConfig_arch_x86.hpp>
#elif defined(GP_ARCH_X86_64)
#   include <GpCore2/Config/GpConfig_arch_x86_64.hpp>
#elif defined(GP_ARCH_MIPS)
#   include <GpCore2/Config/GpConfig_arch_mips.hpp>
#elif defined(GP_ARCH_ARMv5T)
#   include <GpCore2/Config/GpConfig_arch_arm_5T.hpp">
#elif defined(GP_ARCH_ARMv7A)
#   include <GpCore2/Config/GpConfig_arch_arm_7A.hpp>
#elif defined(GP_ARCH_ARM_CORTEX_M)
#   include <GpCore2/Config/GpConfig_arch_arm_CORTEX_M.hpp>
#elif defined(GP_ARCH_WASM_32)
#   include <GpCore2/Config/GpConfig_arch_wasm_32.hpp>
#elif defined(GP_ARCH_WASM_64)
#   include <GpCore2/Config/GpConfig_arch_wasm_64.hpp>
#else
#   error Current architecture is not supported yet.
#endif

//******************* OS *****************
#if defined(GP_OS_WINDOWS)
#   include <GpCore2/Config/GpConfig_os_windows.hpp>
#elif defined(GP_OS_LINUX)
#   include <GpCore2/Config/GpConfig_os_linux.hpp>
#elif defined(GP_OS_ANDROID)
#   include <GpCore2/Config/GpConfig_os_android.hpp>
#elif defined(GP_OS_IOS)
#   include <GpCore2/Config/GpConfig_os_ios.hpp>
#elif defined(GP_OS_IOS_SIMULATOR)
#   include <GpCore2/Config/GpConfig_os_ios_simulator.hpp>
#elif defined(GP_OS_MACOSX)
#   include <GpCore2/Config/GpConfig_os_macosx.hpp>
#elif defined(GP_OS_BARE_METAL)
#   include <GpCore2/Config/GpConfig_os_baremetal.hpp>
#elif defined(GP_OS_BROWSER)
#   include <GpCore2/Config/GpConfig_os_browser.hpp>
#else
#   error Current OS is not supported yet.
#endif

#if defined(GP_COMPILER_MSVC)
#   pragma warning(disable : 4275)
#   pragma warning(disable : 4251)
#   pragma warning(disable : 4702)
#   pragma warning(disable : 4710)
#   pragma warning(disable : 4711)
#   pragma warning(disable : 4868)
#   pragma warning(disable : 5045)
#   pragma warning(disable : 5246)
#endif
