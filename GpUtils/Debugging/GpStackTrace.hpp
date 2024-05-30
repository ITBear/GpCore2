#pragma once

#include "../GpUtils_global.hpp"
#include "../Macro/GpMacroClass.hpp"
#include "GpSourceLocation.hpp"

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
