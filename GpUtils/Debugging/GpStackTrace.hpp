#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Debugging/GpSourceLocation.hpp>

namespace GPlatform {

class GP_UTILS_API GpStackTrace
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpStackTrace)

public:
    static std::string  STraceToStr         (void);
    static void         SPrintStacktrace    (void);
    static void         SPrintStacktrace    (std::string_view aMessage);
};

}// namespace GPlatform
