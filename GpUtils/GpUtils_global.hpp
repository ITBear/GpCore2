#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroImportExport.hpp>

#if defined(GP_UTILS_LIBRARY)
    #define GP_UTILS_API GP_DECL_EXPORT
#else
    #define GP_UTILS_API GP_DECL_IMPORT
#endif

namespace GPlatform {}
