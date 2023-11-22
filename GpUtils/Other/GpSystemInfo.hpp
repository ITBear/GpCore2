#pragma once

#include "../GpUtils_global.hpp"
#include "../Macro/GpMacroClass.hpp"

#include <string>

namespace GPlatform {

class GP_UTILS_API GpSystemInfo
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSystemInfo)

public:
    static std::u8string    SOsName                 (void);
    static std::u8string    SOsInfo                 (void);
    static std::u8string    SArcName                (void);
    static size_t           SHardwareConcurrency    (void);
};

}// GPlatform
