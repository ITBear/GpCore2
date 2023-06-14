#pragma once

#include "../GpUtils/GpUtils_global.hpp"

#if defined(GP_REFLECTION_LIBRARY)
    #define GP_REFLECTION_API GP_DECL_EXPORT
#else
    #define GP_REFLECTION_API GP_DECL_IMPORT
#endif
