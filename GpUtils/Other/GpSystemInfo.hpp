#pragma once

#include "../GpUtils_global.hpp"
#include "../Macro/GpMacroClass.hpp"

#include <string>

namespace GPlatform {

class GP_UTILS_API GpSystemInfo
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSystemInfo)

public:
    static std::string  SOsName                 (void);
    static std::string  SOsInfo                 (void);
    static std::string  SArcName                (void);
    static size_t       SHardwareConcurrency    (void);
};

}// namespace GPlatform
