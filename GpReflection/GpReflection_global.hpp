#pragma once

#include "../GpUtils/Macro/GpMacroImportExport.hpp"

#if defined(GP_REFLECTION_LIBRARY)
    #define GP_REFLECTION_API GP_DECL_EXPORT
#else
    #define GP_REFLECTION_API GP_DECL_IMPORT
#endif
