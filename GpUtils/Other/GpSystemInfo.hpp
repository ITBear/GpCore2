#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

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
