#pragma once

#include "GpEnvironmentDetector.hpp"
#include "GpCompilerFeatures.hpp"

//******************* ARCH *****************
#if defined(GP_ARCH_X86)
#   include "GpConfig_arch_x86.hpp"
#elif defined(GP_ARCH_X86_64)
#   include "GpConfig_arch_x86_64.hpp"
#elif defined(GP_ARCH_MIPS)
#   include "GpConfig_arch_mips.hpp"
#elif defined(GP_ARCH_ARMv5T)
#   include "GpConfig_arch_arm_5T.hpp"
#elif defined(GP_ARCH_ARMv7A)
#   include "GpConfig_arch_arm_7A.hpp"
#elif defined(GP_ARCH_ARM_CORTEX_M)
#   include "GpConfig_arch_arm_CORTEX_M.hpp"
#elif defined(GP_ARCH_WASM_32)
#   include "GpConfig_arch_wasm_32.hpp"
#elif defined(GP_ARCH_WASM_64)
#   include "GpConfig_arch_wasm_64.hpp"
#else
#   error Current architecture is not supported yet.
#endif

//******************* OS *****************
#if defined(GP_OS_WINDOWS)
#   include "GpConfig_os_windows.hpp"
#elif defined(GP_OS_LINUX)
#   include "GpConfig_os_linux.hpp"
#elif defined(GP_OS_ANDROID)
#   include "GpConfig_os_android.hpp"
#elif defined(GP_OS_IOS)
#   include "GpConfig_os_ios.hpp"
#elif defined(GP_OS_IOS_SIMULATOR)
#   include "GpConfig_os_ios_simulator.hpp"
#elif defined(GP_OS_MACOSX)
#   include "GpConfig_os_macosx.hpp"
#elif defined(GP_OS_BARE_METAL)
#   include "GpConfig_os_baremetal.hpp"
#elif defined(GP_OS_BROWSER)
#   include "GpConfig_os_browser.hpp"
#else
#   error Current OS is not supported yet.
#endif

//******************* Diagnostics *****************
#if defined(GP_COMPILER_GCC) || defined(GP_COMPILER_CLANG)
#   pragma GCC diagnostic ignored "-Wpragmas"
#   pragma GCC diagnostic ignored "-Wcovered-switch-default"
#   pragma GCC diagnostic ignored "-Wweak-vtables"
#   pragma GCC diagnostic ignored "-Wunreachable-code-break"
#   pragma GCC diagnostic ignored "-Wgnu-string-literal-operator-template"
#endif
