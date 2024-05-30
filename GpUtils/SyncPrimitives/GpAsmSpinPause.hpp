#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#if defined(GP_ARCH_X86_64) || defined(GP_ARCH_X86)
#   include <emmintrin.h>
#endif

namespace GPlatform {

inline void GP_ASM_SPIN_PAUSE (void) noexcept
{
#if defined(GP_ARCH_X86_64) || defined(GP_ARCH_X86)
    _mm_pause();
#elif defined(GP_ARCH_ARM)
    asm volatile ("yield");
#else
    //NOP
//# error Unsupported arc
#endif
}

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
