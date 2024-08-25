#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Debugging/GpSourceLocation.hpp>

namespace GPlatform {

class GP_UTILS_API GpDebugging
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDebugging)

public:
#if defined(DEBUG_BUILD)
    static bool     SIsUnderDebugger        (void);
    static void     SBreakpointIfDebugging  (const SourceLocationT& aSourceLocation = SourceLocationT::current());
    static void     SBreakpoint             (const SourceLocationT& aSourceLocation = SourceLocationT::current());
#else
    static bool     SIsUnderDebugger        (void) {return false;}
    static void     SBreakpointIfDebugging  ([[maybe_unused]] const SourceLocationT& aSourceLocation = SourceLocationT::current()) {}
    static void     SBreakpoint             ([[maybe_unused]] const SourceLocationT& aSourceLocation = SourceLocationT::current()) {}
#endif// #if defined(DEBUG_BUILD)
};

}// namespace GPlatform
