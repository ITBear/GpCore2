#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroImportExport.hpp>
#include <cstddef>

#if defined(GP_TASKS_LIBRARY)
    #define GP_TASKS_API GP_DECL_EXPORT
#else
    #define GP_TASKS_API GP_DECL_IMPORT
#endif

#include <GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTasksSettings
{
public:
    using bistset_type = u_int_64;

public:
    static constexpr size_t SMaxCoresCount  (void) noexcept {return sizeof(bistset_type)*8;}
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
