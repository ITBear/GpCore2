#pragma once

#include "../GpUtils/GpUtils.hpp"

#if defined(GP_TASKS_LIBRARY)
    #define GP_TASKS_API GP_DECL_EXPORT
#else
    #define GP_TASKS_API GP_DECL_IMPORT
#endif

namespace GPlatform {

class GpTasksSettings
{
public:
    static constexpr size_t SMaxCoresCount  (void) noexcept {return 128;}
};

}//namespace GPlatform
