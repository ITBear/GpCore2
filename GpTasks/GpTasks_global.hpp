#pragma once

#include "../GpUtils/GpUtils_global.hpp"
#include <cstddef>

#if defined(GP_TASKS_LIBRARY)
    #define GP_TASKS_API GP_DECL_EXPORT
#else
    #define GP_TASKS_API GP_DECL_IMPORT
#endif

#include "../GpUtils/Types/Numerics/GpNumericTypes.hpp"

namespace GPlatform {

class GpTasksSettings
{
public:
    using bistset_type = u_int_64;

public:
    static constexpr size_t SMaxCoresCount  (void) noexcept {return sizeof(bistset_type)*8;}
};

}//namespace GPlatform
