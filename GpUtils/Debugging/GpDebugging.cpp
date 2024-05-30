#include "GpDebugging.hpp"

#if defined(DEBUG_BUILD)

#include "../Exceptions/GpException.hpp"

#if defined(GP_COMPILER_MSVC)
#   include <intrin.h>
#endif

#if defined(GP_POSIX)
#   include <signal.h>
#endif

namespace GPlatform {

//TODO: add std::breakpoint support (C++26)

//void  GpDebugging_SOsBreakpoint (void)
//{
//#if defined(GP_POSIX) && defined(SIGTRAP)
//  raise(SIGTRAP);
//#elif defined(GP_OS_WINDOWS)
//
//#else
//# error Unsupported OS
//#endif
//}

void    GpDebugging_SArchBreakpoint (void)
{
#if defined(GP_ARCH_X86)
#   if  defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
        __asm__ volatile("int $0x03");
#   elif defined(GP_COMPILER_MSVC)
        __asm {
            int 0x03;
        }
#   else
#       error Unsupported compiler
#   endif
#elif defined(GP_ARCH_X86_64)
#   if  defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
        __asm__ volatile("int $0x03");
#   elif defined(GP_COMPILER_MSVC)
        __debugbreak();
#   else
#       error Unsupported compiler
#   endif
#else
#   error Unsupported ARCH
#endif
}

bool    GpDebugging::SIsUnderDebugger (void)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

void    GpDebugging::SBreakpointIfDebugging (const SourceLocationT& aSourceLocation)
{
    if (SIsUnderDebugger())
    {
        SBreakpoint(aSourceLocation);
    }
}

void    GpDebugging::SBreakpoint ([[maybe_unused]] const SourceLocationT& aSourceLocation)
{
    // TODO: add source location to std out

#if defined(GP_COMPILER_CLANG)
#   if (__has_builtin(__builtin_debugtrap))
        __builtin_debugtrap();
#   else
        // TODO: check __builtin_trap
        //???__builtin_trap();
        GpDebugging_SArchBreakpoint();
#   endif
#elif defined(GP_COMPILER_GCC)
    GpDebugging_SArchBreakpoint();
#elif defined(GP_COMPILER_MSVC)
    __debugbreak();
#else
#   error Unsupported compiller
#endif
}

}// namespace GPlatform

#endif// #if defined(DEBUG_BUILD)
